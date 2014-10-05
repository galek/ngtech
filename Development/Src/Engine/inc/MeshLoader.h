#pragma once


namespace NGTech
{
	class Model;
	class MeshFormat;

	/**
	Mesh loader class
	*/
	class MeshLoader
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
		void RegisterFormat(MeshFormat *format);


	protected:
		std::vector<MeshFormat*> formats;
	};

}
