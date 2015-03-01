/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
//**************************************

namespace NGTech
{
	class I_MeshFormatSkinned;
	class SkinnedMesh;
	/**
	Mesh loader class
	*/
	class SkinnedMeshLoader
	{
	public:
		/*
		*/
		SkinnedMeshLoader();

		/*
		*/
		~SkinnedMeshLoader();

		/*
		*/
		void Save(const String &path, SkinnedMesh *mesh);

		/*
		*/
		bool Load(const String &path, SkinnedMesh *mesh);

		/*
		*/
		void RegisterFormat(I_MeshFormatSkinned *format);


	protected:
		std::vector<I_MeshFormatSkinned*> formats;
	};

}
