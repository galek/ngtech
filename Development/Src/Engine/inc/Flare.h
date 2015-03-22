#pragma once

#include "../../Common/IRender.h"
#include "../../Common/MathLib.h"
#include <vector>

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the system flare
	//---------------------------------------------------------------------------
	class Flare {
	public:
		explicit Flare(const String &path);
		~Flare();

		void draw();

		ENGINE_INLINE const Vec3 &getPosition() { return position; };
		ENGINE_INLINE void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE const Vec3 &getColor() { return color; };
		ENGINE_INLINE void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE float getRadius() { return radius; };
		ENGINE_INLINE void setRadius(float radius) { this->radius = radius; };

	private:
		Vec3 position, color;

		I_Texture *texture;
		I_DisplayList *flareList;

		float radius;
	};

}