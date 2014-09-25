#pragma once

//**************************************
#include "../../Common/StringHelper.h"
#include "../../Core/inc/MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "../../Core/inc/Log.h"
#include "Model.h"
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
			OBJECT_PARTICLE_SYSTEM,
		};

		ENGINE_INLINE virtual void drawSubset(int s) {}
		ENGINE_INLINE virtual int getNumSubsets() { return 0; }

		virtual Vec3 &getMax() = 0;
		virtual Vec3 &getMin() = 0;
		virtual Vec3 &getCenter() = 0;
		ENGINE_INLINE virtual float getRadius() { return 0; }

		virtual Vec3 &getMax(int s) = 0;
		virtual Vec3 &getMin(int s) = 0;
		virtual Vec3 &getCenter(int s) = 0;
		ENGINE_INLINE virtual float getRadius(int s) { return 0; }

		ENGINE_INLINE virtual Material *getMaterial(int s) { return NULL; }

		ENGINE_INLINE virtual void setTransform(const Mat4 &trans) {}
		ENGINE_INLINE virtual Mat4 getTransform() { return Mat4(); }

		ENGINE_INLINE virtual PhysBody *getPhysBody() { return NULL; }

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

		ENGINE_INLINE virtual Vec3 &getMax() { return model->max; };
		ENGINE_INLINE virtual Vec3 &getMin() { return model->min; };
		ENGINE_INLINE virtual Vec3 &getCenter() { return model->center; };
		ENGINE_INLINE virtual float getRadius() { return model->radius; };

		ENGINE_INLINE virtual Vec3 &getMax(int s) { return model->subsets[s]->max; };
		ENGINE_INLINE virtual Vec3 &getMin(int s) { return model->subsets[s]->min; };
		ENGINE_INLINE virtual Vec3 &getCenter(int s) { return model->subsets[s]->center; };
		ENGINE_INLINE virtual float getRadius(int s) { return model->subsets[s]->radius; };

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
}