#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Object.h"
#include "FileHelper.h"
#include "Log.h"
#include "Frustum.h"
#include "MathLib.h"
#include "Error.h"
#include "Cache.h"
#include "Scene.h"
//**************************************
#include "SkinnedMesh.h"
//**************************************

namespace NGTech {

	ObjectSkinnedMesh::ObjectSkinnedMesh(const String &path)
		:Object()
	{
		mesh = new SkinnedMesh("meshes/" + path);
		materials = new Material*[mesh->GetNumSubsets()];
		transform.Identity();

	}

	ObjectSkinnedMesh::~ObjectSkinnedMesh() {
		for (size_t i = 0; i < getNumSubsets(); i++)
		{
			GetCache()->deleteMaterial(materials[i]);
		}

		SAFE_DELETE(mesh);

	}

	void ObjectSkinnedMesh::drawSubset(size_t subset) {
		//Warning(__FUNCTION__);
		//BBox bbox = GetTransformedBBox(subset);
		//BSphere bsphere = GetTransformedBSphere(subset);

		Camera *camera = GetScene()->GetCurrentCamera();
		//LightData *light = GetScene()->GetCurrentLight();

		////frustum visibility
		//if (camera)
		//{
		//	if (!camera->GetFrustum()->IsBSphereInside(bsphere))
		//		return;
		//	if (!camera->GetFrustum()->IsBBoxInside(bbox))
		//		return;
		//}

		////light visibility
		//if (light)
		//{
		//	BSphere lightSphere = BSphere(light->GetPosition(), light->GetRadius());
		//	if (!lightSphere.IntersectsSphere(bsphere))
		//		return;
		//}
		//draw
		this->mesh->drawSubset(subset);
	}

	void ObjectSkinnedMesh::setMaterial(const String &path, const String &name) {
		if (GetCache()){
			Material *material = GetCache()->loadMaterial(path);
			if (name == "*")
				for (size_t s = 0; s < mesh->GetNumSubsets(); s++)
					materials[s] = material;
			materials[mesh->GetSubset(name)] = material;
		}
	}

	//void ObjectSkinnedMesh::SetMaterialList(const String &path) {
	//	VFile mFile(path.c_str(), VFile::READ_TEXT);

	//	//Check if exist
	//	if (!&mFile) {
	//		Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
	//		return;
	//	}

	//	while (!mFile.EndOfFile()) {
	//		String line = mFile.GetLine();
	//		SetMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
	//	}
	//}

	void ObjectSkinnedMesh::setTransform(const Mat4 &trans) {
		transform = trans;
	}

	Mat4 ObjectSkinnedMesh::getTransform() {
		//Warning(__FUNCTION__);
		return transform;
	}

	/**
	*/
	size_t ObjectSkinnedMesh::getNumSubsets()
	{
		return mesh->GetNumSubsets();
	}

	/**
	*/
	const BBox &ObjectSkinnedMesh::getBBox(size_t s)
	{
		return mesh->GetBBox(s);
	}

	/**
	*/
	const BSphere &ObjectSkinnedMesh::getBSphere(size_t s)
	{
		return mesh->GetBSphere(s);
	}

	/**
	*/
	Material *ObjectSkinnedMesh::getMaterial(size_t subset)
	{
		return materials[subset];
	}

	/**
	*/
	/*void ObjectSkinnedMesh::SetMaterialList(const String &path)
	{
	MaterialList *list = new MaterialList(path);

	for (int i = 0; i < list->GetNumMaterials(); i++)
	{
	String material = list->GetMaterial(i);

	for (int j = 0; j < list->GetNumSubsets(i); j++)
	{
	String subset = list->GetSubset(i, j);

	SetMaterial(material, subset);
	}
	}

	SAFE_DELETE(list);
	}*/

	/**
	*/
	void ObjectSkinnedMesh::setAnimationFrame(int frame, int from, int to)
	{
		mesh->SetFrame(frame, from, to);
	}
}