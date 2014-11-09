/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#pragma once

//**************************************
#include "String.h"
#include <stdio.h>
//**************************************

/**
File functionality
*/
class File
{
public:
	/**
	Open mode
	*/
	enum Mode
	{
		READ_TEXT,
		WRITE_TEXT,
		APPEND_TEXT,

		READ_BINARY,
		WRITE_BINARY,
		APPEND_BINARY,
	};

	/**
	Creates new File
	*/
	File(const String &path, Mode mode);

	/**
	Creates new File
	*/
	File(void *data, int size);

	/**
	Destroys File
	*/
	~File();

	/**
	false if FILE is NULL
	*/
	bool IsOpened();

	/**
	Moves file to memory
	*/
	void ToMemory();

	/**
	Close file
	*/
	void Close();

	/**
	feof analog
	*/
	bool Eof();

	/**
	getc analog
	*/
	char Getc();

	/**
	gets analog
	*/
	String Gets();

	/**
	fread analog
	*/
	void Read(void *buf, int size, int count);

	/**
	file->Write analog
	*/
	void Write(void *buf, int size, int count);

	/**
	printf analog
	*/
	void Printf(const char *format, ...);

	/**
	return file data
	*/
	void *GetData();

	/**
	return file size
	*/
	unsigned int GetSize();

private:
	FILE *file;

	char *memoryBuffer;
	unsigned int currentPos;
	unsigned int size;
	Mode mode;

	friend class FileSystem;
};
