/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "Common/EString.h"
#include "MathLib.h"
#include "GLSystem.h"
#include "MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "Log.h"
//**************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: class of the scene object
//---------------------------------------------------------------------------
class SkinnedModel {
public:
	SkinnedModel(const String &path);
	~SkinnedModel();

	void save(const String &path);
	void drawSubset(int s);
	int getNumSubsets() { return subsets.size(); };
	int getSubset(String name);

	Vec3 &getMax() { return max; };
	Vec3 &getMin() { return min; };
	Vec3 &getCenter() { return center; };
	float getRadius() { return radius; };

	Vec3 &getMax(int s) { return subsets[s]->max; };
	Vec3 &getMin(int s) { return subsets[s]->min; };
	Vec3 &getCenter(int s) { return subsets[s]->center; };
	float getRadius(int s) { return subsets[s]->radius; };

	void setFrame(float frame, int from = -1, int to = -1);

private:
	void loadASMDL(const String &path);
	
	struct Weight {
		int bone;
		float weight;
		Vec3 position;
		Vec3 normal;
		Vec3 tangent;
		Vec3 binormal;
	};
	
	struct Vertex {
		Vec3 position;
		Vec2 texcoord;
		Vec3 normal;
		Vec3 tangent;
		Vec3 binormal;
		int numWeights;
		Weight *weights;
	};

	struct Bone {
		char name[32];
		Mat4 transform;
		Mat4 rotation;
		int parent;
	};
	
	struct Frame {
		Vec3 position;			
		Quat rotation;			
	};
	
	int numBones;
	Bone *bones;
	
	int numFrames;
	Frame **frames;

	struct Subset {
		char name[32];

		Subset() {
			vertBuff = NULL;

			numVertices = 0;
			numIndices  = 0;
		};

		int numVertices;
		int numIndices;

		Vertex *vertices;
		unsigned int *indices;

		GLVBO *vertBuff;

		Vec3 min, max;
		Vec3 center;
		float radius;

		bool visible;
	};

	std::vector<Subset*> subsets;

	Vec3 min, max;
	Vec3 center;
	float radius;

	void calcBoundings();
	void calculateTBN();
	void createVBO();

	bool visible;

	friend class Scene;
	friend class ObjectSkinnedMesh;
};

};
