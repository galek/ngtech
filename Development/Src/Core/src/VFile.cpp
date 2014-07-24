#include "CorePrivate.h"
#include "VFS.h"
#include "FileHelper.h"

namespace NGTech
{
	VFile::VFile(const char* _name, int _mode, bool _notSearch)
		:mName(_name)
	{
		DebugM("Loading/Search file with name: %s", _name);
		_OpenFile(_name, _mode, _notSearch);
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
			line = GetLine();
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

	void VFile::WriteString(const String &text) {
		fprintf(mFile, "%s\n", text.c_str());
	}

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
}