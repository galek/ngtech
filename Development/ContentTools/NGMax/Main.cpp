#include <max.h>
#include <stdmat.h>
#include <vector>
#include "String.h"

#pragma comment(lib, "core.lib") 
#pragma comment(lib, "maxutil.lib")
#pragma comment(lib, "geom.lib")
#pragma comment(lib, "mesh.lib")
#pragma comment(lib, "comctl32.lib")

namespace NGTech {
	struct Vertex {
		Point3 position;
		Point3 normal;
		Point3 texcoord;
	};

	struct Subset {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		String name;

		INode *node;

		Point3 min;
		Point3 max;
	};

	/**
	NGmax ENUM PROC
	*/
	class NGEnumProc : public ITreeEnumProc {
	public:
		NGEnumProc(Interface *i) { iface = i; }

		int callback(INode *node);
		void exportAll(const TCHAR *);

		TriObject *NODE2OBJ(INode *node, int &deleteIt);

		Interface *iface;
		bool selected;

		std::vector<INode*> nodes;
		std::vector<Subset> subsets;
	};

	/*
	MAIN EXPORT
	*/
	void NGEnumProc::exportAll(const TCHAR *name) {
		//Get number of surfaces
		int numObjects = 0;
		for (int n = 0; n < nodes.size(); n++) {
			INode *node = nodes[n];

			int d;
			TriObject *object = NODE2OBJ(node, d);

			if (!object) continue;
			numObjects++;
		}

		/*
		Process model
		*/
		for (int n = 0; n < nodes.size(); n++) {
			INode *node = nodes[n];

			Subset subset;
			subset.node = node;

			int d;
			TriObject *object = NODE2OBJ(node, d);
			if (!object) continue;

			subset.name = _tcslwr((wchar_t*)node->GetName());
			replaceSpaces(subset.name);

			/*
			Read model
			*/
			Matrix3 tm = node->GetObjTMAfterWSM(iface->GetTime());
			Matrix3 nm = tm;
			nm.NoScale();
			nm.NoTrans();

			/*Get position*/
			Box3 box;
			object->mesh.buildBoundingBox();
			box = object->mesh.getBoundingBox(&tm);
			subset.min = box.Min();
			subset.max = box.Max();

			object->mesh.buildNormals();

			for (int i = 0; i < object->mesh.numFaces; i++) {
				Face *f = &object->mesh.faces[i];
				TVFace *tf = &object->mesh.tvFace[i];

				for (int j = 0; j < 3; j++) {
					Vertex vertex;

					Point3 v = tm * object->mesh.verts[f->v[j]];

					vertex.position.x = v.x;
					vertex.position.y = v.y;
					vertex.position.z = v.z;

					vertex.texcoord.x = object->mesh.tVerts[tf->t[j]].x;
					vertex.texcoord.y = object->mesh.tVerts[tf->t[j]].y;

					//read normal
					bool specifiedNormal = true;

					RVertex *rv = object->mesh.getRVertPtr(f->v[j]);
					int nnormals;

					if (rv->rFlags & SPECIFIED_NORMAL) {
						Point3 normal = nm * rv->rn.getNormal();
						vertex.normal.x = normal.x;
						vertex.normal.y = normal.y;
						vertex.normal.z = normal.z;
					}
					else {
						if ((nnormals = rv->rFlags & NORCT_MASK) && f->smGroup) {
							if (nnormals == 1) {
								Point3 normal = nm * rv->rn.getNormal();
								vertex.normal.x = normal.x;
								vertex.normal.y = normal.y;
								vertex.normal.z = normal.z;
							}
							else {
								for (int l = 0; l < nnormals; l++) {
									if (rv->ern[l].getSmGroup() & f->smGroup) {
										Point3 normal = nm * rv->ern[l].getNormal();
										vertex.normal.x = normal.x;
										vertex.normal.y = normal.y;
										vertex.normal.z = normal.z;
									}
								}
							}
						}
						else {
							specifiedNormal = false;
							Point3 normal = nm * object->mesh.getFaceNormal(i);
							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;
						}
					}

					//make indices
					int index = -1;
					for (int k = 0; k < subset.vertices.size(); k++) {
						if ((subset.vertices[k].position == vertex.position) &&
							(subset.vertices[k].texcoord == vertex.texcoord)) {
							if (specifiedNormal) {
								if (subset.vertices[k].normal == vertex.normal) {
									index = k;
								}
							}
							else {
								index = k;
								subset.vertices[k].normal += vertex.normal;
							}
						}
					}

					if (index > -1) {
						subset.indices.push_back(index);
					}
					else {
						subset.indices.push_back(subset.vertices.size());
						subset.vertices.push_back(vertex);
					}
				}
			}

			//normalize normals
			for (int k = 0; k < subset.vertices.size(); k++) {
				subset.vertices[k].normal = subset.vertices[k].normal.Normalize();
			}

			subsets.push_back(subset);
		}

		//get position
		Point3 pos;
		Point3 smin = subsets[0].min;
		Point3 smax = subsets[0].max;

		for (int s = 1; s < subsets.size(); s++) {
			smin.x = min(smin.x, subsets[0].min.x);
			smin.y = min(smin.y, subsets[0].min.y);
			smin.z = min(smin.z, subsets[0].min.z);

			smax.x = max(smax.x, subsets[0].max.x);
			smax.y = max(smax.y, subsets[0].max.y);
			smax.z = max(smax.z, subsets[0].max.z);
		}

		pos = (smin + smax);
		pos.x = pos.x * 0.5;
		pos.y = pos.y * 0.5;
		pos.z = pos.z * 0.5;

		//Open material list file for writing
		String ml = String((TCHAR*)name);
		cutFileExt(ml);
		ml = ml + _T(".matlst");
		FILE *fmtrlst = _tfopen(ml.c_str(), _T("wt"));

		//Save to file
		FILE *fmdl = _tfopen(name, _T("wt"));

		//Write mesh
		_ftprintf(fmdl, _T("#NG_GRAPHICS_FORMAT\n\n"));
		_ftprintf(fmdl, _T("num_subsets %i\n\n"), subsets.size());

		for (int s = 0; s < subsets.size(); s++) {
			//Save subset to file
			Subset &st = subsets[s];

			_ftprintf(fmdl, _T("subset \"%s\"\n"), st.name.c_str());
			_ftprintf(fmdl, _T("num_vertices %i\n"), st.vertices.size());

			for (int v = 0; v < st.vertices.size(); v++)	{
				_ftprintf(fmdl, _T("%f %f %f %f %f %f %f %f\n"),
					st.vertices[v].position.x - pos.x,
					st.vertices[v].position.z - pos.z,
					st.vertices[v].position.y - pos.y,
					st.vertices[v].normal.x,
					st.vertices[v].normal.z,
					st.vertices[v].normal.y,
					st.vertices[v].texcoord.x,
					1.0 - st.vertices[v].texcoord.y);
			};

			fprintf(fmdl, "num_faces %i\n", st.indices.size() / 3);

			for (int f = 0; f < st.indices.size() / 3; f++) {
				_ftprintf(fmdl, _T("%i %i %i\n"),
					st.indices[3 * f + 0],
					st.indices[3 * f + 2],
					st.indices[3 * f + 1]);
			};
			_ftprintf(fmdl, _T("\n"));

			//Save material
			Mtl *m = st.node->GetMtl();

			if (!m || m->ClassID() != Class_ID(DMTL_CLASS_ID, 0)) {
			}
			else {
				String tex0;
				Texmap *tmap = m->GetSubTexmap(ID_DI);

				if (!tmap || tmap->ClassID() != Class_ID(BMTEX_CLASS_ID, 0)) {
					tex0 = _T("");
				}
				else {
					BitmapTex *bmt = (BitmapTex *)tmap;
					tex0 = _tcslwr((TCHAR *)bmt->GetMapName());
					getFileName(tex0);
					replaceSpaces(tex0);

					FILE *fmtr = _tfopen(String(st.name + _T(".mat")).c_str(), _T("wt"));

					_ftprintf(fmtr, _T("pass Ambient\n"));
					_ftprintf(fmtr, _T("{\n"));
					_ftprintf(fmtr, _T("shader = mesh_ambient.glsl\n"));
					_ftprintf(fmtr, _T("mat4 u_mvp = scene::mvp\n"));
					_ftprintf(fmtr, _T("vec3 u_light_color = scene::light_color\n"));
					_ftprintf(fmtr, _T("sampler2D u_base_map = %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("}\n\n"));

					_ftprintf(fmtr, _T("pass LightOmni\n"));
					_ftprintf(fmtr, _T("{\n"));
					_ftprintf(fmtr, _T("shader = mesh_omni.glsl\n"));
					_ftprintf(fmtr, _T("float u_spec_coef = 0.5\n"));
					_ftprintf(fmtr, _T("float u_shininess = 24\n"));
					_ftprintf(fmtr, _T("mat4 u_mvp = scene::mvp\n"));
					_ftprintf(fmtr, _T("mat4 u_world = scene::world\n"));
					_ftprintf(fmtr, _T("vec3 u_light_color = scene::light_color\n"));
					_ftprintf(fmtr, _T("vec3 u_light_pos = scene::light_position\n"));
					_ftprintf(fmtr, _T("vec3 u_view_pos = scene::view_position\n"));
					_ftprintf(fmtr, _T("float u_inv_radius = scene::light_iradius\n"));
					_ftprintf(fmtr, _T("sampler2D u_base_map = %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("sampler2D u_bump_map = normal_map() %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("sampler2D u_shadow_map = scene::shadow_map\n"));
					_ftprintf(fmtr, _T("}\n\n"));

					_ftprintf(fmtr, _T("pass LightSpot\n"));
					_ftprintf(fmtr, _T("{\n"));
					_ftprintf(fmtr, _T("shader = mesh_spot.glsl\n"));
					_ftprintf(fmtr, _T("float u_spec_coef = 0.5\n"));
					_ftprintf(fmtr, _T("float u_shininess = 24\n"));
					_ftprintf(fmtr, _T("mat4 u_mvp = scene::mvp\n"));
					_ftprintf(fmtr, _T("mat4 u_world = scene::world\n"));
					_ftprintf(fmtr, _T("mat4 u_spot_proj_transform = scene::spot_transform\n"));
					_ftprintf(fmtr, _T("vec3 u_light_color = scene::light_color\n"));
					_ftprintf(fmtr, _T("vec3 u_light_pos = scene::light_position\n"));
					_ftprintf(fmtr, _T("vec3 u_light_dir = scene::light_direction\n"));
					_ftprintf(fmtr, _T("float u_inv_radius = scene::light_iradius\n"));
					_ftprintf(fmtr, _T("vec3 u_view_pos = scene::view_position\n"));
					_ftprintf(fmtr, _T("sampler2D u_base_map = %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("sampler2D u_bump_map = normal_map() %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("sampler2D u_shadow_map = scene::shadow_map\n"));
					_ftprintf(fmtr, _T("sampler2D u_spot_proj_map = scene::spot_map\n"));
					_ftprintf(fmtr, _T("}\n\n"));

					_ftprintf(fmtr, _T("pass LightDirect\n"));
					_ftprintf(fmtr, _T("{\n"));
					_ftprintf(fmtr, _T("shader = mesh_direct.glsl\n"));
					_ftprintf(fmtr, _T("float u_specular = 0.5\n"));
					_ftprintf(fmtr, _T("float u_shininess = 24\n"));
					_ftprintf(fmtr, _T("mat4 u_mvp = scene::mvp\n"));
					_ftprintf(fmtr, _T("mat4 u_world = scene::world\n"));
					_ftprintf(fmtr, _T("vec3 u_light_color = scene::light_color\n"));
					_ftprintf(fmtr, _T("vec3 u_light_dir = scene::light_direction\n"));
					_ftprintf(fmtr, _T("vec3 u_view_pos = scene::view_position\n"));
					_ftprintf(fmtr, _T("sampler2D u_base_map = %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("sampler2D u_bump_map = normal_map() %s\n"), tex0.c_str());
					_ftprintf(fmtr, _T("}"));

					fclose(fmtr);
				}
			}

			//Save material_list
			_ftprintf(fmtrlst, _T("\"%s\" \"%s\"\n"), st.name.c_str(), String(st.name + _T(".mat")).c_str());
		}

		fclose(fmdl);
		fclose(fmtrlst);
	};

	/*
	Callback
	*/
	int NGEnumProc::callback(INode *node) {
		if (selected && node->Selected() == FALSE) {
			return TREE_CONTINUE;
		}
		nodes.push_back(node);
		return TREE_CONTINUE;
	}


	/*
	Convert node to triobj
	*/
	TriObject *NGEnumProc::NODE2OBJ(INode *node, int &deleteIt) {
		deleteIt = false;
		Object *obj = node->EvalWorldState(iface->GetTime()).obj;
		if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		{
			TriObject *tri = (TriObject *)obj->ConvertToType(iface->GetTime(),
				Class_ID(TRIOBJ_CLASS_ID, 0));
			if (obj != tri) deleteIt = true;
			return tri;
		}
		else return NULL;
	}


	/*
	NGMax
	*/
	class NGMax : public SceneExport {
	public:
		int          ExtCount()   { return 1; }
		const TCHAR* Ext(int i)   { if (i == 0) return _T("nggf"); else return _T(""); }
		const TCHAR* LongDesc()   { return _T("NGMax"); }
		const TCHAR* ShortDesc()  { return _T("NGMax"); }
		const TCHAR* AuthorName() { return _T("NG Games"); }
		const TCHAR* CopyrightMessage() { return _T("NG Games (C) 2014"); }
		const TCHAR* OtherMessage1()    { return _T(""); }
		const TCHAR* OtherMessage2()    { return _T(""); }
		unsigned int Version()          { return 0.2; }

		void ShowAbout(HWND hWnd) { MessageBox(hWnd, _T("About"), _T("NGExporter"), MB_OK); }
		BOOL SupportsOptions(int ext, DWORD options) { return (options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE; }

		int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts = FALSE, DWORD options = 0);

		NGMax() {}
		virtual ~NGMax() {}
	};

	/*
	DO EXPORT
	*/
	int NGMax::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts, DWORD options) {
		NGEnumProc mProc(i);
		mProc.selected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

		ei->theScene->EnumTree(&mProc);

		mProc.exportAll(name);

		MessageBox(0, _T("Mesh was succesfully exported!"), _T("Succses!"), MB_OK);
		return 1;
	}

	/*
	plugin entry
	*/
	HINSTANCE hInstance;
	int controlsInit = FALSE;

	BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) {
		hInstance = hinstDLL;
		if (!controlsInit) {
			controlsInit = TRUE;
			InitCommonControls();
		}
		return TRUE;
	}

	/*
	Class description
	*/
	class NGMaxClassDesc : public ClassDesc {
	public:
		int IsPublic() { return 1; }
		void *Create(BOOL loading = FALSE) { return new NGMax; }

		const TCHAR *ClassName() { return _T("NGExporter"); }
		SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
		Class_ID ClassID()       { return Class_ID(0x1f527516, 0x2fc1118d); }
		const TCHAR *Category()  { return _T(""); }
	};

	/*
	NGMaxClassDesc
	*/
	static NGMaxClassDesc _gNGMaxDesc;
}
/*
Number of plugins
*/
__declspec(dllexport) int LibNumberClasses() {
	return 1;
}

/*
LibClassDesc
*/
__declspec(dllexport) ClassDesc *LibClassDesc(int i) {
	return (i == 0) ? &NGTech::_gNGMaxDesc : 0;
}

/*
LibDesc
*/
__declspec(dllexport) const TCHAR *LibDescription() {
	return _T("NGMax");
}

/*
LibVersion
*/
__declspec(dllexport) ULONG LibVersion() {
	return VERSION_3DSMAX;
}

/*
*/
__declspec(dllexport)ULONG CanAutoDefer() {
	return 1;
}