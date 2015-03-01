#pragma once


namespace NGTech
{
	class Model;
	class I_MeshFormat;

	/**
	Mesh loader class
	*/
	class CORE_API MeshLoader
	{
	public:
		/*
		*/
		MeshLoader();

		/*
		*/
		~MeshLoader();
		
		/*
		*/
		void Save(const String &path, Model *mesh);

		/*
		*/
		void Load(const String &path, Model *mesh);

		/*
		*/
		void RegisterFormat(I_MeshFormat *format);


	protected:
		std::vector<I_MeshFormat*> formats;
	};

}
