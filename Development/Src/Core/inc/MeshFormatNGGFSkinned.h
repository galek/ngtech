#pragma once

namespace NGTech
{
	class SkinnedMesh;
	/**
	SkinnedMesh format XSSMSH
	*/
	class MeshFormatNGGFSkinned : public I_MeshFormatSkinned
	{
	public:
		/**
		*/
		MeshFormatNGGFSkinned();

		/**
		*/
		virtual ~MeshFormatNGGFSkinned();

		/**
		*/
		virtual void Save(const String &path, SkinnedMesh *mesh);

		/**
		*/
		virtual bool Load(const String &path, SkinnedMesh *mesh);

		/**
		*/
		ENGINE_INLINE virtual const char* GetExt() { return "nggf"; }
	};

}
