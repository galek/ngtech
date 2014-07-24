#include "CorePrivate.h"
#include "VFS.h"
#include "FileHelper.h"

namespace NGTech
{
	VFile::VFile(const char* _name,int _mode)
		:mName(_name)
	{
		DebugM("Loading/Search file with name: %s", _name);
		if (!IsDataExist())
			return;
		if (_mode = READ_TEXT)
			mFile = fopen(GetDataPath(), "rt");
		else if (_mode = READ_BIN)
			mFile = fopen(GetDataPath(), "rb");
	}

	VFile::~VFile(){
		fclose(mFile);
	}

	bool VFile::IsDataExist() {
		bool status = GetVFS()->isDataExist(mName);
		if (!status)
			Warning("File %s :is not exist", mName);
		return status;
	}

	const char * VFile::LoadFile(){
		String buff;
		String line;
		while (!EndOfFile()){
			line = FileHelper::readString(mFile);
			buff += line;
		}
		mSize = buff.size();
		return buff.c_str();
	}

	bool VFile::EndOfFile()
	{
		return feof(mFile);
	}

	const char*  VFile::GetDataPath()
	{
		return GetVFS()->getDataPath(mName).c_str();
	}

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
}