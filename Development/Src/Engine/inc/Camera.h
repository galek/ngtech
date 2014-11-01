#pragma once

//**************************************
#include "../../Common/StringHelper.h"
//**************************************
#include "../../Core/inc/MathLib.h"
//**************************************
#include "PhysSystem.h"
#include "Object.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: base class of the camera
	//---------------------------------------------------------------------------
	class ENGINE_API Camera :public Object {
	public:
		Camera();
		enum CameraType {
			CAMERA,
			CAMERA_FPS,
			CAMERA_FREE,
			CAMERA_FIXED
		};

		/**
		*/
		ENGINE_INLINE virtual void drawSubset(int) { }

		/**
		*/
		ENGINE_INLINE virtual ObjectType getType() { return OBJECT_CAMERA; };

		/**
		*/
		ENGINE_INLINE virtual CameraType getCameraType() { return CAMERA; };

		/**
		*/
		ENGINE_INLINE virtual Vec3 &getPosition() { return position; }

		/**
		*/
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) { this->position = position; }

		/**
		sets camera screen aspect
		*/
		ENGINE_INLINE virtual Vec3 &getDirection() { return direction; }

		ENGINE_INLINE virtual void setDirection(const Vec3 &direction) { this->direction = direction; }

		ENGINE_INLINE virtual float getMaxVelocity() { return maxVelocity; }

		ENGINE_INLINE virtual void setMaxVelocity(float maxVelocity) { this->maxVelocity = maxVelocity; }

		//TODO
		virtual Vec3 &getMax(int s) { return Vec3(0, 0, 0); }
		virtual Vec3 &getMin(int s) { return Vec3(0, 0, 0); }
		virtual Vec3 &getCenter(int s) { return Vec3(0, 0, 0); }
		//TODO
		virtual Vec3 &getMax() { return Vec3(0, 0, 0); }
		virtual Vec3 &getMin() { return Vec3(0, 0, 0); }
		virtual Vec3 &getCenter() { return Vec3(0, 0, 0); }

		/**
		gets camera fov
		\return fov
		*/
		ENGINE_INLINE float getFOV() { return fov; }

		/**
		sets camera fov
		*/
		ENGINE_INLINE void setFOV(float _fov) { fov = _fov; }

		/**
		gets camera screen aspect
		\return aspect
		*/
		ENGINE_INLINE float getAspect(){ return aspect; }
		/**
		sets camera screen aspect
		*/
		ENGINE_INLINE void setAspect(float _aspect){ aspect = _aspect; }

		/**
		gets camera zNear clip plane distance
		\return distance
		*/
		ENGINE_INLINE float getZNear(){ return zNear; }

		/**
		sets camera zNear clip plane distance
		*/
		ENGINE_INLINE void setZNear(float _zNear){ zNear = _zNear; }

		/**
		gets camera zFar clip plane distance
		\return distance
		*/
		ENGINE_INLINE float getZFar(){ return zFar; }

		/**
		sets camera zFar clip plane distance
		*/
		ENGINE_INLINE void setZFar(float _zFar){ zFar = _zFar; }

		/**
		*/
		ENGINE_INLINE virtual Mat4 getTransform();

		/**
		gets camera projection matrix
		\return projection matrix
		*/
		ENGINE_INLINE virtual Mat4 getProjection();

		/**
		*/
		ENGINE_INLINE virtual void update() {}

		/**
		*/
		ENGINE_INLINE float getAngle(int _i) { if (_i <= 2) return angle[_i]; else return 0; }

		/**
		*/
		ENGINE_INLINE void setAngle(int _i, float _v) { if (_i <= 2) angle[_i] = _v; }

		/**
		*/
		ENGINE_INLINE void lookAt(float _x, float _y) { angle[0] = -0.4 * _x; angle[1] = -0.4 * _y; }

	protected:
		/**
		*/
		ENGINE_INLINE virtual void setPhysics(const Vec3 &size, float mass) {}
		/**
		gets camera phys body
		\return phys body
		*/
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return NULL; }
	private:
		/**
		*/
		ENGINE_INLINE virtual int getNumSubsets() { return 0; };
		/**
		*/
		ENGINE_INLINE virtual float getRadius(int s) { return 0; };
		/**
		*/
		ENGINE_INLINE virtual float getRadius() { return 0; };
		/**
		*/
		ENGINE_INLINE virtual Material* getMaterial(int s) { return NULL; };
	protected:
		float angle[2];
		float fov;
		float aspect;
		float zNear, zFar;

		Mat4 view;
		Mat4 transform;

		Vec3 position;
		Vec3 direction;

		float maxVelocity;
	};

	/**
	Class of the FPS camera
	*/
	class ENGINE_API CameraFPS : public Camera {
	public:
		/**
		Creates new FPS camera
		*/
		CameraFPS();
		/**
		Camera destructor
		*/
		virtual ~CameraFPS();

		ENGINE_INLINE virtual CameraType getCameraType() { return CAMERA_FPS; };

		virtual void update();

		//---physics---------------------------
	public:
		virtual void setPhysics(const Vec3 &size, float mass);
		/**
		gets camera phys body
		\return phys body
		*/
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return pBody; };

	private:
		ALSound *jumpSnd;
		ALSoundSource *jumpSrc;

		PhysBody *pBody;
		PhysJointUpVector *pJoint;
		Vec3 size;
	};

	/**
	*/
	class ENGINE_API CameraFree : public Camera {
	public:
		CameraFree();
		virtual ~CameraFree();

		ENGINE_INLINE virtual CameraType getCameraType() { return CAMERA_FREE; };

		virtual void update();

		//---physics---------------------------
	public:
		virtual void setPhysics(const Vec3 &size, float mass);

	private:
		struct PhysXCharacterController* pBody;
	};

	/**
	*/
	class ENGINE_API CameraFixed : public Camera {
	public:
		CameraFixed();
		virtual ~CameraFixed();

		ENGINE_INLINE virtual CameraType getCameraType() { return CAMERA_FIXED; };

		virtual void update();

		//---physics---------------------------
	public:
		virtual void setPhysics(const Vec3 &size, float mass);

	private:
		PhysBody *pBody;
	};
}