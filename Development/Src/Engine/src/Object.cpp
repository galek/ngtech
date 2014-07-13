/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Object.h"
#include "File.h"
#include "Log.h"
#include "Frustum.h"
#include "MathLib.h"
#include "Error.h"
#include "Cache.h"
#include "Scene.h"
//**************************************

namespace NGTech {

	Object::Object() {
		if (GetEngine()->scene)
			GetEngine()->scene->addObject(this);
	}

	ObjectMesh::ObjectMesh(const String &path)
		:Object() {
		if (GetEngine()->cache){
			model = GetEngine()->cache->loadModel("../data/meshes/" + path);
			materials.resize(model->getNumSubsets());
			for (int i = 0; i < materials.size(); i++)
				materials[i] = nullptr;
		}
		transform.identity();
		pBody = nullptr;
		return;
	}

	ObjectMesh::~ObjectMesh() {
		GetEngine()->cache->deleteModel(model);
		for (int i = 0; i < materials.size(); i++) {
			GetEngine()->cache->deleteMaterial(materials[i]);
		}
		materials.clear();
		GetEngine()->cache->deleteSound(impactSound);
		delete pBody;
	}

	void ObjectMesh::drawSubset(int s) {
		model->drawSubset(s);
	}

	Material *ObjectMesh::getMaterial(int s) {
		return materials[s];
	}

	void ObjectMesh::setMaterial(const String &name, const String &path) {
		if (GetEngine()->cache){
			Material *material = GetEngine()->cache->loadMaterial(path);
			if (name == "*")
				for (int s = 0; s < model->getNumSubsets(); s++)
					materials[s] = material;
			materials[model->getSubset(name)] = material;
		}
	}

	void ObjectMesh::setMaterialList(const String &path) {
		FILE *mFile = fopen(String("../data/meshes/" + path).c_str(), "rt");

		//Check if exist
		if (!mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while (!feof(mFile)) {
			String line = FileHelper::readString(mFile);
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
		fclose(mFile);
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

	void ObjectMesh::setPhysicsCylinder(float radius, float height, float mass) {
		pBody = PhysBody::CreateCylinder(radius, height, mass);
		setTransform(transform);
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
		int numPos = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {
			numPos += model->subsets[i]->numVertices;
		}
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {

			for (int v = 0; v < model->subsets[i]->numVertices; v++) {
				pos[k] = model->subsets[i]->vertices[v].position;
				k++;
			}
		}

		pBody = PhysBody::CreateConvexHull(pos, numPos, mass);
		setTransform(transform);

		delete[] pos;
	}

	void ObjectMesh::setPhysicsStaticMesh() {
		int numPos = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {
			numPos += model->subsets[i]->numIndices;
		}
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for (int i = 0; i < model->getNumSubsets(); i++) {

			for (int v = 0; v < model->subsets[i]->numIndices / 3; v++) {
				pos[k * 3 + 0] = model->subsets[i]->vertices[model->subsets[i]->indices[v * 3 + 0]].position;
				pos[k * 3 + 1] = model->subsets[i]->vertices[model->subsets[i]->indices[v * 3 + 1]].position;
				pos[k * 3 + 2] = model->subsets[i]->vertices[model->subsets[i]->indices[v * 3 + 2]].position;
				k++;
			}
		}

		if (pBody)
			pBody = PhysBody::CreateStaticMesh(pos, numPos, true);
		setTransform(transform);

		delete[] pos;
	}

	void ObjectMesh::setImpactSound(const String &path) {
		impactSound = GetEngine()->cache->loadSound("../data/sounds/" + path);
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
			GetEngine()->cache->deleteMaterial(materials[i]);
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
		Material *material = GetEngine()->cache->loadMaterial(path);
		if (name == "*")	{
			for (int s = 0; s < model->getNumSubsets(); s++) {
				materials[s] = material;
			}
		}
		materials[model->getSubset(name)] = material;
	}

	void ObjectSkinnedMesh::setMaterialList(const String &path) {
		FILE *mFile = fopen(String("../data/meshes/" + path).c_str(), "rt");

		//Check if exist
		if (!mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while (!feof(mFile)) {
			String line = FileHelper::readString(mFile);
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
		fclose(mFile);
	}

	void ObjectSkinnedMesh::setTransform(const Mat4 &trans) {
		transform = trans;
	}

	Mat4 ObjectSkinnedMesh::getTransform() {
		return transform;
	}

}