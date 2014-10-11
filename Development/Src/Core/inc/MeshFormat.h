#pragma once

#include <string>

namespace NGTech
{
	class Model;
	/**
	Mesh loader class
	*/
	class MeshFormat
	{
	public:
		virtual ~MeshFormat()
		{
		}

		/*
		*/
		virtual void Save(const std::string &path, Model *mesh) = 0;

		/*
		*/
		virtual void Load(const std::string &path, Model *mesh) = 0;

		/*
		*/
		virtual std::string GetExt() = 0;
	};

}
