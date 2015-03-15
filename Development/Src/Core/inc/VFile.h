#pragma once

namespace NGTech
{
	class CORE_API VFile
	{
	public:
		enum {
			READ_BIN,
			READ_TEXT,
			WRITE_BIN,
		};
	public:
		explicit VFile(const String &_name, int _mode = READ_BIN, bool _notSearch = false);
		~VFile();

		bool IsDataExist();
		bool IsEof();

		char* LoadFile();

		const char* GetDataPath();

		size_t Size();
		size_t FTell();
		size_t FSeek(long offset, int mode);

		String GetLine();

		String GetFileExt();
		String CutFileExt();

		//Revert 0 if all is normal
		int FClose();
		void Read(void *buf, int size, int count);
		void WriteString(const String &text);
		void Write(void *buf, int size, int count);
		void ScanF(const char * format, ...);

		ENGINE_INLINE FILE* GetLowLevelFile() { return mFile; }
		ENGINE_INLINE String GetName() { return mName; }
		bool IsValid();
	private:
		void _OpenFile(const String&path, int _mode, bool _notSearch);
	private:
		FILE *mFile;
		char *memoryBuffer;
		String mName;
		size_t mSize;
		size_t mCurrentPos;
	};
}