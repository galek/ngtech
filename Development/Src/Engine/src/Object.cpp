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

namespace NGTech {

	Object::Object() {
		if (GetScene())
			GetScene()->addObject(this);
	}

	ObjectMesh::ObjectMesh(const String &path)
		:Object(), pBody(nullptr), bMultipart(false)
	{
		auto cache = GetCache();
		if (cache){
			model = cache->loadModel("meshes/" + path);
			materials.resize(model->getNumSubsets());
			for (size_t i = 0; i < materials.size(); i++)
				materials[i] = nullptr;
		}
		transform.Identity();
	}

	ObjectMesh::~ObjectMesh() {
		Warning(__FUNCTION__);
		auto cache = GetCache();
		if (cache){
			cache->deleteModel(model);
			for (size_t i = 0; i < materials.size(); i++) {
				cache->deleteMaterial(materials[i]);
			}
			materials.clear();
			cache->deleteSound(impactSound);
		}

		if (GetScene())
			GetScene()->deleteObjectMesh(this);

		if (bMultipart) {
			SAFE_DELETE_ARRAY(pBody);
		}
		else
			SAFE_DELETE(pBody);
	}

	void ObjectMesh::drawSubset(size_t s) {
		model->drawSubset(s);
	}

	Material *ObjectMesh::getMaterial(size_t s) {
		return materials[s];
	}

	void ObjectMesh::setMaterial(const String &path, const String &name) {
		auto cache = GetCache();
		if (cache)
		{
			Material *material = cache->loadMaterial(path);
			if (name == "*")
				for (size_t s = 0; s < model->getNumSubsets(); s++)
					materials[s] = material;
			materials[model->getSubset(name)] = material;
		}
	}

	void ObjectMesh::setMaterialList(const String &path) {
		VFile mFile(path.c_str(), VFile::READ_TEXT);

		//Check if exist
		if (!&mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while (!mFile.IsEof()) {
			String line = mFile.GetLine();
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
	}

	void ObjectMesh::setTransform(const Mat4 &trans) {
		if (pBody)
			pBody->SetTransform(trans);
		else
			transform = trans;
	}

	Mat4 ObjectMesh::getTransform() {
		if (pBody == NULL)
			return transform;
		else
			return pBody->GetTransform();
	}

	void ObjectMesh::setPhysicsBox(const Vec3 &size, float mass) {
		pBody = PhysBody::CreateBox(size, &transform, mass);
	}

	void ObjectMesh::setPhysicsSphere(const Vec3 &size, float mass) {
		pBody = PhysBody::CreateSphere(size.y, &transform, mass);//Nick:TODO:заменить на getRadius
	}

	void ObjectMesh::setPhysicsCylinder(float radius, float width, float mass) {
		pBody = PhysBody::CreateCylinder(radius, width, &transform, mass);
	}

	void ObjectMesh::setPhysicsCapsule(float radius, float height, float mass)  {
		pBody = PhysBody::CreateCapsule(radius, height, &transform, mass);
	}

	void ObjectMesh::setPhysicsConvexHull(float mass) {
		int numVertices = 0;
		for (size_t i = 0; i < model->getNumSubsets(); i++) {
			numVertices += model->subsets[i]->numVertices;
		}

		PhysBody*pb = new PhysBody[model->getNumSubsets()];
		for (size_t i = 0; i < model->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateConvexHull(model->subsets[i]->numVertices, model->subsets[i]->numIndices, &transform, model->subsets[i]->vertices, model->subsets[i]->indices, mass);

		pBody = pb;
		bMultipart = true;
	}

	void ObjectMesh::setPhysicsStaticMesh() {
		int numIndices = 0;
		int numVertices = 0;
		for (size_t i = 0; i < model->getNumSubsets(); i++) {
			numIndices += model->subsets[i]->numIndices;
			numVertices += model->subsets[i]->numVertices;
		}

		auto pb = new PhysBody[model->getNumSubsets()];

		for (size_t i = 0; i < model->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateStaticMesh(model->subsets[i]->numVertices, model->subsets[i]->numIndices, &transform, model->subsets[i]->vertices, model->subsets[i]->indices);

		//Nick:BUG:Меш собирается в реалтайме,и часть тел пролетает,если сначала создать физику,и потом задать коллизию
		pBody = pb;
		bMultipart = true;
	}

	void ObjectMesh::setPhysicsCloth() {

	}

	void ObjectMesh::setImpactSound(const String &path) {
		auto cache = GetCache();
		if (cache)
		{
			impactSound = cache->loadSound(path);
			if (pBody)
				pBody->SetImpactSound(impactSound);
		}
	}
}