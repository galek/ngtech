#include "CorePrivate.h"

#include "VFS.h"
#include "FileHelper.h"
#include <iostream>
#include <fstream>
#include <stdarg.h>

#define SAFE_DELETE_ARRAY

namespace NGTech
{

	/**
	*/
	VFile::VFile(const char* _name, int _mode, bool _notSearch)
		:mName(_name), mCurrentPos(0), mSize(0), memoryBuffer(nullptr)
	{
		DebugM("Loading/Search file with name: %s", _name);
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
	bool VFile::IsDataExist() {
		bool status = GetVFS()->isDataExist(mName);
		if (!status)
			Warning("File %s :is not exist", mName);
		return status;
	}

	/**
	*/
	char* VFile::LoadFile(){

		PROFILER_START(VFile::LoadFile);

		// obtain file size:
		fseek(mFile, 0, SEEK_END);
		mSize = ftell(mFile);
		rewind(mFile);

		// allocate memory to contain the whole file:
		memoryBuffer = new char[sizeof(char)*mSize];
		fseek(mFile, 0, SEEK_SET);
		fread(memoryBuffer, sizeof(char), mSize, mFile);
		memoryBuffer[mSize] = '\0';

		PROFILER_END();

		return memoryBuffer;
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
	bool VFile::EndOfFile()	{
		if (mFile)
		{
			return feof(mFile) != 0;
		}

		if (memoryBuffer)
		{
			return (mCurrentPos > mSize);
		}
		return true;
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
		if (EndOfFile()) return "";

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
		}//»щем
		else{
			if (!IsDataExist())
				return;
			if (_mode == READ_TEXT)
				mFile = fopen(GetDataPath(), "rt");
			else if (_mode == READ_BIN)
				mFile = fopen(GetDataPath(), "rb");
		}
	}

	/**
	*/
	void VFile::ScanF(const char *format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vfscanf(mFile, format, ap);
		va_end(ap);
	}

	/**
	*/
}