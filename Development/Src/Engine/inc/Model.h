#pragma once

//**************************************
#include "../../Common/EString.h"
#include "../../Core/inc/MathLib.h"
#include "PhysSystem.h"
#include "Frustum.h"
#include "Material.h"
#include "../../Core/inc/Log.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class Model {
	public:
		explicit Model(const String &path);
		~Model();

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

	private:
		void loadAMDL(const String &path);

		struct Vertex {
			Vec3 position;
			Vec2 texcoord;
			Vec3 normal;
			Vec3 tangent;
			Vec3 binormal;
		};

		struct Subset {
			char name[32];

			Subset() {
				vertBuff = NULL;

				numVertices = 0;
				numIndices = 0;
			};

			int numVertices;
			int numIndices;

			Vertex *vertices;
			unsigned int *indices;

			I_VBManager *vertBuff;

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
		friend class ObjectMesh;
	};

}