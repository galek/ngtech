#pragma once


namespace NGTech
{

	/**
	Skinned mesh class
	*/
	class CORE_API SkinnedMesh
	{
	public:
		/**
		Creates new SkinnedSkinnedMesh from file
		*/
		SkinnedMesh(const String &path);

		/**
		destroys object
		*/
		~SkinnedMesh();

		/**
		Loads new SkinnedSkinnedMesh from file
		*/
		void Load(const String &path);

		/**
		draws object subset
		*/
		void drawSubset(int subset);

		/**
		gets number of subsets
		*/
		ENGINE_INLINE int GetNumSubsets()	{
			return numSubsets;
		}

		/**
		get subset number by number
		*/
		int GetSubset(String name);

		/**
		*/
		const BBox &GetBBox(int subset = -1)
		{
			if (subset > 0)
				return subsets[subset]->bBox;
			return bBox;
		}

		/**
		*/
		const BSphere &GetBSphere(int subset = -1)
		{
			if (subset > 0)
				return subsets[subset]->bSphere;
			return bSphere;
		}

		/**
		*/
		void SetFrame(float frame, int from = 0, int to = -1);

		/*
		*/
		void CalculateBoundings();

		/*
		*/
		void CalculateTBN();

		/**
		*/
		ENGINE_INLINE const String &GetPath() {
			return path;
		}

	public:
		BBox bBox;
		BSphere bSphere;
	private:

		/**
		*/
		void CreateVBO();

		String path;

		struct Weight
		{
			int bone;
			float weight;
			Vec3 position;
			Vec3 normal;
			Vec3 tangent;
			Vec3 binormal;
		};

		struct Vertex
		{
			Vec3 position;
			Vec2 texcoord;
			Vec3 normal;
			Vec3 tangent;
			Vec3 binormal;
			int numWeights;
			Weight *weights;
		};

		struct Bone
		{
			String name;
			Mat4 transform;
			Mat4 rotation;
			int parent;
		};

		struct Frame
		{
			Vec3 position;
			Quat rotation;
		};

		unsigned int numBones;
		Bone *bones;

		unsigned int numFrames;
		Frame **frames;

		struct Subset
		{
			String name;

			Subset()
			{
				vertBuff = nullptr;
				numVertices = 0;
				numIndices = 0;
			};

			unsigned int numVertices;
			unsigned int numIndices;

			Vertex *vertices;
			unsigned int *indices;

			I_VBManager* vertBuff;

			BBox bBox;
			BSphere bSphere;

			bool visible;
		};
		unsigned int numSubsets;

	public:
		Subset **subsets;

		bool visible;

	private:
		friend class SkinnedMeshLoader;
		friend class SkinnedMeshFormatXSSMSH;
		friend class SceneNodeSkinnedMesh;
	};

}
