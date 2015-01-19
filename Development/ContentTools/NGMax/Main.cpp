#include <max.h>
#include <stdmat.h>
#include <vector>

#include "String.h"
#include "File.h"
#include "xmlParser/xmlParser.h"

#pragma comment(lib, "MaxCore.lib") 
#pragma comment(lib, "maxutil.lib")
#pragma comment(lib, "geom.lib")
#pragma comment(lib, "mesh.lib")
#pragma comment(lib, "comctl32.lib")

/*
String
*/

//name buffer
#define NAME_SIZE 64
char nameBuffer[NAME_SIZE];

//write name to file
void WriteName(const String &name, File *file)
{
	memset(nameBuffer, 0, NAME_SIZE);
	strcpy(nameBuffer, name.c_str());
	file->Write(nameBuffer, NAME_SIZE, 1);
}

//replaces ' ' with '_'
void ReplaceSpaces(String &str)
{
	char buffer[1024];
	strcpy(buffer, str.c_str());

	int i = 0;
	while (i < strlen(buffer))
	{
		if (buffer[i] == ' ')
			buffer[i] = '_';
		i++;
	}

	str.assign(buffer);
}


/*
Mesh
*/

//header
#define MESH_HEADER ('x' | 's' << 8 | 'm' << 16 | 's' << 24)
#define MESH_HEADER_UNWRAPPED ('x' | 's' << 8 | 'm' << 16 | 'u' << 24)

//
Matrix3 zySwap = Matrix3(Point3(1, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0), Point3(0, 0, 0));

//vertex class
struct Vertex
{
	Point3 position;
	Point3 normal;
	Point2 texcoord;
	Point2 unwrappedTexcoord;
};

//subset class
struct Subset
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	String name;

	INode *node;
	TriObject *object;

	Point3 min;
	Point3 max;
};

//material class
struct MaterialData
{
	String name;
	String texture;

	std::vector<String> subsets;
};

/**
NGmax ENUM PROC
*/
class NGEnumProc : public ITreeEnumProc
{
public:
	NGEnumProc(Interface *i)
	{
		iface = i;
	}

	/*
	*/
	int callback(INode *node);
	void Export(const char *name);
	void ReadModel();
	void SaveXSMSH(const String &name);
	void SaveMaterials();
	void SaveMaterialList(const String &name);

	/*
	*/
	TriObject *NODE2OBJ(INode *node, int &deleteIt);

	Interface *iface;
	bool selected;

	std::vector<INode*> nodes;
	std::vector<Subset> subsets;
	std::vector<MaterialData> materials;
	MaterialData withoutMaterials;
	Point3 pos;

	bool unwrappedTexcoord;
};

/*
*/
void NGEnumProc::Export(const char *name)
{
	ReadModel();
	SaveXSMSH(name);
	SaveMaterials();
	SaveMaterialList(name);
}

/*
*/
int NGEnumProc::callback(INode *node)
{
	if (selected && node->Selected() == FALSE)
	{
		return TREE_CONTINUE;
	}
	nodes.push_back(node);
	return TREE_CONTINUE;
}

/*
*/
void NGEnumProc::ReadModel()
{
	//Process model
	for (int n = 0; n < nodes.size(); n++)
	{
		Subset subset;
		subset.node = nodes[n];;

		int d;
		subset.object = NODE2OBJ(subset.node, d);
		if (!subset.object)
			continue;
		//name
		subset.name = subset.node->GetName();

		//process geometry
		Matrix3 tm = subset.node->GetObjTMAfterWSM(iface->GetTime());
		Matrix3 nm = tm;
		nm.NoScale();
		nm.NoTrans();

		//get bBox
		Box3 box;
		subset.object->mesh.buildBoundingBox();
		box = subset.object->mesh.getBoundingBox(&tm);
		subset.min = box.Min();
		subset.max = box.Max();

		//build normals
		subset.object->mesh.buildNormals();

		//additional texcoord
		int mp = -1;
		for (int i = 2; i < MAX_MESHMAPS - 1; i++)
		{
			if (subset.object->mesh.mapSupport(i))
			{
				mp = i;
				break;
			}
		}
		if (mp > 0)
			unwrappedTexcoord = true;

		for (int i = 0; i < subset.object->mesh.numFaces; i++)
		{
			Face *f = &subset.object->mesh.faces[i];

			TVFace *tfu = NULL;
			/*TVFace *tf = &subset.object->mesh.tvFace[i];*///Nick:Crash

			if (unwrappedTexcoord)
			{
				tfu = &subset.object->mesh.mapFaces(mp)[i];
			}
			//цикл по индексам грани
			for (int j = 0; j < 3; j++)
			{
				Vertex vertex;

				Point3 v = tm * subset.object->mesh.verts[f->v[j]];

				vertex.position.x = v.x;
				vertex.position.y = v.y;
				vertex.position.z = v.z;


				if (unwrappedTexcoord)
				{
					vertex.unwrappedTexcoord.x = subset.object->mesh.mapVerts(mp)[tfu->t[j]].x;
					vertex.unwrappedTexcoord.y = subset.object->mesh.mapVerts(mp)[tfu->t[j]].y;
				}
				//else //Nick:Crash
				//{			
				//	if (tf)
				//	{
				//		vertex.texcoord.x = subset.object->mesh.tVerts[tf->t[j]].x;
				//		vertex.texcoord.y = subset.object->mesh.tVerts[tf->t[j]].y;
				//	}
				//}

				//read normal
				bool specifiedNormal = true;

				RVertex *rv = subset.object->mesh.getRVertPtr(f->v[j]);
				int nnormals;

				if (rv->rFlags & SPECIFIED_NORMAL)
				{
					Point3 normal = nm * rv->rn.getNormal();
					vertex.normal.x = normal.x;
					vertex.normal.y = normal.y;
					vertex.normal.z = normal.z;
				}
				else
				{
					if ((nnormals = rv->rFlags & NORCT_MASK) && f->smGroup)
					{
						if (nnormals == 1)
						{
							Point3 normal = nm * rv->rn.getNormal();
							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;
						}
						else
						{
							for (int l = 0; l < nnormals; l++)
							{
								if (rv->ern[l].getSmGroup() & f->smGroup)
								{
									Point3 normal = nm * rv->ern[l].getNormal();
									vertex.normal.x = normal.x;
									vertex.normal.y = normal.y;
									vertex.normal.z = normal.z;
								}
							}
						}
					}
					else
					{
						specifiedNormal = false;
						Point3 normal = nm * subset.object->mesh.getFaceNormal(i);
						vertex.normal.x = normal.x;
						vertex.normal.y = normal.y;
						vertex.normal.z = normal.z;
					}
				}

				//make indices
				int index = -1;
				for (int k = 0; k < subset.vertices.size(); k++)
				{
					if ((subset.vertices[k].position == vertex.position) &&
						(subset.vertices[k].texcoord == vertex.texcoord))
					{
						if (unwrappedTexcoord && subset.vertices[k].unwrappedTexcoord == vertex.unwrappedTexcoord)
						{
							if (specifiedNormal)
							{
								if (subset.vertices[k].normal == vertex.normal)
									index = k;
							}
							else
							{
								index = k;
								subset.vertices[k].normal += vertex.normal;
							}
						}

						if (!unwrappedTexcoord)
						{
							if (specifiedNormal)
							{
								if (subset.vertices[k].normal == vertex.normal)
									index = k;
							}
							else
							{
								index = k;
								subset.vertices[k].normal += vertex.normal;
							}
						}
					}
				}

				if (index > -1)
				{
					subset.indices.push_back(index);
				}
				else
				{
					subset.indices.push_back(subset.vertices.size());
					subset.vertices.push_back(vertex);
				}
			}
		}

		//normalize normals
		for (int k = 0; k < subset.vertices.size(); k++)
		{
			subset.vertices[k].normal = subset.vertices[k].normal.Normalize();
		}

		subsets.push_back(subset);
	}

	//Get position
	Point3 smin = subsets[0].min;
	Point3 smax = subsets[0].max;

	for (int s = 1; s < subsets.size(); s++)
	{
		smin.x = min(smin.x, subsets[s].min.x);
		smin.y = min(smin.y, subsets[s].min.y);
		smin.z = min(smin.z, subsets[s].min.z);

		smax.x = max(smax.x, subsets[s].max.x);
		smax.y = max(smax.y, subsets[s].max.y);
		smax.z = max(smax.z, subsets[s].max.z);
	}

	this->pos = (smin + smax);
	this->pos.x = this->pos.x * 0.5;
	this->pos.y = this->pos.y * 0.5;
	this->pos.z = this->pos.z * 0.5;
}

/*
*/
void NGEnumProc::SaveXSMSH(const String& name)
{
	//Save mesh
	File *file = new File(name, File::WRITE_BINARY);

	//header
	unsigned int header;
	if (unwrappedTexcoord)
		header = MESH_HEADER_UNWRAPPED;
	else
		header = MESH_HEADER;

	file->Write(&header, sizeof(unsigned int), 1);

	//num_subsets
	unsigned int numSubsets = subsets.size();
	file->Write(&numSubsets, sizeof(unsigned int), 1);

	//subsets
	for (int s = 0; s < subsets.size(); s++)
	{
		Subset *st = &subsets[s];
		WriteName(st->name, file);

		//num vertices
		unsigned int numVertices = st->vertices.size();
		file->Write(&numVertices, sizeof(unsigned int), 1);

		//process vertices
		for (unsigned int v = 0; v < st->vertices.size(); v++)
		{
			Point3 vpositon = (st->vertices[v].position - pos) * zySwap;
			Point3 vnormal = (st->vertices[v].normal) * zySwap;

			file->Write(&st->vertices[v].position.x, sizeof(float), 1);
			file->Write(&st->vertices[v].position.z, sizeof(float), 1);
			file->Write(&st->vertices[v].position.y, sizeof(float), 1);
			file->Write(&st->vertices[v].normal.x, sizeof(float), 1);
			file->Write(&st->vertices[v].normal.z, sizeof(float), 1);
			file->Write(&st->vertices[v].normal.y, sizeof(float), 1);
			file->Write(&st->vertices[v].texcoord.x, sizeof(float), 1);
			file->Write(&st->vertices[v].texcoord.y, sizeof(float), 1);

			if (unwrappedTexcoord)
			{
				file->Write(&st->vertices[v].unwrappedTexcoord.x, sizeof(float), 1);
				file->Write(&st->vertices[v].unwrappedTexcoord.y, sizeof(float), 1);
			}
		}

		//number of indices
		unsigned int numIndices = st->indices.size();
		file->Write(&numIndices, sizeof(unsigned int), 1);

		//process indices
		for (unsigned int i = 0; i < numIndices / 3; i++)
		{
			file->Write(&st->indices[i * 3 + 0], sizeof(unsigned int), 1);
			file->Write(&st->indices[i * 3 + 2], sizeof(unsigned int), 1);
			file->Write(&st->indices[i * 3 + 1], sizeof(unsigned int), 1);
		}
	}

	delete file;
}

/*
*/
void NGEnumProc::SaveMaterials()
{
	//materials
	for (int s = 0; s < subsets.size(); s++)
	{
		Subset *st = &subsets[s];

		//Save material
		Mtl *m = st->node->GetMtl();

		//
		if (m && m->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			Texmap *tmap = m->GetSubTexmap(ID_DI);

			//
			if (tmap && tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			{
				BitmapTex *bmt = (BitmapTex*)tmap;

				String textureName;
				textureName = strlwr((char *)bmt->GetMapName());
				ReplaceSpaces(textureName);

				String textureFile = "";

				for (int i = textureName.length() - 1; i >= 0; i--)
				{
					char c = textureName.data()[i];
					if (c == '/' || c == '\\')
						break;
					textureFile = c + textureFile;
				}

				String materialName = textureFile.CutFileExt() + ".mat";

				//find existing
				bool found = false;
				for (int k = 0; k < materials.size(); k++)
				{
					if (materials[k].name == materialName)
					{
						materials[k].subsets.push_back(subsets[s].name);
						found = true;
					}
				}

				if (!found)
				{
					MaterialData material;
					material.name = materialName;
					material.subsets.push_back(subsets[s].name);
					materials.push_back(material);

					/*XMLNode materialNode = XMLNode::createXMLTopNode(L"Material");

					XMLNode effectNode = materialNode.addChild(L"Effect");
					effectNode.addAttribute(L"file", L"mesh.xsfx");

					XMLNode baseMapNode = effectNode.addChild("Texture");
					baseMapNode.addAttribute(L"type", L"2D");
					baseMapNode.addAttribute(L"name", L"baseMap");
					baseMapNode.addAttribute(L"file", textureFile.c_str());

					XMLNode bumpMapNode = effectNode.addChild(L"Texture");
					bumpMapNode.addAttribute(L"type", L"2D");
					bumpMapNode.addAttribute(L"name", L"bumpMap");
					bumpMapNode.addAttribute(L"file", textureFile.c_str());
					bumpMapNode.addAttribute(L"bumpMap", L"1");

					XMLNode paralaxNode = effectNode.addChild("Float");
					paralaxNode.addAttribute("name", "paralaxCoef");
					paralaxNode.addAttribute("value", "6");

					XMLNode specularNode = effectNode.addChild("Float");
					specularNode.addAttribute("name", "specularCoef");
					specularNode.addAttribute("value", "0.5");

					XMLNode shininessNode = effectNode.addChild("Float");
					shininessNode.addAttribute("name", "shininess");
					shininessNode.addAttribute("value", "16");

					materialNode.writeToFile(materialName.c_str());*/
				}
			}
			else
			{
				withoutMaterials.subsets.push_back(subsets[s].name);
			}
		}
		else
		{
			withoutMaterials.subsets.push_back(subsets[s].name);
		}
	}
}

/*
*/
void NGEnumProc::SaveMaterialList(const String &name)
{
	//String path = name;
	//path = path.CutFileExt();
	//path = path + ".xsmtrlst";

	//XMLNode listNode = XMLNode::createXMLTopNode("MaterialList");
	//for (int i = 0; i < materials.size(); i++)
	//{
	//	String mfile = "data/materials/" + materials[i].name;
	//	XMLNode materialNode = listNode.addChild("Material");
	//	materialNode.addAttribute("file", mfile.c_str());

	//	for (int k = 0; k < materials[i].subsets.size(); k++)
	//	{
	//		XMLNode subsetNode = materialNode.addChild("Subset");
	//		subsetNode.addAttribute("name", materials[i].subsets[k].c_str());
	//	}
	//}

	////no materials
	//XMLNode materialNode = listNode.addChild("NoMaterial");

	//for (int k = 0; k < withoutMaterials.subsets.size(); k++)
	//{
	//	XMLNode subsetNode = materialNode.addChild("Subset");
	//	subsetNode.addAttribute("name", withoutMaterials.subsets[k].c_str());
	//}

	//listNode.writeToFile(path.c_str());
}

/*
	Convert node to triobj
	*/
TriObject *NGEnumProc::NODE2OBJ(INode *node, int &deleteIt)
{
	deleteIt = false;
	Object *obj = node->EvalWorldState(iface->GetTime()).obj;

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *)obj->ConvertToType(iface->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = true;
		return tri;
	}
	else
	{
		return NULL;
	}
}


/*
NGMax
*/
class NGMax : public SceneExport
{
public:
	/*
	*/
	NGMax(){}

	/*
	*/
	virtual ~NGMax()
	{
	}

	/*
	*/
	int ExtCount()
	{
		return 1;
	}

	/*
	*/
	const TCHAR* Ext(int i)
	{
		if (i == 0)
			return _T("nggf");
		else
			return _T("");
	}

	/*
	*/
	const TCHAR* LongDesc()
	{
		return _T("NGMax mesh exporter");
	}

	/*
	*/
	const TCHAR* ShortDesc()
	{
		return _T("NGMax");
	}

	/*
	*/
	const TCHAR* AuthorName()
	{
		return _T("NG Games");
	}

	/*
	*/
	const TCHAR* CopyrightMessage()
	{
		return _T("Copyright (C) 2006-2015");
	}

	/*
	*/
	const TCHAR* OtherMessage1()
	{
		return _T("");
	}

	/*
	*/
	const TCHAR* OtherMessage2()
	{
		return _T("");
	}

	/*
	*/
	unsigned int Version()
	{
		return 0.1;
	}

	/*
	*/
	void ShowAbout(HWND hWnd)
	{
		MessageBoxA(hWnd, "About", "NGTech Engine mesh exporter", MB_OK);
	}

	/*
	*/
	BOOL SupportsOptions(int ext, DWORD options)
	{
		return (options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
	}

	/*
	*/
	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts = FALSE, DWORD options = 0);
};

std::string getstring(const wchar_t* wstr)
{
	std::wstring ws(wstr);
	std::string str(ws.begin(), ws.end());
	return str;
}

/*
*/
int NGMax::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPromts, DWORD options)
{
	NGEnumProc xProc(i);
	xProc.selected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	ei->theScene->EnumTree(&xProc);
#ifdef _UNICODE
	auto str = getstring(name);
	xProc.Export(str.c_str());
#else
	xProc.Export(name);
#endif

	MessageBoxA(0, "Mesh was succesfully exported!", "Succes!", MB_OK);
	return 1;
}



/*
Plugin entry
*/
HINSTANCE hInstance;
int controlsInit = FALSE;

/*
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
	hInstance = hinstDLL;
	if (!controlsInit)
	{
		controlsInit = TRUE;
		InitCommonControls();
	}
	return TRUE;
}



/*
Class description
*/
class NGMaxClassDesc : public ClassDesc
{
public:
	/*
	*/
	int IsPublic()
	{
		return 1;
	}

	/*
	*/
	void *Create(BOOL loading = FALSE)
	{
		return new NGMax;
	}

	/*
	*/
	const TCHAR *ClassName()
	{
		return _T("NGMax exporter");
	}

	/*
	*/
	SClass_ID SuperClassID()
	{
		return SCENE_EXPORT_CLASS_ID;
	}

	/*
	*/
	Class_ID ClassID()
	{
		return Class_ID(0x1f527516, 0x2fc1118d);
	}

	/*
	*/
	const TCHAR *Category()
	{
		return _T("");
	}
};

/*
NGMaxClassDesc
*/
static NGMaxClassDesc _gNGMaxDesc;
/*
Number of plugins
*/
__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

/*
LibClassDesc
*/
__declspec(dllexport) ClassDesc *LibClassDesc(int i)
{
	return (i == 0) ? &_gNGMaxDesc : 0;
}

/*
LibDesc
*/
__declspec(dllexport) const TCHAR *LibDescription()
{
	return _T("NGMax export plugin");
}

/*
LibVersion
*/
__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

/*
*/
__declspec(dllexport) ULONG CanAutoDefer()
{
	return 1;
}