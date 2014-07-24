#include "CorePrivate.h"
#include "VFS.h"
#include "FileHelper.h"

namespace NGTech
{
	VFile::VFile(const char* _name){
		if (!IsDataExist(_name))
			return;
		mFile = fopen(GetDataPath(_name), "rt");
	}
	
	VFile::~VFile(){
		fclose(mFile);
	}

	bool VFile::IsDataExist(const char* _name){ return GetVFS()->isDataExist(_name); }
	
	const char * VFile::LoadFile(const char* _name){
		String buff;
		String line;
		while (!EndOfFile()){
			line = FileHelper::readString(mFile);
			buff += line;
		}
		return buff.c_str();
	}
	
	bool VFile::EndOfFile()
	{
		return feof(mFile);
	}
	
	const char*  VFile::GetDataPath(const char* _name)
	{
		return GetVFS()->getDataPath(_name).c_str();
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
}