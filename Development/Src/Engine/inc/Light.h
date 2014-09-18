#pragma once

//**************************************
#include "../../Common/IRender.h"
#include "../../Core/inc/MathLib.h"
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

		ENGINE_INLINE virtual LightType getType() { return LIGHT; }
		ENGINE_INLINE void setEnable(bool flag) { enabled = flag; }
		ENGINE_INLINE bool isVisible() { return visible; }
		ENGINE_INLINE bool isEnable() { return enabled; }
		ENGINE_INLINE void setVisible(bool _s) { visible = _s; }
	private:
		bool enabled;
		bool visible;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light omni light
	//---------------------------------------------------------------------------
	class ENGINE_API LightOmni : public Light {
	public:
		LightOmni();
		virtual ~LightOmni();

		ENGINE_INLINE const Vec3 &getPosition() { return position; };
		ENGINE_INLINE void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE const Vec3 &getColor() { return color; };
		ENGINE_INLINE void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE float getRadius() { return radius; };
		ENGINE_INLINE float getIRadius() { return 1 / radius; };
		ENGINE_INLINE void setRadius(float radius) { this->radius = radius; };

		ENGINE_INLINE bool getShadows() { return castShadows; };
		ENGINE_INLINE void setShadows(bool shadows) { castShadows = shadows; };

		ENGINE_INLINE void setFlare(Flare *flare) { this->flare = flare; };

		ENGINE_INLINE virtual LightType getType() { return LIGHT_OMNI; };

		void getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w);

	private:
		Vec3 position, color;
		float radius;
		bool castShadows;

		I_Texture *shadowMap;

		Flare *flare;

		friend class Scene;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light spot light
	//---------------------------------------------------------------------------
	class ENGINE_API LightSpot : public Light {
	public:
		LightSpot();
		virtual ~LightSpot();

		ENGINE_INLINE const Vec3 &getPosition() { return position; };
		ENGINE_INLINE void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE const Vec3 &getDirection() { return direction; };
		ENGINE_INLINE void setDirection(const Vec3 &direction) { this->direction = direction; };

		ENGINE_INLINE const Vec3 &getColor() { return color; };
		ENGINE_INLINE void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE float getFOV() { return fov; };
		ENGINE_INLINE void setFOV(float fov) { this->fov = fov; };

		ENGINE_INLINE float getRadius() { return radius; };
		ENGINE_INLINE float getIRadius() { return 1 / radius; };
		ENGINE_INLINE void setRadius(float radius) { this->radius = radius; };

		ENGINE_INLINE bool getShadows() { return castShadows; };
		ENGINE_INLINE void setShadows(bool shadows) { castShadows = shadows; };

		ENGINE_INLINE void setFlare(Flare *flare) { this->flare = flare; };

		ENGINE_INLINE virtual LightType getType() { return LIGHT_SPOT; };

		void getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w);

	private:
		Vec3 position, direction, color;
		float radius, fov;
		bool castShadows;

		Mat4 projTransform;

		I_Texture *shadowMap;
		I_Texture *projMap;

		Flare *flare;

		friend class Scene;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the light dir light
	//---------------------------------------------------------------------------
	class ENGINE_API LightDirect : public Light {
	public:
		LightDirect();
		virtual ~LightDirect();

		ENGINE_INLINE const Vec3 &getDirection() { return direction; };
		ENGINE_INLINE void setDirection(const Vec3 &direction) { this->direction = direction; };

		ENGINE_INLINE const Vec3 &getColor() { return color; };
		ENGINE_INLINE void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE virtual LightType getType() { return LIGHT_DIRECT; };

	private:
		Vec3 direction, color;

		friend class Scene;
	};
}