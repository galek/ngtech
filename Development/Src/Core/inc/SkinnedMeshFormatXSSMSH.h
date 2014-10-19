/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "SkinnedMeshFormat.h"
//**************************************

namespace NGTech
{
	class SkinnedMesh;
	/**
	SkinnedMesh format XSSMSH
	*/
	class SkinnedMeshFormatXSSMSH : public SkinnedMeshFormat
	{
	public:
		/*
		*/
		SkinnedMeshFormatXSSMSH();

		/*
		*/
		virtual ~SkinnedMeshFormatXSSMSH();

		/*
		*/
		virtual void Save(const String &path, SkinnedMesh *mesh);

		/*
		*/
		virtual void Load(const String &path, SkinnedMesh *mesh);

		/*
		*/
		virtual String GetExt()
		{
			return "xssmsh";
		}
	};

}
