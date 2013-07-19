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
#include <vector>
//**************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: class of the system flare
//---------------------------------------------------------------------------
class Flare {
public:
	Flare(const String &path);
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
	
	GLTexture *texture;
	GLDisplayList *flareList;

	float radius;
};

};

