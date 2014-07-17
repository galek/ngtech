#pragma once

#include "IRender.h"
#include "MathLib.h"
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

		const Vec3 &getPosition() { return position; };
		void setPosition(const Vec3 &position) { this->position = position; };

		const Vec3 &getColor() { return color; };
		void setColor(const Vec3 &color) { this->color = color; };

		float getRadius() { return radius; };
		void setRadius(float radius) { this->radius = radius; };

	private:
		Vec3 position, color;

		I_Texture *texture;
		I_DisplayList *flareList;

		float radius;
	};

}