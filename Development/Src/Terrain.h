/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//************************************
#include "GLSystem.h"
#include "ILSystem.h"
#include "PhysSystem.h"
#include "Material.h"
//************************************

namespace VEGA {

	class VEGA_API Terrain {
	public:
		Terrain(const String &path, float step, float height, int nodeSize);
		~Terrain();

		void drawNode(int n, const Vec3 &cameraPos);
		int getNumNodes() { return numNodes; };

		void setLods(const Vec3 &lods) { this->lods = lods; };

		const Vec3 &getMin(int n);
		const Vec3 &getMax(int n);
		const Vec3 &getCenter(int n);
		float getRadius(int n);

		void setPhysics();

		void computeTBN();

		void setMaterial(const String &path) { this->material = new Material(path); };
		Material *getMaterial() { return material; };

	private:
		Vec3 lods;

		struct Vertex {
			Vec3 position;
			Vec2 texcoord;
			Vec3 normal;
		};

		struct Node {
			Vec3 min, max, center;
			float radius;

			unsigned int *indices[4];
			unsigned int numIndices[4];

			bool visible;
		};

		int numNodes;
		Node *nodes;

		Material *material;

		GLVBO *vertBuff;
		Vertex *vertices;
		unsigned int numVertices;

		PhysBody *pBody;

		friend class Scene;
	};

};