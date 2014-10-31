/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    ALSystem.cpp
* Desc:    OpenAL sound system.
* Version: 1.0
* Author:  George Evmenov <altren@mygui.info> , Nick Galko <galek@vegaengine.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://vegaengine.com/
*/
#include "CorePrivate.h"

#include "DataFileStream.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include <fstream>
#include "../../Common/IDataManager.h"
#include "VFS.h"
#include "UString.h"



namespace NGTech
{
	FileSystem::FileSystem() :
		mIsInitialise(false)
	{
	}

	void FileSystem::initialise()	{
		mIsInitialise = true;
	}

	void FileSystem::shutdown()	{
		mIsInitialise = false;
	}

	IDataStream* FileSystem::getData(const std::string& _name)
	{
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		std::ifstream* stream = new std::ifstream();
		stream->open(filepath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(stream);

		return data;
	}

	bool FileSystem::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return files.size() == 1;
	}
	//TODO:Rewrite

	const VectorString& FileSystem::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		VectorWString wresult;
		result.clear();

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
			scanFolder(wresult, (*item).name, (*item).recursive, NGTech::UString(_pattern).asWStr(), false);

		for (VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
			result.push_back(NGTech::UString(*item).asUTF8());
		return result;
	}
	//Nick:TODO:Rewrite

	const std::string& FileSystem::getDataPath(const std::string& _name)
	{
		static std::string path;
		VectorString result;
		VectorWString wresult;

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
			scanFolder(wresult, (*item).name, (*item).recursive, NGTech::UString(_name).asWStr(), true);

		for (VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
			result.push_back(NGTech::UString(*item).asUTF8());

		//	path = result.size() == 1 ? result[0] : "";
		if (!result.empty())
		{
			path = result[0];
			if (result.size() > 1)
			{
				Warning("There are several files with name '", _name, "'. '", path, "' was used.");
				Warning("Other candidater are:");
				for (size_t index = 1; index < result.size(); index++)
					Warning(" - '", result[index], "'");
			}
		}
		return path;
	}

	void FileSystem::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = NGTech::UString(_name).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

	void FileSystem::freeData(IDataStream* _data)
	{
		delete _data;
	}
} // namespace NGTech