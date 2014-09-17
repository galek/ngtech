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
		:Object() {
		if (GetCache()){
			model = GetCache()->loadModel("../data/meshes/" + path);
			materials.resize(model->getNumSubsets());
			for (int i = 0; i < materials.size(); i++)
				materials[i] = nullptr;
		}
		transform.identity();
		pBody = nullptr;
		return;
	}

	ObjectMesh::~ObjectMesh() {
		GetCache()->deleteModel(model);
		for (int i = 0; i < materials.size(); i++) {
			GetCache()->deleteMaterial(materials[i]);
		}
		materials.clear();
		GetCache()->deleteSound(impactSound);
		delete pBody;
	}

	void ObjectMesh::drawSubset(int s) {
		model->drawSubset(s);
	}

	Material *ObjectMesh::getMaterial(int s) {
		return materials[s];
	}

	void ObjectMesh::setMaterial(const String &name, const String &path) {
		if (GetCache()){
			Material *material = GetCache()->loadMaterial(path);
			if (name == "*")
				for (int s = 0; s < model->getNumSubsets(); s++)
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

		while (!mFile.EndOfFile()) {
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

	void ObjectMesh::setPhysicsCone(float radius, float height, float mass) {
		pBody = PhysBody::CreateCone(radius, height, mass);
		setTransform(transform);
	}

	void ObjectMesh::setPhysicsCapsule(float radius, float height, float mass)  {
		pBody = PhysBody::CreateCapsule(radius, height, &transform, mass);
	}

	void ObjectMesh::setPhysicsChampferCylinder(float radius, float height, float mass) {
		pBody = PhysBody::CreateChampferCylinder(radius, height, mass);
		setTransform(transform);
	}

	void ObjectMesh::setPhysicsConvexHull(float mass) {
		int numVertices = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {
			numVertices += model->subsets[i]->numVertices;
		}

		PhysBody*pb = new PhysBody[model->getNumSubsets()];
		for (int i = 0; i < model->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateConvexHull(model->subsets[i]->numVertices, model->subsets[i]->numIndices, &transform, model->subsets[i]->vertices, model->subsets[i]->indices, mass);

		pBody = pb;
		setTransform(transform);
	}

	void ObjectMesh::setPhysicsStaticMesh() {
		int numIndices = 0;
		int numVertices = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {
			numIndices += model->subsets[i]->numIndices;
			numVertices += model->subsets[i]->numVertices;
		}

		PhysBody*pb = new PhysBody[model->getNumSubsets()];

		for (int i = 0; i < model->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateStaticMesh(model->subsets[i]->numVertices, model->subsets[i]->numIndices, &transform, model->subsets[i]->vertices, model->subsets[i]->indices);


		//Nick:BUG:Меш собирается в реалтайме,и часть тел пролетает,если сначала создать физику,и потом задать коллизию
		pBody = pb;
		setTransform(transform);
	}

	void ObjectMesh::setImpactSound(const String &path) {
		impactSound = GetCache()->loadSound(path);
		if (pBody)
			pBody->SetImpactSound(impactSound);
	}

	ObjectSkinnedMesh::ObjectSkinnedMesh(const String &path) {
		model = new SkinnedModel("../data/meshes/" + path);
		materials.resize(model->getNumSubsets());
		for (int i = 0; i < materials.size(); i++)
			materials[i] = nullptr;
		transform.identity();
		return;
	}

	ObjectSkinnedMesh::~ObjectSkinnedMesh() {
		delete model;
		for (int i = 0; i < materials.size(); i++) {
			GetCache()->deleteMaterial(materials[i]);
		}
		materials.clear();
	}

	void ObjectSkinnedMesh::drawSubset(int s) {
		model->drawSubset(s);
	}

	Material *ObjectSkinnedMesh::getMaterial(int s) {
		return materials[s];
	}

	void ObjectSkinnedMesh::setMaterial(const String &name, const String &path) {
		Material *material = GetCache()->loadMaterial(path);
		if (name == "*")	{
			for (int s = 0; s < model->getNumSubsets(); s++) {
				materials[s] = material;
			}
		}
		materials[model->getSubset(name)] = material;
	}

	void ObjectSkinnedMesh::setMaterialList(const String &path) {
		VFile mFile(path.c_str(), VFile::READ_TEXT);

		//Check if exist
		if (!&mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while (!mFile.EndOfFile()) {
			String line = mFile.GetLine();
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
	}

	void ObjectSkinnedMesh::setTransform(const Mat4 &trans) {
		transform = trans;
	}

	Mat4 ObjectSkinnedMesh::getTransform() {
		return transform;
	}

}