/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
//#include "../Include/CompileConfig.h"
//#include "../Include/String.h"
//#include "../Include/MathLib.h"
//#include "../Include/SkinnedMesh.h"
//**************************************

namespace NGTech
{
	class SkinnedMesh;
	/**
	Skinned Mesh loader class
	*/
	class SkinnedMeshFormat
	{
	public:
		virtual ~SkinnedMeshFormat()
		{
		}

		/*
		*/
		virtual void Save(const String &path, SkinnedMesh *mesh) = 0;

		/*
		*/
		virtual void Load(const String &path, SkinnedMesh *mesh) = 0;

		/*
		*/
		virtual String GetExt() = 0;
	};

}
