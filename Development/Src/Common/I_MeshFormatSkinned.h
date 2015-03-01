#pragma once

namespace NGTech
{
	class SkinnedMesh;
	/**
	Skinned Mesh loader class
	*/
	class I_MeshFormatSkinned
	{
	public:
		virtual ~I_MeshFormatSkinned() {}

		/*
		*/
		virtual void Save(const String &path, SkinnedMesh *mesh) = 0;

		/*
		*/
		virtual bool Load(const String &path, SkinnedMesh *mesh) = 0;

		/*
		*/
		virtual const char* GetExt() = 0;
	};
}