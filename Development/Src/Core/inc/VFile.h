#pragma once

namespace NGTech
{
	class CORE_API VFile
	{
	public:
		VFile(const char* _name);
		~VFile();

		bool IsDataExist(const char*);
		bool EndOfFile();
		const char *LoadFile(const char* _name);
		const char* GetDataPath(const char* _name);
		String GetLine();
		String GetFileExt();
	private:
		FILE *mFile;
		String mName;
	};
}