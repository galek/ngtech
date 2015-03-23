/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    VFile.cpp
* Desc:    Virtual file impl.
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
//***************************************************************************
#include "CorePrivate.h"
//***************************************************************************
#include <iostream>
#include <fstream>
#include <stdarg.h>
//***************************************************************************
#include "string.h"//memset for gcc
//***************************************************************************
#include "VFS.h"
#include "FileHelper.h"
//***************************************************************************
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#else
#include <unistd.h>
#endif
//***************************************************************************

namespace NGTech
{

	/**
	*/
	VFile::VFile(const String & _name, int _mode, bool _notSearch)
		:mName(_name), mCurrentPos(0), mSize(0), memoryBuffer(nullptr), mFile(nullptr)
	{
		DebugM("Loading/Search file with name: %s", _name.c_str());
		_OpenFile(_name, _mode, _notSearch);
	}

	/**
	*/
	VFile::~VFile(){
		if (mFile) fclose(mFile);
		SAFE_DELETE_ARRAY(memoryBuffer);
		mSize = 0;
		mCurrentPos = 0;
	}

	/**
	*/
	bool VFile::IsDataExist(bool _warn) {
		bool status = GetVFS()->isDataExist(mName);
		if ((!status) && _warn)
			Warning("File %s :is not exist", mName.c_str());
		return status;
	}

	/**
	*/
	bool VFile::IsDataExist(const char* _name,bool _warn) {
		bool status = GetVFS()->isDataExist(_name);
		if ((!status) && _warn)
			Warning("File %s :is not exist", _name);
		return status;
	}

	/**
	*/
	char* VFile::LoadFile()
	{
		if (!IsDataExist())
			return nullptr;

		//PROFILER_START(VFile::LoadFile);

		// obtain file size:
		fseek(mFile, 0, SEEK_END);
		mSize = ftell(mFile);
		rewind(mFile);

		// allocate memory to contain the whole file:
		memoryBuffer = new char[sizeof(char)*mSize];
		fseek(mFile, 0, SEEK_SET);
		fread(memoryBuffer, sizeof(char), mSize, mFile);
		memoryBuffer[mSize] = '\0';

		//PROFILER_END();

		return memoryBuffer;
	}

	/**
	*/
	size_t VFile::FTell(){
		if (mFile)
			return ftell(mFile);
		return -1;
	}

	/**
	*/
	size_t VFile::FSeek(long offset, int mode)
	{
		if (mFile)
			return fseek(mFile, offset, mode);
		return 1;
	}

	/**
	*/
	void VFile::Read(void *buf, int size, int count)
	{
		if (mFile)
			fread(buf, size, count, mFile);

		if (memoryBuffer)
		{
			memcpy((char*)buf, (char*)(memoryBuffer)+mCurrentPos, size*count);
			mCurrentPos += size*count;
		}
	}

	/**
	*/
	bool VFile::IsEof()	{
		if (mFile)
			return feof(mFile) != 0;

		if (memoryBuffer)
			return (mCurrentPos > mSize);

		return true;
	}

	/**
	*/
	void VFile::Write(void *buf, int size, int count)
	{
		if (mFile)
		{
			fwrite(buf, size, count, mFile);
		}

		if (memoryBuffer)
		{
			memcpy((char*)(memoryBuffer)+mCurrentPos, (char*)buf, size*count);
			mCurrentPos += size*count;
		}
	}

	/**
	*/
	const char*  VFile::GetDataPath() {
		return GetVFS()->getDataPath(mName).c_str();
	}

	/**
	*/
	size_t VFile::Size()
	{
		if (mSize == 0)
		{
			DebugM("[FileSystem] loading file with zero size.All is correctly? Filename: %s", mName.c_str());
			LoadFile();
		}

		return mSize;
	}

	/**
	*/
	String VFile::GetLine()
	{
		if (!mFile) return "";
		if (IsEof()) return "";

		String output = "";
		unsigned char h = fgetc(mFile);

		while ((h != '\n') && (!feof(mFile))) {
			output += h;
			h = fgetc(mFile);
		}
		return output;
	}

	/**
	*/
	String VFile::GetFileExt() {
		if (mName.size() == 0)
			return "UNKNOWN_EXT";

		int i = (int)mName.size() - 1;
		String buf;

		while ((mName[i] != '.') && (i > 0)) {
			buf = mName[i] + buf;
			i--;
		}
		return buf;
	}

	/**
	*/
	void VFile::WriteString(const String &text) {
		if (mFile)
			fprintf(mFile, "%s\n", text.c_str());
	}

	/**
	*/
	String VFile::CutFileExt() {
		if (mName.size() == 0)
			return "UNKNOWN_EXT";

		String buf;

		int i = 0;
		while ((mName[i] != '.') && (i < (int)mName.size()))	{
			buf += mName[i];
			i++;
		}
		return buf;
	}

	/**
	*/
	void VFile::_OpenFile(const String&path, int _mode, bool _notSearch)
	{
		if (_notSearch)
		{
			if (_mode == READ_TEXT)
				mFile = fopen(path.c_str(), "rt");
			else if (_mode == READ_BIN)
				mFile = fopen(path.c_str(), "rb");
		}
		else{
			if (!IsDataExist())
				return;
			if (_mode == READ_TEXT)
				mFile = fopen(GetDataPath(), "rt");
			else if (_mode == READ_BIN)
				mFile = fopen(GetDataPath(), "rb");
			else if (_mode == WRITE_BIN)
				mFile = fopen(GetDataPath(), "wb");
		}
	}

	/**
	*/
	void VFile::ScanF(const char *format, ...)
	{
		if (mFile)
		{
			va_list ap;
			va_start(ap, format);
			vfscanf(mFile, format, ap);
			va_end(ap);
		}
	}

	/**
	*/
	int VFile::FClose()
	{
		if (mFile)
			return fclose(mFile);

		return 1;
	}

	/**
	*/
	bool VFile::IsValid()
	{
		return
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
			!(_access(mName.c_str(), 00) != -1);
#else
			!(access(mName.c_str(), F_OK) != -1);
#endif
	}
}