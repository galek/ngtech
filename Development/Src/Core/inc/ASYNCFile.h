#pragma once

#if PLATFORM_OS == PLATFORM_OS_WINDOWS

namespace NGTech
{
	struct CORE_API ASYNCFILE
	{
		static void *  CreateTempFile(std::string &WritePath, std::string &FileName);
		unsigned int FileSeek(unsigned int distance, unsigned long MoveMethod);
		unsigned int FileTell();
		bool FileRead(void *pBuffer, unsigned int size);
		bool FileWrite(void *pBuffer, unsigned int size);
	public:
		static unsigned int FileSeek(void *  hFile, unsigned int distance, unsigned long MoveMethod);
		static unsigned int FileTell(void *  hFile);
		static bool FileRead(void *  hFile, void *pBuffer, unsigned int size);
		static bool FileWrite(void *  hFile, void *pBuffer, unsigned int size);
	private:
		void *  fileHandle;
	};
}
#endif