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
	class SkinnedMeshFormat;
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
		void Load(const String &path, SkinnedMesh *mesh);

		/*
		*/
		void RegisterFormat(SkinnedMeshFormat *format);


	protected:
		std::vector<SkinnedMeshFormat*> formats;
	};

}
