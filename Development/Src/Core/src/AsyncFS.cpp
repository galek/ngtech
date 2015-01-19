#include "CorePrivate.h"
#include "ASYNCFILE.h"
#include "strtools.h"

namespace NGTech
{


	HANDLE ASYNCFILE::CreateTempFile(std::string &WritePath, std::string &FileName)
	{
		char tempFileName[MAX_PATH];
		if (WritePath.empty())
		{
			// use a safe name in the cwd
			char *pBuffer = tmpnam(NULL);
			if (!pBuffer)
			{
				return INVALID_HANDLE_VALUE;
			}
			if (pBuffer[0] == '\\')
			{
				pBuffer++;
			}
			if (pBuffer[strlen(pBuffer) - 1] == '.')
			{
				pBuffer[strlen(pBuffer) - 1] = '\0';
			}
			V_snprintf(tempFileName, sizeof(tempFileName), "_%s.tmp", pBuffer);
		}
		else
		{
			// generate safe name at the desired prefix
			char uniqueFilename[MAX_PATH];
			SYSTEMTIME sysTime;                                                       \
				GetLocalTime(&sysTime);
			sprintf(uniqueFilename, "%d_%d_%d_%d_%d.tmp", sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);                                                \
				V_ComposeFileName(WritePath.c_str(), uniqueFilename, tempFileName, sizeof(tempFileName));
		}

		FileName = tempFileName;
		HANDLE hFile = CreateFile(tempFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		return hFile;
	}

	unsigned int ASYNCFILE::FileSeek(HANDLE hFile, unsigned int distance, DWORD MoveMethod)
	{
		LARGE_INTEGER li;

		li.QuadPart = distance;
		li.LowPart = SetFilePointer(hFile, li.LowPart, &li.HighPart, MoveMethod);
		if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		{
			li.QuadPart = -1;
		}

		return (unsigned int)li.QuadPart;
	}

	unsigned int ASYNCFILE::FileTell(HANDLE hFile)
	{
		return FileSeek(hFile, 0, FILE_CURRENT);
	}

	bool ASYNCFILE::FileRead(HANDLE hFile, void *pBuffer, unsigned int size)
	{
		DWORD numBytesRead;
		BOOL bSuccess = ::ReadFile(hFile, pBuffer, size, &numBytesRead, NULL);
		return bSuccess && (numBytesRead == size);
	}

	bool ASYNCFILE::FileWrite(HANDLE hFile, void *pBuffer, unsigned int size)
	{
		DWORD numBytesWritten;
		BOOL bSuccess = WriteFile(hFile, pBuffer, size, &numBytesWritten, NULL);
		return bSuccess && (numBytesWritten == size);
	}

	unsigned int ASYNCFILE::FileSeek(unsigned int distance, DWORD MoveMethod)
	{
		LARGE_INTEGER li;

		li.QuadPart = distance;
		li.LowPart = SetFilePointer(fileHandle, li.LowPart, &li.HighPart, MoveMethod);
		if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		{
			li.QuadPart = -1;
		}

		return (unsigned int)li.QuadPart;
	}

	unsigned int ASYNCFILE::FileTell()
	{
		return FileSeek(fileHandle, 0, FILE_CURRENT);
	}

	bool ASYNCFILE::FileRead(void *pBuffer, unsigned int size)
	{
		DWORD numBytesRead;
		BOOL bSuccess = ::ReadFile(fileHandle, pBuffer, size, &numBytesRead, NULL);
		return bSuccess && (numBytesRead == size);
	}

	bool ASYNCFILE::FileWrite(void *pBuffer, unsigned int size)
	{
		DWORD numBytesWritten;
		BOOL bSuccess = WriteFile(fileHandle, pBuffer, size, &numBytesWritten, NULL);
		return bSuccess && (numBytesWritten == size);
	}
}