/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#include "GLSystem.h"
#include "MathLib.h"
#include "Flare.h"
//**************************************

namespace NGTech {

	class ENGINE_API Light {
	public:
		Light();
		enum LightType {
			LIGHT,
			LIGHT_OMNI,
			LIGHT_SPOT,
			LIGHT_DIRECT,
		};

		virtual LightType getType() { return LIGHT; };
		bool visible;
		void enable(bool flag) { enabled = flag; };
		bool enabled;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light omni light
	//---------------------------------------------------------------------------
	class ENGINE_API LightOmni : public Light {
	public:
		LightOmni();
		~LightOmni();

		const Vec3 &getPosition() { return position; };
		void setPosition(const Vec3 &position) { this->position = position; };

		const Vec3 &getColor() { return color; };
		void setColor(const Vec3 &color) { this->color = color; };

		float getRadius() { return radius; };
		float getIRadius() { return 1 / radius; };
		void setRadius(float radius) { this->radius = radius; };

		bool getShadows() { return castShadows; };
		void setShadows(bool shadows) { castShadows = shadows; };

		void setFlare(Flare *flare) { this->flare = flare; };

		virtual LightType getType() { return LIGHT_OMNI; };

		void getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w);

	private:
		Vec3 position, color;
		float radius;
		bool castShadows;

		GLTexture *shadowMap;

		Flare *flare;

		friend class Scene;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light spot light
	//---------------------------------------------------------------------------
	class ENGINE_API LightSpot : public Light {
	public:
		LightSpot();
		~LightSpot();

		const Vec3 &getPosition() { return position; };
		void setPosition(const Vec3 &position) { this->position = position; };

		const Vec3 &getDirection() { return direction; };
		void setDirection(const Vec3 &direction) { this->direction = direction; };

		const Vec3 &getColor() { return color; };
		void setColor(const Vec3 &color) { this->color = color; };

		float getFOV() { return fov; };
		void setFOV(float fov) { this->fov = fov; };

		float getRadius() { return radius; };
		float getIRadius() { return 1 / radius; };
		void setRadius(float radius) { this->radius = radius; };

		bool getShadows() { return castShadows; };
		void setShadows(bool shadows) { castShadows = shadows; };

		void setFlare(Flare *flare) { this->flare = flare; };

		virtual LightType getType() { return LIGHT_SPOT; };

		void getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w);

	private:
		Vec3 position, direction, color;
		float radius, fov;
		bool castShadows;

		Mat4 projTransform;

		GLTexture *shadowMap;
		GLTexture *projMap;

		Flare *flare;

		friend class Scene;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light dir light
	//---------------------------------------------------------------------------
	class ENGINE_API LightDirect : public Light {
	public:
		LightDirect();
		~LightDirect();

		const Vec3 &getDirection() { return direction; };
		void setDirection(const Vec3 &direction) { this->direction = direction; };

		const Vec3 &getColor() { return color; };
		void setColor(const Vec3 &color) { this->color = color; };

		virtual LightType getType() { return LIGHT_DIRECT; };

	private:
		Vec3 direction, color;

		friend class Scene;
	};
}