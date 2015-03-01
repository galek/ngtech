#pragma once

//**************************************
#include "I_MeshFormat.h"
//**************************************

namespace NGTech
{

	/**
	Mesh format NGGF
	*/
	class MeshFormatNGGFStatic : public I_MeshFormat
	{
	public:
		/*
		*/
		MeshFormatNGGFStatic(){}

		/*
		*/
		virtual ~MeshFormatNGGFStatic(){}

		/*
		*/
		virtual void Save(const std::string &path, Model *mesh);

		/*
		*/
		virtual void Load(const std::string &path, Model *mesh);

		/*
		*/
		ENGINE_INLINE virtual const char* GetExt() { return "nggf"; }
	};

}
