/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "CorePrivate.h"
//**************************************
#include "File.h"
//**************************************

namespace VEGA {

#define UNKNOWN_EXT "UNKNOWN_EXT"
	//---------------------------------------------------------------------------
	//Desc:    reads the String from text file
	//Params:  file - pointer to file to read
	//Returns: read String
	//---------------------------------------------------------------------------
	String FileHelper::readString(FILE *file) {
		if(file == NULL) return "";
		if(feof(file)) return "";

		String output = "";
		unsigned char h = fgetc(file);

		while((h != '\n') && (!feof(file))) {
			output += h;
			h = fgetc(file);
			//if(h == 9) h = ' ';
		}
		return output;
	}

	//---------------------------------------------------------------------------
	//Desc:    writes the String to text file
	//Params:  file - pointer to file to write, text - text to write
	//Returns: -
	//---------------------------------------------------------------------------
	void FileHelper::writeString(FILE *file, const String &text) {
		fprintf(file, "%s\n", text.c_str());
	}

	//---------------------------------------------------------------------------
	//Desc:    cuts the fileName`s extensions
	//Params:  fName - file name
	//Returns: fName without extension
	//---------------------------------------------------------------------------
	String FileHelper::cutFileExt(const String &fName) {
		if(fName.size() == 0) 
			return UNKNOWN_EXT;

		String buf;

		int i = 0;
		while((fName[i] != '.') && (i < (int)fName.size()))	{
			buf += fName[i];
			i++;
		}
		return buf;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets the fileName`s extensions
	//Params:  fName - file name
	//Returns: fName`s extension
	//---------------------------------------------------------------------------
	String FileHelper::getFileExt(const String &fName) {
		if(fName.size() == 0) 
			return UNKNOWN_EXT;

		int i = (int)fName.size() - 1;
		String buf;

		while((fName[i] != '.') && (i > 0)) {
			buf = fName[i] + buf;
			i--;
		}
		return buf;
	}

	//---------------------------------------------------------------------------
	//Desc:    checks if the file exists
	//Params:  path - file name
	//Returns: true if exists
	//---------------------------------------------------------------------------
	bool FileHelper::fileExist(const String &path) {
		FILE* testFile;
		testFile = fopen(path.c_str(), "r");
		return (testFile != NULL);
	}

}

