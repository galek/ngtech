/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "../Common/EString.h"
#include "MathLib.h"
#include "MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "Log.h"
#include "Model.h"
#include "SkinnedModel.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: base object class
	//---------------------------------------------------------------------------
	class ENGINE_API Object {
	public:
		Object();
		enum ObjectType {
			OBJECT,
			OBJECT_MESH,
			OBJECT_SKINNED_MESH,
			OBJECT_PARTICLE_SYSTEM,
		};

		virtual void drawSubset(int s) {};
		virtual int getNumSubsets() { return 0; };

		virtual Vec3 &getMax() { return Vec3(); };
		virtual Vec3 &getMin() { return Vec3(); };
		virtual Vec3 &getCenter() { return Vec3(); };
		virtual float getRadius() { return 0; };

		virtual Vec3 &getMax(int s) { return Vec3(); };
		virtual Vec3 &getMin(int s) { return Vec3(); };
		virtual Vec3 &getCenter(int s) { return Vec3(); };
		virtual float getRadius(int s) { return 0; };

		virtual Material *getMaterial(int s) { return NULL; };

		virtual void setTransform(const Mat4 &trans) {};
		virtual Mat4 getTransform() { return Mat4(); };

		virtual PhysBody *getPhysBody() { return NULL; };

		virtual ObjectType getType() { return OBJECT; };
	};

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class ENGINE_API ObjectMesh : public Object{
	public:
		ObjectMesh(const String &path);
		~ObjectMesh();

		virtual void drawSubset(int s);
		virtual int getNumSubsets() { return model->getNumSubsets(); };

		virtual Vec3 &getMax() { return model->max; };
		virtual Vec3 &getMin() { return model->min; };
		virtual Vec3 &getCenter() { return model->center; };
		virtual float getRadius() { return model->radius; };

		virtual Vec3 &getMax(int s) { return model->subsets[s]->max; };
		virtual Vec3 &getMin(int s) { return model->subsets[s]->min; };
		virtual Vec3 &getCenter(int s) { return model->subsets[s]->center; };
		virtual float getRadius(int s) { return model->subsets[s]->radius; };

		virtual Material *getMaterial(int s);

		void setMaterial(const String &name, const String &path);
		void setMaterialList(const String &path);

		virtual void setTransform(const Mat4 &trans);
		virtual Mat4 getTransform();

		//---physics---------------------------
	public:
		void setPhysicsBox(const Vec3 &size, float mass = 0);
		void setPhysicsSphere(const Vec3 &size, float mass = 0);

		void setPhysicsCylinder(float radius, float width, float mass = 0);
		void setPhysicsCone(float radius, float height, float mass = 0);
		void setPhysicsCapsule(float radius, float height, float mass = 0);
		void setPhysicsChampferCylinder(float radius, float height, float mass = 0);

		void setPhysicsConvexHull(float mass = 0);
		void setPhysicsStaticMesh();
		virtual PhysBody *getPhysBody() { return pBody; };

		void setImpactSound(const String &path);
		virtual ObjectType getType() { return OBJECT_MESH; };

	private:
		Model *model;

		PhysBody *pBody;
		ALSound *impactSound;
		std::vector<Material*> materials;

		Mat4 transform;

		bool visible;

		friend class Scene;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class ENGINE_API ObjectSkinnedMesh : public Object {
	public:
		ObjectSkinnedMesh(const String &path);
		~ObjectSkinnedMesh();

		virtual void drawSubset(int s);
		virtual int getNumSubsets() { return model->getNumSubsets(); };

		virtual Vec3 &getMax() { return model->max; };
		virtual Vec3 &getMin() { return model->min; };
		virtual Vec3 &getCenter() { return model->center; };
		virtual float getRadius() { return model->radius; };

		virtual Vec3 &getMax(int s) { return model->subsets[s]->max; };
		virtual Vec3 &getMin(int s) { return model->subsets[s]->min; };
		virtual Vec3 &getCenter(int s) { return model->subsets[s]->center; };
		virtual float getRadius(int s) { return model->subsets[s]->radius; };

		virtual Material *getMaterial(int s);

		void setMaterial(const String &name, const String &path);
		void setMaterialList(const String &path);

		virtual void setTransform(const Mat4 &trans);
		virtual Mat4 getTransform();

		void setFrame(float frame) { model->setFrame(frame, -1, -1); };

		virtual ObjectType getType() { return OBJECT_SKINNED_MESH; };

	private:
		SkinnedModel *model;

		std::vector<Material*> materials;

		Mat4 transform;
		bool visible;

		friend class Scene;
	};
}