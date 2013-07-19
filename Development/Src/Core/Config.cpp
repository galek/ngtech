/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "CorePrivate.h"
//**************************************
#include "Config.h"
#include "File.h"
#include "Log.h"
#include "Error.h"
//**************************************

namespace VEGA {


	//---------------------------------------------------------------------------
	//Desc:    creates new Config
	//Params:  -
	//Returns: pointer to the new Config
	//---------------------------------------------------------------------------
	Config::Config(const String &path) {

		if(!FileHelper::fileExist(path)) 
			Error::showAndExit("Config::create() error: config file '" + path + "' not found");


		FILE *cFile = fopen(path.c_str(), "rt");

		while(!feof(cFile)) {
			String line = FileHelper::readString(cFile);
			if(line[0] == '#' || StringHelper::getWordsNumber(line) < 3) { 
				continue; 
			}

			ConfigVar cv;
			cv.name = StringHelper::getWord(line, 1);
			cv.value = StringHelper::getWord(line, 3);

			vars.push_back(cv);
		}

		fclose(cFile);
	}


	//---------------------------------------------------------------------------
	//Desc:    saves all vars to file
	//Params:  path - file name
	//Returns: -
	//---------------------------------------------------------------------------
	void Config::save(const String &path) {
		FILE *cFile = fopen(path.c_str(), "wt");

		for(int i = 0; i < vars.size(); i++) {
			String line = vars[i].name + " = " + vars[i].value + "\n";
			fprintf(cFile, "%s", line.c_str());
		}

		fclose(cFile);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets param by name
	//Params:  name - parameter name
	//Returns: parameter value
	//---------------------------------------------------------------------------
	float Config::getFloat(const String &name) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name)
				return StringHelper::toFloat(vars[i].value);
		}
		return 0;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets param by name
	//Params:  name - parameter name
	//Returns: parameter value
	//---------------------------------------------------------------------------
	int Config::getInt(const String &name) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name)
				return StringHelper::toInt(vars[i].value);
		}
		return 0;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets param by name
	//Params:  name - parameter name
	//Returns: parameter value
	//---------------------------------------------------------------------------
	bool Config::getBool(const String &name) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name)
				return StringHelper::toInt(vars[i].value);
		}
		return false;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets param by name
	//Params:  name - parameter name, value - new value
	//Returns: parameter value
	//---------------------------------------------------------------------------
	void Config::setFloat(const String &name, float value) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name) {
				vars[i].value = StringHelper::fromFloat(value);
				return;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets param by name
	//Params:  name - parameter name, value - new value
	//Returns: parameter value
	//---------------------------------------------------------------------------
	void Config::setInt(const String &name, int value) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name) {
				vars[i].value = StringHelper::fromInt(value);
				return;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets param by name
	//Params:  name - parameter name, value - new value
	//Returns: parameter value
	//---------------------------------------------------------------------------
	void Config::setBool(const String &name, bool value) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name) {
				vars[i].value = StringHelper::fromInt(value);
				return;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets param by name
	//Params:  name - parameter name, value - new value
	//Returns: parameter value
	//---------------------------------------------------------------------------
	void Config::setString(const String &name, const String &value) {
		for(int i = 0; i < vars.size(); i++) {
			if(vars[i].name == name) {
				vars[i].value = value;
				return;
			}
		}
	}

}