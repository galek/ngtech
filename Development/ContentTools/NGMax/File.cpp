/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

//**************************************
#include "File.h"
#include <cstdlib>
#include <stdarg.h>
#include <vector>
//**************************************

/*
*/
File::File(const String &path, Mode mode)
{
	memoryBuffer = NULL;
	this->mode = mode;

	if (mode == READ_TEXT) file = fopen(path.c_str(), "rt");
	if (mode == WRITE_TEXT) file = fopen(path.c_str(), "wt");
	if (mode == APPEND_TEXT) file = fopen(path.c_str(), "a+t");

	if (mode == READ_BINARY) file = fopen(path.c_str(), "rb");
	if (mode == WRITE_BINARY) file = fopen(path.c_str(), "wb");
	if (mode == APPEND_BINARY) file = fopen(path.c_str(), "a+b");
}

/*
*/
File::File(void *data, int size)
{
	file = NULL;
	this->size = size;
	memoryBuffer = new char[size];
	currentPos = 0;

	memcpy(memoryBuffer, data, size);
}

/*
*/
File::~File()
{
	Close();
}

/*
*/
bool File::IsOpened()
{
	return file != NULL;
}

/*
*/
void File::ToMemory()
{
	if (!file)
		return;

	if (memoryBuffer)
		return;

	if (mode < 3)
		return;

	fseek(file, 0, SEEK_END);
	size = ftell(file);

	memoryBuffer = new char[size + 1];
	((char*)memoryBuffer)[size] = '\0';

	fseek(file, 0, SEEK_SET);
	fread(memoryBuffer, 1, size, file);
	fclose(file);
	
	file = NULL;
	currentPos = 0;
}


/*
*/
void File::Close()
{
	if (file) fclose(file);
	if (memoryBuffer) delete [] memoryBuffer;
	size = 0;
	currentPos = 0;
}

/*
*/
bool File::Eof()
{
	if (file)
	{
		return feof(file) != 0;
	}

	if (memoryBuffer)
	{
		return (currentPos > size);
	}
	return true;
}

/*
*/
char File::Getc()
{
	if (file)
	{
		if (feof(file))
		{
			return '\0';
		}
		return fgetc(file);
	}

	if (memoryBuffer)
	{
		if (Eof())
		{
			return '\0';
		}

		char c;
		c = *(memoryBuffer + currentPos);
		currentPos += 1;
		return c;
	}
	
	return '\0';
}

/*
*/
String File::Gets()
{
	String output = "";
	
	char h = Getc();
	
	while ((h != '\n') && !Eof())
	{
		if  (h != 13)
		{
			output += h;
		}

		h = Getc();

		if  (h == 9)
		{
			h=' ';
		}
	}
	return output;
}

/*
*/
void File::Read(void *buf, int _size, int count)
{
	if (file)
	{
		fread(buf, _size, count, file);
	}

	if (memoryBuffer)
	{
		memcpy((char*)buf, memoryBuffer + currentPos, _size*count);
		currentPos += _size*count;
	}
}

/*
*/
void File::Write(void *buf, int _size, int count)
{
	if (file)
	{
		fwrite(buf, _size, count, file);
	}

	if (memoryBuffer)
	{
		memcpy(memoryBuffer + currentPos, (char*)buf, _size*count);
		currentPos += _size*count;
	}
}

/*
*/
void File::Printf(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	if (file)
	{
		vfprintf(file, format, arg);
	}

	if  (memoryBuffer)
	{
		vsprintf((char*)memoryBuffer + currentPos, format, arg);
	}
		
	va_end(arg);
}

/*
*/
void *File::GetData()
{
	return memoryBuffer;
}

/*
*/
unsigned int File::GetSize()
{
	return size;
}


