#pragma once

namespace NGTech
{
	class CORE_API VFile
	{
	public:
		enum {
			READ_BIN,
			READ_TEXT
		};
	public:
		explicit VFile(const char* _name, int _mode = READ_BIN, bool _notSearch = false);
		~VFile();

		bool IsDataExist();
		bool EndOfFile();
		char* LoadFile();
		const char* GetDataPath();
		size_t Size();
		String GetLine();
		String GetFileExt();
		String CutFileExt();
		void WriteString(const String &text);
		void ScanF(const char * format, ...);
	private:
		void _OpenFile(const String&path, int _mode, bool _notSearch);
	private:
		FILE *mFile;
		String mName;
		size_t mSize;
	};
}