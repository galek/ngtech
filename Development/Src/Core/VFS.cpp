/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    ALSystem.cpp
* Desc:    OpenAL sound system.
* Version: 1.0
* Author:  George Evmenov <altren@mygui.info> , Nick Galko <galek@vegaengine.com>
*
* This file is part of the Vega engine (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#include "../EnginePrivate.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include <fstream>
#include "../Common/IDataManager.h"
#include "VFS.h"



namespace VEGA
{
	using namespace Common;
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
		//Nick:TODO:тут надо этот класс перенести из mygui
		using namespace MyGUI;
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
		common::VectorWString wresult;
		result.clear();

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_pattern).asWStr(), false);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		return result;
	}
	//Nick:TODO:Rewrite

	const std::string& FileSystem::getDataPath(const std::string& _name)
	{
		static std::string path;
		VectorString result;
		common::VectorWString wresult;

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_name).asWStr(), true);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		path = result.size() == 1 ? result[0] : "";
		return path;
	}
	//Nick:TODO:Rewrite
	void FileSystem::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = MyGUI::UString(_name).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}
} // namespace MyGUI
