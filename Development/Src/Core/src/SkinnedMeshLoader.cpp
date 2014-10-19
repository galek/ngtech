//**************************************
#include "CorePrivate.h"
//**************************************
#include "SkinnedMeshLoader.h"
#include "SkinnedMeshFormat.h"
#include "SkinnedMesh.h"
//**************************************

namespace NGTech
{
	/*
	*/
	SkinnedMeshLoader::SkinnedMeshLoader()
	{
	}

	/*
	*/
	SkinnedMeshLoader::~SkinnedMeshLoader()
	{
		for (int i = 0; i < formats.size(); i++)
		{
			SAFE_DELETE(formats[i]);
		}
		formats.clear();
	}

	/*
	*/
	void SkinnedMeshLoader::RegisterFormat(SkinnedMeshFormat *format)	{
		formats.push_back(format);
	}

	/*
	*/
	void SkinnedMeshLoader::Save(const String &path, SkinnedMesh *mesh)
	{
	}

	/*
	*/
	void SkinnedMeshLoader::Load(const String &path, SkinnedMesh *mesh)
	{
		VFile file(path.c_str());
		auto ext = file.GetFileExt();

		for (int i = 0; i < formats.size(); i++)
		{
			if (formats[i]->GetExt() == ext)
			{
				formats[i]->Load(path, mesh);
				return;
			}
		}
		Error("SkinnedMeshLoader::Load() error: format is not supported", true);
	}

}