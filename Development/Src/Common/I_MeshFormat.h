#pragma once

#include <string>

namespace NGTech
{
	class Model;
	/**
	Mesh loader class
	*/
	class I_MeshFormat
	{
	public:
		virtual ~I_MeshFormat()	{}

		/*
		*/
		virtual void Save(const std::string &path, Model *mesh) = 0;

		/*
		*/
		virtual void Load(const std::string &path, Model *mesh) = 0;

		/*
		*/
		virtual const char* GetExt() = 0;
	};

}
