#include <max.h>
#include <stdmat.h>
#include <vector>
#include <TCHAR.H>
#include "String.h"
#include "MathLib.h"

#pragma comment(lib, "core.lib") 
#pragma comment(lib, "maxutil.lib")
#pragma comment(lib, "geom.lib")
#pragma comment(lib, "mesh.lib")
#pragma comment(lib, "comctl32.lib")

static void W(FILE *f, String s) { _ftprintf(f, _T("%s\n"), s.c_str()); };

struct Vertex {
	Point3 position;
	Point3 normal;
	Point3 texcoord;
	int flag;
};

class AstEnumProc : public ITreeEnumProc {
public:
	AstEnumProc(Interface *i) { iface = i; }

	int callback(INode *node);
	void exportAll(const TCHAR *);

	TriObject *NODE2OBJ(INode *node, int &deleteIt);

	Interface *iface;

	std::vector<INode*> nodes;
};

//---------------------------------------------------------------------------
//MAIN EXPORT
//---------------------------------------------------------------------------
void AstEnumProc::exportAll(const TCHAR *name) {
	FILE *fmdl = _tfopen(name, _T("wt"));

	//-----------------------------------------------
	//Get number of surfaces
	//-----------------------------------------------
	int nobj = 0;
	for (int n = 0; n < nodes.size(); n++) {
		INode *node = nodes[n];
		int d; TriObject *object = NODE2OBJ(node, d);
		if (!object) continue;
		nobj++;
	}

	//-----------------------------------------------
	//Open material list file for writing
	//-----------------------------------------------
	std::wstring ml = std::wstring((wchar_t*) name);
	cutFileExt(ml);
	ml = ml + _T(".amtrlst");
	FILE *fmtrlst = _tfopen(ml.c_str(), _T("wt"));

	//-----------------------------------------------
	//get model position
	//-----------------------------------------------
	Point3 min, max;
	for (int n = 0; n < nobj; n++) {
		INode *node = nodes[n];
		int d; TriObject *object = NODE2OBJ(node, d);
		if (!object) continue;

		object->mesh.buildBoundingBox();
		Box3 box = object->mesh.getBoundingBox();

		if (n == 0) {
			min.x = box.Min().x;
			min.y = box.Min().y;
			min.z = box.Min().z;

			max.x = box.Max().x;
			max.y = box.Max().y;
			max.z = box.Max().z;
		}
		else {
			if (min.x > box.Min().x) min.x = box.Min().x;
			if (min.y > box.Min().y) min.y = box.Min().y;
			if (min.z > box.Min().z) min.z = box.Min().z;

			if (max.x < box.Max().x) max.x = box.Max().x;
			if (max.y < box.Max().y) max.y = box.Max().y;
			if (max.z < box.Max().z) max.z = box.Max().z;
		}
	}
	Point3 pos;
	pos.x = (min.x + max.x) * 0.5;
	pos.y = (min.y + max.y) * 0.5;
	pos.z = (min.z + max.z) * 0.5;

	//-----------------------------------------------
	//Write number of surfaces
	//-----------------------------------------------
	_ftprintf(fmdl, _T("#AST_ENGINE_MODEL\n\n"));
	_ftprintf(fmdl, _T("num_subsets %i\n\n"), nobj);

	//-----------------------------------------------
	//Process model
	//-----------------------------------------------
	for (int n = 0; n < nodes.size(); n++) {
		INode *node = nodes[n];
		int d; TriObject *object = NODE2OBJ(node, d);
		if (!object) continue;

		String subsetName = _tcslwr((wchar_t*) node->GetName());
		replaceSpaces(subsetName);

		//-------------------------------------------------
		//Read model
		//-------------------------------------------------
		Matrix3 tm = node->GetObjTMAfterWSM(iface->GetTime());
		Matrix3 nm = tm;
		nm.NoScale();
		nm.NoTrans();

		object->mesh.buildNormals();

		Vertex *m_vertices = new Vertex[object->mesh.numFaces * 3];

		for (int i = 0; i < object->mesh.numFaces; i++) {
			Face *f = &object->mesh.faces[i];
			TVFace *tf = &object->mesh.tvFace[i];

			for (int j = 0; j < 3; j++) {
				Point3 v = tm * object->mesh.verts[f->v[j]];
				m_vertices[3 * i + j].position.x = v.x;
				m_vertices[3 * i + j].position.y = v.y;
				m_vertices[3 * i + j].position.z = v.z;

				m_vertices[3 * i + j].texcoord.x = object->mesh.tVerts[tf->t[j]].x;
				m_vertices[3 * i + j].texcoord.y = object->mesh.tVerts[tf->t[j]].y;

				m_vertices[3 * i + j].flag = -1;

				RVertex *rv = object->mesh.getRVertPtr(f->v[j]);
				int nnormals;

				if (rv->rFlags & SPECIFIED_NORMAL) {
					Point3 normal = nm * rv->rn.getNormal();
					m_vertices[3 * i + j].normal.x = normal.x;
					m_vertices[3 * i + j].normal.y = normal.y;
					m_vertices[3 * i + j].normal.z = normal.z;
				}
				else {
					if ((nnormals = rv->rFlags & NORCT_MASK) && f->smGroup) {
						if (nnormals == 1) {
							Point3 normal = nm * rv->rn.getNormal();
							m_vertices[3 * i + j].normal.x = normal.x;
							m_vertices[3 * i + j].normal.y = normal.y;
							m_vertices[3 * i + j].normal.z = normal.z;
						}
						else {
							for (int l = 0; l < nnormals; l++) {
								if (rv->ern[l].getSmGroup() & f->smGroup) {
									Point3 normal = nm * rv->ern[l].getNormal();
									m_vertices[3 * i + j].normal.x = normal.x;
									m_vertices[3 * i + j].normal.y = normal.y;
									m_vertices[3 * i + j].normal.z = normal.z;
								}
							}
						}
					}
					else {
						//Point3 normal = nm * object->mesh.getFaceNormal(i);
						m_vertices[3 * i + j].normal.x = 0;
						m_vertices[3 * i + j].normal.y = 0;
						m_vertices[3 * i + j].normal.z = 0;
						m_vertices[3 * i + j].flag = f->v[j];
					}
				}

				//FINISHED!!
			}
		}

		//-------------------------------------------------
		//Process normals
		//-------------------------------------------------
		for (int i = 0; i < object->mesh.numFaces; i++) {
			Face *f = &object->mesh.faces[i];
			Point3 normal = nm * object->mesh.getFaceNormal(i);

			for (int k = 0; k < object->mesh.numFaces * 3; k++) {
				if (m_vertices[k].flag == f->v[0] || m_vertices[k].flag == f->v[1] || m_vertices[k].flag == f->v[2]) {
					m_vertices[k].normal.x += normal.x;
					m_vertices[k].normal.y += normal.y;
					m_vertices[k].normal.z += normal.z;
				}
			}
		}

		for (int k = 0; k < object->mesh.numFaces * 3; k++) {
			m_vertices[k].normal = m_vertices[k].normal.Normalize();
		}

		//-------------------------------------------------
		//Build indices
		//-------------------------------------------------
		Vertex *vertices = new Vertex[object->mesh.numFaces * 3];
		int *indices = new int[object->mesh.numFaces * 3];

		int numFaces = object->mesh.numFaces;
		int numVertices = 0;

		for (int i = 0; i < numFaces * 3; i++) {
			bool flag = false;
			int index;
			for (int j = 0; j < numVertices; j++) {
				if ((vertices[j].position == m_vertices[i].position) &&
					(vertices[j].texcoord == m_vertices[i].texcoord) &&
					(vertices[j].normal == m_vertices[i].normal)) {
						flag = true;
						index = j;
				}
			}

			if (flag) {
				indices[i] = index;
			}
			else {
				vertices[numVertices].position = m_vertices[i].position;
				vertices[numVertices].texcoord = m_vertices[i].texcoord;
				vertices[numVertices].normal = m_vertices[i].normal;
				indices[i] = numVertices;
				numVertices++;
			}
		}

		//-----------------------------------------------
		//Save subset to file
		//-----------------------------------------------
		_ftprintf(fmdl, _T("subset %s\n"), subsetName.c_str());
		_ftprintf(fmdl, _T("num_vertices %i\n"), numVertices);

		for (int v = 0; v < numVertices; v++)	{
			_ftprintf(fmdl, _T("%f %f %f %f %f %f %f %f\n"),
				vertices[v].position.x - pos.x,
				vertices[v].position.z - pos.z,
				vertices[v].position.y - pos.y,
				vertices[v].normal.x,
				vertices[v].normal.z,
				vertices[v].normal.y,
				vertices[v].texcoord.x,
				vertices[v].texcoord.y);
		}

		_ftprintf(fmdl, _T("num_faces %i\n"), numFaces);

		for (int f = 0; f < numFaces; f++)
		{
			_ftprintf(fmdl, _T("%i %i %i\n"),
				indices[3 * f + 0],
				indices[3 * f + 2],
				indices[3 * f + 1]);
		}
		_ftprintf(fmdl, _T("\n"));

		delete [] m_vertices;
		delete [] vertices;
		delete [] indices;

		//-----------------------------------------------
		//Save materials
		//-----------------------------------------------
		Mtl *m = node->GetMtl();
		if (!m || m->ClassID() != Class_ID(DMTL_CLASS_ID, 0)) {
		}
		else {
			String tex0;
			Texmap *tmap = m->GetSubTexmap(ID_DI);

			if (!tmap || tmap->ClassID() != Class_ID(BMTEX_CLASS_ID, 0)) {
				tex0 = _T("");
			}
			else {
				BitmapTex *bmt = (BitmapTex *) tmap;
				tex0 = _tcslwr((wchar_t *) bmt->GetMapName());
				getFileName(tex0);
				replaceSpaces(tex0);

				FILE *fmtr = _tfopen(String(subsetName + _T(".amtr")).c_str(), _T("wt"));

				_ftprintf(fmtr, _T("pass Ambient\n"));
				_ftprintf(fmtr, _T("{\n"));
				_ftprintf(fmtr, _T("shader = mesh_ambient.ashd\n"));
				_ftprintf(fmtr, _T("mat4 u_mvp = scene::mvp\n"));
				_ftprintf(fmtr, _T("vec3 u_light_color = scene::light_color\n"));
				_ftprintf(fmtr, _T("sampler2D u_base_map = %s\n"), tex0.c_str());
				_ftprintf(fmtr, _T("}\n\n"));

				_ftprintf(fmtr, _T("pass LightOmni\n"));
				_ftprintf(fmtr, _T("{\n"));
				_ftprintf(fmtr, _T("shader = mesh_omni.ashd\n"));
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
				_ftprintf(fmtr, _T("shader = mesh_spot.ashd\n"));
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
				_ftprintf(fmtr, _T("shader = mesh_direct.ashd\n"));
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

		//-----------------------------------------------
		//Save material_list
		//-----------------------------------------------
		_ftprintf(fmtrlst, _T("%s = %s"), subsetName.c_str(), String(subsetName + _T(".amtr \n")).c_str());
	}
	fclose(fmdl);
	fclose(fmtrlst);
};

//---------------------------------------------------------------------------
//CALLBACK
//---------------------------------------------------------------------------
int AstEnumProc::callback(INode *node) {
	nodes.push_back(node);
	return TREE_CONTINUE;
}

//---------------------------------------------------------------------------
//CONVERTS NODE TO TRIOBJ
//---------------------------------------------------------------------------
TriObject *AstEnumProc::NODE2OBJ(INode *node, int &deleteIt) {
	deleteIt = false;
	Object *obj = node->EvalWorldState(iface->GetTime()).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *) obj->ConvertToType(iface->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = true;
		return tri;
	}
	else return NULL;
}

//---------------------------------------------------------------------------
//VegaExport
//---------------------------------------------------------------------------
class VegaExport : public SceneExport {
public:
	int          ExtCount()   { return 1; }
	const TCHAR* Ext(int i)   { if (i == 0) return _T("amdl"); else return _T(""); }
	const TCHAR* LongDesc()   { return _T("AST_engine model exporter"); }
	const TCHAR* ShortDesc()  { return _T("AST_exporter"); }
	const TCHAR* AuthorName() { return _T("Tsyplyaev Alexander"); }
	const TCHAR* CopyrightMessage() { return _T("Copyright (C) 2006"); }
	const TCHAR* OtherMessage1()    { return _T(""); }
	const TCHAR* OtherMessage2()    { return _T(""); }
	unsigned int Version()          { return 0.1; }

	void ShowAbout(HWND hWnd) { MessageBox(hWnd, _T("About"), _T("AST_engine model exporter"), MB_OK); }
	BOOL SupportsOptions(int ext, DWORD options) { return false; }

	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts = FALSE, DWORD options = 0);

	VegaExport() {}
	virtual ~VegaExport() {}
};

//---------------------------------------------------------------------------
//DO EXPORT
//---------------------------------------------------------------------------

int VegaExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts, DWORD options) {
	AstEnumProc astProc(i);

	ei->theScene->EnumTree(&astProc);

	astProc.exportAll(name);
	return 1;
}










//---------------------------------------------------------------------------
//plugin entry
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//plugin entry
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//plugin entry
//---------------------------------------------------------------------------
HINSTANCE hInstance;
int controlsInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) {
	hInstance = hinstDLL;
	if (!controlsInit) {
		controlsInit = TRUE;
		//Nick:del		InitCustomControls(hInstance);
		InitCommonControls();
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//Class description
//---------------------------------------------------------------------------
class ASTClassDesc : public ClassDesc {
public:
	int IsPublic() { return 1; }
	void *Create(BOOL loading = FALSE) { return new VegaExport; }

	const TCHAR *ClassName() { return _T("AST_engine model exporter"); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID()       { return Class_ID(0x1f527516, 0x2fc1118d); }
	const TCHAR *Category()  { return _T(""); }
};

//---------------------------------------------------------------------------
//ASTClassDesc
//---------------------------------------------------------------------------
static ASTClassDesc astEngineDesc;

//---------------------------------------------------------------------------
//Number of plugins
//---------------------------------------------------------------------------
__declspec(dllexport) int LibNumberClasses() {
	return 1;
}

//---------------------------------------------------------------------------
//LibClassDesc
//---------------------------------------------------------------------------
__declspec(dllexport) ClassDesc *LibClassDesc(int i) {
	return (i == 0) ? &astEngineDesc : 0;
}

//---------------------------------------------------------------------------
//LibDesc
//---------------------------------------------------------------------------
__declspec(dllexport) const TCHAR *LibDescription() {
	return _T("AST engine export plugin");
}

//---------------------------------------------------------------------------
//LibVersion
//---------------------------------------------------------------------------
__declspec(dllexport) ULONG LibVersion(void) {
	return VERSION_3DSMAX;
}

//---------------------------------------------------------------------------
//????????????
//---------------------------------------------------------------------------
__declspec(dllexport)ULONG CanAutoDefer() {
	return 1;
}

