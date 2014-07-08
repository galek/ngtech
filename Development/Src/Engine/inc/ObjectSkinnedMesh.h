/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#include "String.h"
#include "MathLib.h"
#include "GLSystem.h"
#include "MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "Log.h"
#include "Model.h"
//**************************************

namespace NGTech {
	//---------------------------------------------------------------------------
	//Desc: base object class
	//---------------------------------------------------------------------------
	class Object {
	public:
		enum ObjectType {
			OBJECT,
			OBJECT_MESH,
		};

		virtual ObjectType getType() { return OBJECT; };
	};

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class ObjectMesh : Object{
	public:
		ObjectMesh(String path);
		~ObjectMesh();

		void drawSubset(int s);
		int getNumSubsets() { return model->numSubsets; };

		Vec3 &getMax() { return model->max; };
		Vec3 &getMin() { return model->min; };
		Vec3 &getCenter() { return model->center; };
		float getRadius() { return model->radius; };

		Vec3 &getMax(int s) { return model->subsets[s].max; };
		Vec3 &getMin(int s) { return model->subsets[s].min; };
		Vec3 &getCenter(int s) { return model->subsets[s].center; };
		float getRadius(int s) { return model->subsets[s].radius; };

		bool intersectByRay(int s, const Vec3 &src, const Vec3 &dst, Vec3 &point);
		bool intersectByRay(const Vec3 &src, const Vec3 &dst, Vec3 &point);

		Material *getMaterial(int s);

		void setMaterial(String name, String path);
		void setMaterialList(String path);

		void setTransform(const Mat4 &trans);
		Mat4 getTransform();

		//---physics---------------------------
	public:
		void setPhysicsBox(const Vec3 &size, float mass = 0);
		void setPhysicsSphere(const Vec3 &size, float mass = 0);

		void setPhysicsCylinder(float radius, float height, float mass = 0);
		void setPhysicsCone(float radius, float height, float mass = 0);
		void setPhysicsCapsule(float radius, float height, float mass = 0);
		void setPhysicsChampferCylinder(float radius, float height, float mass = 0);

		void setPhysicsConvexHull(float mass = 0);
		void setPhysicsStaticMesh();
		PhysBody *getPhysBody() { return pBody; };

		void setImpactSound(const String &path);
		virtual ObjectType getType() { return OBJECT_MESH; };

		Model *model;
	private:
		PhysBody *pBody;
		Material **materials;


		Mat4 transform;

		bool visible;

		friend class Scene;
	};
	