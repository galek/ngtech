#pragma once

//**************************************
#include "../../Common/StringHelper.h"
#include "../../Core/inc/MathLib.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the scene object
	//---------------------------------------------------------------------------
	class CORE_API Model {
	public:
		explicit Model(const String &path);
		~Model();

		void save(const String &path);
		void drawSubset(size_t s);
		ENGINE_INLINE size_t getNumSubsets() { return numSubsets; };
		int getSubset(String name);

		ENGINE_INLINE Vec3 &getMax() { return bBox.max; };
		ENGINE_INLINE Vec3 &getMin() { return bBox.min; };
		ENGINE_INLINE Vec3 &getCenter() { return bSphere.center; };
		ENGINE_INLINE float getRadius() { return bSphere.radius; };

		ENGINE_INLINE Vec3 &getMax(size_t s) { return subsets[s]->bBox.max; };
		ENGINE_INLINE Vec3 &getMin(size_t s) { return subsets[s]->bBox.min; };
		ENGINE_INLINE Vec3 &getCenter(size_t s) { return subsets[s]->bSphere.center; };
		ENGINE_INLINE float getRadius(size_t s) { return subsets[s]->bSphere.radius; };

		struct Vertex {
			Vec3 position;
			Vec2 texcoord;
			Vec3 normal;
			Vec3 tangent;
			Vec3 binormal;
			Vec2 lmTexcoord;
		};
		bool lmTexcoords;

	public:
		/**
		Loads Mesh from file
		*/
		void _load(const String &path);

		struct Subset {
			std::string name;

			Subset() {
				vertBuff = NULL;

				numVertices = 0;
				numIndices = 0;
			};

			unsigned int numVertices;
			unsigned int numIndices;

			Vertex *vertices;
			unsigned int *indices;

			I_VBManager *vertBuff;

			BBox bBox;
			BSphere bSphere;

			bool visible;
		};

		unsigned int numSubsets;
		Subset **subsets;

		BBox bBox;
		BSphere bSphere;

		void calcBoundings();

		void calculateTBN();

		void createVBO();

		bool visible;

		friend class Scene;
		friend class ObjectMesh;
	};

}