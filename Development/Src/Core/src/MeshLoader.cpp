//**************************************
#include "CorePrivate.h"
//**************************************
#include "MeshLoader.h"
#include "I_MeshFormat.h"
//**************************************

namespace NGTech
{
	/*
	*/
	MeshLoader::MeshLoader()
	{
	}

	/*
	*/
	MeshLoader::~MeshLoader()
	{
		for (int i = 0; i < formats.size(); i++)
		{
			SAFE_DELETE(formats[i]);
		}
		formats.clear();
	}

	/*
	*/
	void MeshLoader::RegisterFormat(I_MeshFormat *format)
	{
		formats.push_back(format);
	}

	/*
	*/
	void MeshLoader::Save(const String &path, Model *mesh)
	{
	
	}

	/*
	*/
	bool MeshLoader::Load(const String &path, Model *mesh)
	{
		VFile file(path.c_str());
		auto ext = file.GetFileExt();

		for (size_t i = 0; i < formats.size(); i++)
		{
			if (formats[i]->GetExt() == ext)
			{
				formats[i]->Load(path, mesh);
				return true;
			}
		}
		Warning("MeshLoader::Load() error: format is not supported");
		return false;
	}

}