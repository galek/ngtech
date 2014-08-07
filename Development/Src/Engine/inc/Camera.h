#pragma once

//**************************************
#include "MathLib.h"
#include "PhysSystem.h"
#include "../Common/StringHelper.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: base class of the camera
	//---------------------------------------------------------------------------
	class ENGINE_API Camera {
	public:
		Camera();
		enum CameraType {
			CAMERA,
			CAMERA_FPS,
			CAMERA_FREE,
		};

		ENGINE_INLINE virtual CameraType getType() { return CAMERA; };

		ENGINE_INLINE virtual Vec3 &getPosition() { Vec3 v = Vec3(0, 0, 0); return v; };
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) {};

		ENGINE_INLINE virtual Vec3 &getDirection() { Vec3 v = Vec3(0, 0, 0); return v; };
		ENGINE_INLINE virtual void setDirection(const Vec3 &direction) {};

		ENGINE_INLINE virtual float getMaxVelocity() { return 0; };
		ENGINE_INLINE virtual void setMaxVelocity(float maxVelocity) {};

		ENGINE_INLINE virtual float getFOV() { return 0; };
		ENGINE_INLINE virtual void setFOV(float fov) {};

		ENGINE_INLINE virtual Mat4 getTransform() { return Mat4(); };
		ENGINE_INLINE virtual Mat4 getProjection() { return Mat4(); };

		ENGINE_INLINE virtual void update() {};
		ENGINE_INLINE float GetAngle(int _i) {	if (_i <= 2) return angle[_i]; else return 0;	}

		//---physics---------------------------
	public:
		ENGINE_INLINE virtual void setPhysics(const Vec3 &size, float mass) {};
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return NULL; };

		float angle[2];
	};

	//---------------------------------------------------------------------------
	//Desc: class of the FPS camera
	//---------------------------------------------------------------------------
	class ENGINE_API CameraFPS : public Camera {
	public:
		CameraFPS();
		~CameraFPS();

		ENGINE_INLINE virtual Vec3 &getPosition() { return position; };
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE virtual Vec3 &getDirection() { return direction; };
		ENGINE_INLINE virtual void setDirection(const Vec3 &direction) { this->direction = direction; };

		ENGINE_INLINE virtual float getMaxVelocity() { return maxVelocity; };
		ENGINE_INLINE virtual void setMaxVelocity(float maxVelocity) { this->maxVelocity = maxVelocity; };

		ENGINE_INLINE virtual float getFOV() { return fov; };
		ENGINE_INLINE virtual void setFOV(float fov) { this->fov = fov; };

		virtual Mat4 getTransform();
		virtual Mat4 getProjection();

		virtual void update();

		//---physics---------------------------
	public:
		virtual void setPhysics(const Vec3 &size, float mass);
		ENGINE_INLINE virtual PhysBody *getPhysBody() { return pBody; };

	private:
		ALSound *jumpSnd;
		ALSoundSource *jumpSrc;

		Vec3 position;
		Vec3 direction;

		float maxVelocity;
		float fov;

		PhysBody *pBody;
		PhysJointUpVector *pJoint;
		Vec3 size;
	};


	//---------------------------------------------------------------------------
	//Desc: class of the FPS camera
	//---------------------------------------------------------------------------
	class ENGINE_API CameraFree : public Camera {
	public:
		CameraFree();
		~CameraFree();

		ENGINE_INLINE virtual Vec3 &getPosition() { return position; };
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE virtual Vec3 &getDirection() { return direction; };
		ENGINE_INLINE virtual void setDirection(const Vec3 &direction) { this->direction = direction; };

		ENGINE_INLINE virtual float getMaxVelocity() { return maxVelocity; };
		ENGINE_INLINE virtual void setMaxVelocity(float maxVelocity) { this->maxVelocity = maxVelocity; };

		ENGINE_INLINE virtual float getFOV() { return fov; };
		ENGINE_INLINE virtual void setFOV(float fov) { this->fov = fov; };

		virtual Mat4 getTransform();
		virtual Mat4 getProjection();

		virtual void update();

		//---physics---------------------------
	public:
		virtual void setPhysics(const Vec3 &size, float mass);

	private:
		Vec3 position;
		Vec3 direction;

		float maxVelocity;
		float fov;

		PhysBody *pBody;
	};
}