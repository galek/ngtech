#pragma once

//************************************
#include "../../Common/IRender.h"
#include "PhysSystem.h"
#include "Material.h"
//************************************

namespace NGTech {

	class ENGINE_API Terrain {
	public:
		Terrain(const String &path, float step, float height, int nodeSize);
		~Terrain();

		void drawNode(int n, const Vec3 &cameraPos);
		ENGINE_INLINE int getNumNodes() { return numNodes; };

		ENGINE_INLINE void setLods(const Vec3 &lods) { this->lods = lods; };

		const Vec3 &getMin(int n);
		const Vec3 &getMax(int n);
		const Vec3 &getCenter(int n);
		float getRadius(int n);

		void setPhysics();

		void computeTBN();

		ENGINE_INLINE void setMaterial(const String &path) { this->material = new Material(path); };
		ENGINE_INLINE Material *getMaterial() { return material; };

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

		I_VBManager *vertBuff;
		Vertex *vertices;
		unsigned int numVertices;

		PhysBody *pBody;

		friend class Scene;
	};
}