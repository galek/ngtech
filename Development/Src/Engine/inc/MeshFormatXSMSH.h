#pragma once

//**************************************
#include "MeshFormat.h"
//**************************************

namespace NGTech
{

	/**
	Mesh format XSMSH
	*/
	class MeshFormatXSMSH : public MeshFormat
	{
	public:
		/*
		*/
		MeshFormatXSMSH();

		/*
		*/
		virtual ~MeshFormatXSMSH();

		/*
		*/
		virtual void Save(const std::string &path, Model *mesh);

		/*
		*/
		virtual void Load(const std::string &path, Model *mesh);

		/*
		*/
		virtual std::string GetExt()
		{
			return "nggf";
		}
	};

}
