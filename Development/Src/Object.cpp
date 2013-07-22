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
#include "Cash.h"
#include "Scene.h"
//**************************************

namespace VEGA {

	//**************************************************************************
	//Object
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    Base Object 
	//Params:  
	//Returns: -
	//---------------------------------------------------------------------------
	Object::Object() {
		if(engine.scene)
			engine.scene->addObject(this);
	}
	//**************************************************************************
	//ObjectMesh
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new ObjectMesh from file
	//Params:  path - object file path
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectMesh::ObjectMesh(const String &path)
		:Object() {
			if(engine.cash){
				model = engine.cash->loadModel("data/meshes/" + path);
				materials.resize(model->getNumSubsets());
				for(int i = 0; i < materials.size(); i++) 
					materials[i] = nullptr;
			}
			transform.identity();
			pBody = nullptr;
			return;
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys object
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectMesh::~ObjectMesh() {
		engine.cash->deleteModel(model);
		for(int i = 0; i < materials.size(); i++) {
			engine.cash->deleteMaterial(materials[i]);
		}
		materials.clear();
		engine.cash->deleteSound(impactSound);
		delete pBody;
	}

	//---------------------------------------------------------------------------
	//Desc:    draws object subset
	//Params:  s - surface number
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::drawSubset(int s) {
		model->drawSubset(s);
	}

	//---------------------------------------------------------------------------
	//Desc:    get subset material by number
	//Params:  s - subset number
	//Returns: subset`s material
	//---------------------------------------------------------------------------
	Material *ObjectMesh::getMaterial(int s) {
		return materials[s];
	}

	//---------------------------------------------------------------------------
	//Desc:    set material to the subset
	//Params:  name - subset name, path - material file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setMaterial(const String &name, const String &path) {
		if(engine.cash){
			Material *material = engine.cash->loadMaterial(path);
			if(name == "*")	
				for(int s = 0; s < model->getNumSubsets(); s++) 
					materials[s] = material;
			materials[model->getSubset(name)] = material;
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    set materials to the subsets
	//Params:  path - material list file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setMaterialList(const String &path) {
		FILE *mFile = fopen(String("data/meshes/" + path).c_str(), "rt");

		//Check if exist
		if(!mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while(!feof(mFile)) {
			String line = FileHelper::readString(mFile);
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
		fclose(mFile);
	}



	//---------------------------------------------------------------------------
	//Desc:    sets object transform
	//Params:  trans - transform matrix
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setTransform(const Mat4 &trans) {
		if(pBody == NULL) {
			transform = trans;
		} else {
			pBody->setTransform(trans);
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    gets object transform
	//Params:  -
	//Returns: object transformation matrix
	//---------------------------------------------------------------------------
	Mat4 ObjectMesh::getTransform() {
		if(pBody == NULL)
			return transform;
		else 
			return pBody->getTransform();
	}

	//---------------------------------------------------------------------------
	//Desc:    sets box object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsBox(const Vec3 &size, float mass) {
		pBody = PhysBody::createBox(size, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets sphere object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsSphere(const Vec3 &size, float mass) {
		pBody = PhysBody::createSphere(size, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets cylinder object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCylinder(float radius, float height, float mass) {
		pBody = PhysBody::createCylinder(radius, height, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets cone object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCone(float radius, float height, float mass) {
		pBody = PhysBody::createCone(radius, height, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets capsule object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCapsule(float radius, float height, float mass)  {
		pBody = PhysBody::createCapsule(radius, height, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets champfer cylinder object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsChampferCylinder(float radius, float height, float mass) {
		pBody = PhysBody::createChampferCylinder(radius, height, mass);
		setTransform(transform);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets convex hull object collider with infinite mass (static)
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsConvexHull(float mass) {
		int numPos = 0;
		for(int i = 0; i < model->getNumSubsets(); i++) {
			numPos += model->subsets[i]->numVertices;
		}
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for(int i = 0; i < model->getNumSubsets(); i++) {

			for(int v = 0; v < model->subsets[i]->numVertices; v++) {
				pos[k] = model->subsets[i]->vertices[v].position;
				k++;
			}
		}

		pBody = PhysBody::createConvexHull(pos, numPos, mass);
		setTransform(transform);

		delete [] pos;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets static object collider
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsStaticMesh() {
		int numPos = 0;
		for(int i = 0; i < model->getNumSubsets(); i++) {
			numPos += model->subsets[i]->numIndices;
		}
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for(int i = 0; i < model->getNumSubsets(); i++) {

			for(int v = 0; v < model->subsets[i]->numIndices/3; v++) {
				pos[k*3+0] = model->subsets[i]->vertices[model->subsets[i]->indices[v*3+0]].position;
				pos[k*3+1] = model->subsets[i]->vertices[model->subsets[i]->indices[v*3+1]].position;
				pos[k*3+2] = model->subsets[i]->vertices[model->subsets[i]->indices[v*3+2]].position;
				k++;
			}
		}

		pBody = PhysBody::createStaticObjectMesh(pos, numPos, true);
		setTransform(transform);

		delete [] pos;
	}

	void ObjectMesh::setImpactSound(const String &path) {
		impactSound = engine.cash->loadSound("data/sounds/" + path);
		pBody->setImpactSound(impactSound);
	}







	//**************************************************************************
	//ObjectSkinnedMesh
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new ObjectMesh from file
	//Params:  path - object file path
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectSkinnedMesh::ObjectSkinnedMesh(const String &path) {
		model = new SkinnedModel("data/meshes/" + path);
		materials.resize(model->getNumSubsets());
		for(int i = 0; i < materials.size(); i++) 
			materials[i] = nullptr;
		transform.identity();
		return;
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys object
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectSkinnedMesh::~ObjectSkinnedMesh() {
		delete model;
		for(int i = 0; i < materials.size(); i++) {
			engine.cash->deleteMaterial(materials[i]);
		}
		materials.clear();
	}

	//---------------------------------------------------------------------------
	//Desc:    draws object subset
	//Params:  s - surface number
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectSkinnedMesh::drawSubset(int s) {
		model->drawSubset(s);
	}

	//---------------------------------------------------------------------------
	//Desc:    get subset material by number
	//Params:  s - subset number
	//Returns: subset`s material
	//---------------------------------------------------------------------------
	Material *ObjectSkinnedMesh::getMaterial(int s) {
		return materials[s];
	}

	//---------------------------------------------------------------------------
	//Desc:    set material to the subset
	//Params:  name - subset name, path - material file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectSkinnedMesh::setMaterial(const String &name, const String &path) {
		Material *material = engine.cash->loadMaterial(path);
		if(name == "*")	{
			for(int s = 0; s < model->getNumSubsets(); s++) {
				materials[s] = material;
			}
		}
		materials[model->getSubset(name)] = material;
	}

	//---------------------------------------------------------------------------
	//Desc:    set materials to the subsets
	//Params:  path - material list file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectSkinnedMesh::setMaterialList(const String &path) {
		FILE *mFile = fopen(String("data/meshes/" + path).c_str(), "rt");

		//Check if exist
		if(!mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while(!feof(mFile)) {
			String line = FileHelper::readString(mFile);
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
		fclose(mFile);
	}



	//---------------------------------------------------------------------------
	//Desc:    sets object transform
	//Params:  trans - transform matrix
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectSkinnedMesh::setTransform(const Mat4 &trans) {
		transform = trans;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets object transform
	//Params:  -
	//Returns: object transformation matrix
	//---------------------------------------------------------------------------
	Mat4 ObjectSkinnedMesh::getTransform() {
		return transform;
	}

}