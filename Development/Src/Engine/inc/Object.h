#pragma once

//**************************************
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
	class ENGINE_API Object
	{
	public:
		Object();
		enum ObjectType {
			OBJECT,
			OBJECT_MESH,
			OBJECT_SKELETEAL_MESH,
			OBJECT_PARTICLE_SYSTEM,
			OBJECT_CAMERA
		};

		/**
		Renderable
		*/
		ENGINE_INLINE virtual Material *GetMaterial(size_t s) = 0;
		ENGINE_INLINE virtual void DrawSubset(size_t s) {}
		ENGINE_INLINE virtual size_t GetNumSubsets() { return 0; }

		virtual Vec3 &getMax() = 0;
		virtual Vec3 &getMin() = 0;
		virtual Vec3 &getCenter() = 0;
		ENGINE_INLINE virtual float getRadius() = 0;
		ENGINE_INLINE virtual float getRadius(size_t s) = 0;

		virtual Vec3 &getMax(size_t s) = 0;
		virtual Vec3 &getMin(size_t s) = 0;
		virtual Vec3 &getCenter(size_t s) = 0;



		ENGINE_INLINE virtual PhysBody *getPhysBody() = 0;

		ENGINE_INLINE virtual ObjectType getType() { return OBJECT; }

		ENGINE_INLINE const BBox &GetBBox(int subset = -1) { return box; }
		ENGINE_INLINE const BSphere &GetBSphere(int subset = -1) { return sphere; }
		ENGINE_INLINE BBox &GetTransformedBBox(int subset = -1) { return GetTransform() * GetBBox(subset); }
		ENGINE_INLINE BSphere &GetTransformedBSphere(int subset = -1)	{ return GetTransform() * GetBSphere(subset); }

		ENGINE_INLINE virtual void SetTransform(const Mat4 &_transform) { transform = _transform; }
		ENGINE_INLINE virtual const Mat4 GetTransform() { return transform; }
	protected:
		BBox box;
		BSphere sphere;

		Mat4 transform;

		Vec3 position;
		Vec3 direction;
		Vec3 upVector;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class ENGINE_API ObjectMesh : public Object
	{
	public:
		explicit ObjectMesh(const String &path);
		virtual ~ObjectMesh();

		virtual void DrawSubset(size_t s);
		ENGINE_INLINE virtual size_t GetNumSubsets() { return mesh->getNumSubsets(); };

		ENGINE_INLINE virtual Vec3 &getMax() { return mesh->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin() { return mesh->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter() { return mesh->bSphere.center; };
		ENGINE_INLINE virtual float getRadius() { return mesh->bSphere.radius; };

		ENGINE_INLINE virtual Vec3 &getMax(size_t s) { return mesh->subsets[s]->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin(size_t s) { return mesh->subsets[s]->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter(size_t s) { return mesh->subsets[s]->bSphere.center; };
		ENGINE_INLINE virtual float getRadius(size_t s) { return mesh->subsets[s]->bSphere.radius; };

		virtual Material *GetMaterial(size_t s);

		void setMaterial(const String &path, const String &subsetName = "*");
		void setMaterialList(const String &path);

		virtual void SetTransform(const Mat4 &trans);
		virtual const Mat4 GetTransform();
	public:
		void setPhysicsBox(const Vec3 &size, float mass = 0);
		void setPhysicsSphere(const Vec3 &size, float mass = 0);

		void setPhysicsCylinder(float radius, float width, float mass = 0);
		void setPhysicsCapsule(float radius, float height, float mass = 0);
		
		void setPhysicsConvexHull(float mass = 0);
		void setPhysicsStaticMesh();
		void setPhysicsCloth();
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return pBody; };

		void setImpactSound(const String &path);
		ENGINE_INLINE virtual ObjectType getType() { return OBJECT_MESH; };

	private:
		Model *mesh;

		PhysBody *pBody;
		ALSound *impactSound;
		std::vector<Material*> materials;

		bool visible;
		bool bMultipart;

		friend class Scene;
	};


	class ENGINE_API ObjectSkinnedMesh : public Object
	{
	public:
		explicit ObjectSkinnedMesh(const String &path);
		virtual ~ObjectSkinnedMesh();

		ENGINE_INLINE virtual Vec3 &getMax() { return mesh->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin() { return mesh->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter() { return mesh->bSphere.center; };
		ENGINE_INLINE virtual float getRadius() { return mesh->bSphere.radius; };

		ENGINE_INLINE virtual Vec3 &getMax(size_t s) { return mesh->subsets[s]->bBox.max; };
		ENGINE_INLINE virtual Vec3 &getMin(size_t s) { return mesh->subsets[s]->bBox.min; };
		ENGINE_INLINE virtual Vec3 &getCenter(size_t s) { return mesh->subsets[s]->bSphere.center; };
		ENGINE_INLINE virtual float getRadius(size_t s) { return mesh->subsets[s]->bSphere.radius; };

		/**
		draws object subset
		*/
		virtual void DrawSubset(size_t s);

		/**
		get number of object subsets
		*/
		virtual size_t GetNumSubsets();

		/**
		get BBox of the object subset
		*/
		virtual const BBox &getBBox(size_t subset = -1);

		/**
		get BSphere of the object subset
		*/
		virtual const BSphere &getBSphere(size_t subset = -1);

		/**
		get subset material by number
		*/
		virtual Material *GetMaterial(size_t subset);

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

		bool visible;

		friend class Scene;
	};
}