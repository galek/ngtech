#pragma once

//**************************************
//#include "../../Common/StringHelper.h"
#include "../../Core/inc/MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "../../Core/inc/Model.h"
#include "../../Core/inc/SkinnedMesh.h"
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
			OBJECT_SKELETEAL_MESH,
			OBJECT_PARTICLE_SYSTEM,
			OBJECT_CAMERA
		};

		virtual void drawSubset(int s) = 0;
		ENGINE_INLINE virtual int getNumSubsets() = 0;

		virtual Vec3 &getMax() = 0;
		virtual Vec3 &getMin() = 0;
		virtual Vec3 &getCenter() = 0;
		ENGINE_INLINE virtual float getRadius() = 0;
		ENGINE_INLINE virtual float getRadius(int s) = 0;

		virtual Vec3 &getMax(int s) = 0;
		virtual Vec3 &getMin(int s) = 0;
		virtual Vec3 &getCenter(int s) = 0;


		ENGINE_INLINE virtual Material *getMaterial(int s) = 0;

		ENGINE_INLINE virtual void setTransform(const Mat4 &trans) {}
		ENGINE_INLINE virtual Mat4 getTransform() = 0;

		ENGINE_INLINE virtual PhysBody *getPhysBody() = 0;

		ENGINE_INLINE virtual ObjectType getType() { return OBJECT; }
	};

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class ENGINE_API ObjectMesh : public Object{
	public:
		explicit ObjectMesh(const String &path);
		virtual ~ObjectMesh();

		virtual void drawSubset(int s);
		ENGINE_INLINE virtual int getNumSubsets() { return model->getNumSubsets(); };

		ENGINE_INLINE virtual Vec3 &getMax() { return model->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin() { return model->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter() { return model->bSphere.center; };
		ENGINE_INLINE virtual float getRadius() { return model->bSphere.radius; };

		ENGINE_INLINE virtual Vec3 &getMax(int s) { return model->subsets[s]->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin(int s) { return model->subsets[s]->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter(int s) { return model->subsets[s]->bSphere.center; };
		ENGINE_INLINE virtual float getRadius(int s) { return model->subsets[s]->bSphere.radius; };

		virtual Material *getMaterial(int s);

		void setMaterial(const String &path, const String &subsetName = "*");
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
		void setPhysicsCloth();
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return pBody; };

		void setImpactSound(const String &path);
		ENGINE_INLINE virtual ObjectType getType() { return OBJECT_MESH; };

	private:
		Model *model;

		PhysBody *pBody;
		ALSound *impactSound;
		std::vector<Material*> materials;

		Mat4 transform;

		bool visible;

		friend class Scene;
	};


	class ENGINE_API ObjectSkinnedMesh : public Object{
	public:
		explicit ObjectSkinnedMesh(const String &path);
		virtual ~ObjectSkinnedMesh();

		ENGINE_INLINE virtual Vec3 &getMax() { return mesh->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin() { return mesh->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter() { return mesh->bSphere.center; };
		ENGINE_INLINE virtual float getRadius() { return mesh->bSphere.radius; };

		ENGINE_INLINE virtual Vec3 &getMax(int s) { return mesh->subsets[s]->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin(int s) { return mesh->subsets[s]->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter(int s) { return mesh->subsets[s]->bSphere.center; };
		ENGINE_INLINE virtual float getRadius(int s) { return mesh->subsets[s]->bSphere.radius; };

		virtual void setTransform(const Mat4 &trans);
		virtual Mat4 getTransform();

		//Added
		/**
		draws object subset
		*/
		virtual void drawSubset(int s);

		/**
		get number of object subsets
		*/
		virtual int getNumSubsets();

		/**
		get BBox of the object subset
		*/
		virtual const BBox &getBBox(int subset = -1);

		/**
		get BSphere of the object subset
		*/
		virtual const BSphere &getBSphere(int subset = -1);

		/**
		get subset material by number
		*/
		virtual Material *getMaterial(int subset);

		/*
		Not virtual
		*/
		void setAnimationFrame(int frame, int from = -1, int to = -1);
		void setMaterial(const String &path, const String &subset = "*");
		void setMaterialList(const String &path);

		/**
		Serializes attributes
		*/
		//virtual void SerializeAttributes(Attributes *attributes);

		/**
		Deserializes attributes
		*/
		//static SceneNodeSkinnedMesh *DeserializeAttributes(Attributes *attributes);

		//Added
		ENGINE_INLINE virtual ObjectType getType() { return OBJECT_SKELETEAL_MESH; };
		PhysBody *Object::getPhysBody(void){ return NULL; }
	private:
		SkinnedMesh *mesh;

		ALSound *impactSound;
		Material **materials;

		Mat4 transform;

		bool visible;

		friend class Scene;
	};
}