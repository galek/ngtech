/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#include "EnginePrivate.h"
//**************************************
#include "Object.h"
#include "File.h"
#include "Log.h"
#include "Frustum.h"
#include "MathLib.h"
#include "Error.h"
#include "Cash.h"
//**************************************

namespace VEGA {
	//---------------------------------------------------------------------------
	//Desc:    creates new ObjectMesh from file
	//Params:  path - object file path
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectMesh::ObjectMesh(const String & path) {
		model = engine.cash->loadModel(path);
		materials = new Material*[model->numSubsets];
		transform.identity();
		pBody = NULL;
		return;
	};

	//---------------------------------------------------------------------------
	//Desc:    destroys object
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ObjectMesh::~ObjectMesh() {
		delete model;
		delete [] materials;
	};

	//---------------------------------------------------------------------------
	//Desc:    draws object subset
	//Params:  s - surface number
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::drawSubset(int s) {
		model->drawSubset(s);
	};

	//---------------------------------------------------------------------------
	//Desc:    get subset material by number
	//Params:  s - subset number
	//Returns: subset`s material
	//---------------------------------------------------------------------------
	Material *ObjectMesh::getMaterial(int s) {
		return materials[s];
	};

	//---------------------------------------------------------------------------
	//Desc:    set material to the subset
	//Params:  name - subset name, path - material file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setMaterial(const String &name, const String &path) {
		Material *material = engine.cash->loadMaterial(path);
		if (name == "*")	{
			for (int s = 0; s < model->numSubsets; s++) {
				materials[s] = material;
			};
		};
		materials[model->getSubset(name)] = material;
	};

	//---------------------------------------------------------------------------
	//Desc:    set materials to the subsets
	//Params:  path - material list file path
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setMaterialList(String path) {
		path = "../data/meshes/" + path;

		//Check if exist
		if (!FileHelper::fileExist(path)) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		};

		FILE *mFile = fopen(path.c_str(), "rt");

		while (!feof(mFile)) {
			String line = FileHelper::readString(mFile);
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		};
		fclose(mFile);
	};


	//***************************************************************************
	//Physics
	//***************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    sets object transform
	//Params:  trans - transform matrix
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setTransform(const Mat4 &trans) {
		if (pBody == NULL) {
			transform = trans;
		}
		else {
			pBody->setTransform(trans);
		};
	};

	//---------------------------------------------------------------------------
	//Desc:    gets object transform
	//Params:  -
	//Returns: object transformation matrix
	//---------------------------------------------------------------------------
	Mat4 ObjectMesh::getTransform() {
		if (pBody == NULL) {
			return transform;
		}
		else {
			return pBody->getTransform();
		};
	};

	//---------------------------------------------------------------------------
	//Desc:    sets box object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsBox(const Vec3 &size, float mass) {
		pBody = PhysBody::createBox(size, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets sphere object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsSphere(const Vec3 &size, float mass) {
		pBody = PhysBody::createSphere(size, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets cylinder object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCylinder(float radius, float height, float mass) {
		pBody = PhysBody::createCylinder(radius, height, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets cone object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCone(float radius, float height, float mass) {
		pBody = PhysBody::createCone(radius, height, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets capsule object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsCapsule(float radius, float height, float mass)  {
		pBody = PhysBody::createCapsule(radius, height, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets champfer cylinder object collider with mass
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsChampferCylinder(float radius, float height, float mass) {
		pBody = PhysBody::createChampferCylinder(radius, height, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets convex hull object collider with infinite mass (static)
	//Params:  mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsConvexHull(float mass) {
		int numPos = 0;
		for (int i = 0; i < model->numSubsets; i++) {
			numPos += model->subsets[i].numVertices;
		};
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for (int i = 0; i < model->numSubsets; i++) {

			for (int v = 0; v < model->subsets[i].numVertices; v++) {
				pos[k] = model->subsets[i].positions[v];
				k++;
			};
		};

		pBody = PhysBody::createConvexHull(pos, numPos, mass);
		setTransform(transform);
	};

	//---------------------------------------------------------------------------
	//Desc:    sets static object collider
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ObjectMesh::setPhysicsStaticMesh() {
		int numPos = 0;
		for (int i = 0; i < model->numSubsets; i++) {
			numPos += model->subsets[i].numIndices;
		};
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for (int i = 0; i < model->numSubsets; i++) {

			for (int v = 0; v < model->subsets[i].numIndices / 3; v++) {
				pos[k * 3 + 0] = model->subsets[i].positions[model->subsets[i].indices[v * 3 + 0]];
				pos[k * 3 + 1] = model->subsets[i].positions[model->subsets[i].indices[v * 3 + 1]];
				pos[k * 3 + 2] = model->subsets[i].positions[model->subsets[i].indices[v * 3 + 2]];
				k++;
			};
		};

		pBody = PhysBody::createStaticObjectMesh(pos, numPos, true);
		setTransform(transform);
	};

	void ObjectMesh::setImpactSound(const String &path) {
		pBody->setImpactSound(engine.cash->loadSound("../data/sounds/" + path));
	};

	/*
	bool Model::intersectByRay(int s, const Vec3 &src, const Vec3 &dst, Vec3 &point) {
	Subset &st = subsets[s];

	Vec3 tsrc = Vec3(Vec4(src) * transform);
	Vec3 tdst = Vec3(Vec4(dst) * transform);

	if(!Math::intersectSphereByRay(st.center, st.radius, tsrc, tdst))
	return false;

	point = dst;
	bool f = false;

	for(int i = 0; i < st.numIndices/3; i++) {
	Vec3 in;
	if(Math::intersectPolygonByRay(st.positions[st.indices[i*3+0]],
	st.positions[st.indices[i*3+1]],
	st.positions[st.indices[i*3+2]],
	tsrc, tdst, in)) {

	f = true;
	if((in - tsrc).length() < (point - tsrc).length()) {
	point = in;
	};
	};
	};

	point = Vec3(Vec4(point) * Mat4::inverse(transform));
	return f;
	};

	bool Model::intersectByRay(const Vec3 &src, const Vec3 &dst, Vec3 &point) {
	point = dst;
	bool f = false;

	for(int i = 0; i < numSubsets; i++) {
	Vec3 in;
	if(intersectByRay(i, src, dst, in)) {
	f = true;

	if((in - src).length() < (point - src).length()) {
	point = in;
	};
	};
	};
	return f;
	};*/
}