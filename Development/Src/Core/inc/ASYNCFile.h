#pragma once

namespace NGTech
{
	struct CORE_API ASYNCFILE
	{
		static HANDLE CreateTempFile(std::string &WritePath, std::string &FileName);
		unsigned int FileSeek(unsigned int distance, DWORD MoveMethod);
		unsigned int FileTell();
		bool FileRead(void *pBuffer, unsigned int size);
		bool FileWrite(void *pBuffer, unsigned int size);
	public:
		static unsigned int FileSeek(HANDLE hFile, unsigned int distance, DWORD MoveMethod);
		static unsigned int FileTell(HANDLE hFile);
		static bool FileRead(HANDLE hFile, void *pBuffer, unsigned int size);
		static bool FileWrite(HANDLE hFile, void *pBuffer, unsigned int size);
	private:
		HANDLE fileHandle;
	};
}