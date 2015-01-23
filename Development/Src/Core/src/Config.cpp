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
#include "FileHelper.h"
#include "Log.h"
#include "Error.h"
//**************************************

namespace NGTech {

	Config::Config(const String &path) {
		VFile file(path.c_str(), VFile::READ_TEXT, true);

		while (!file.IsEof()) {
			String line = file.GetLine();
			if (line[0] == '#' || StringHelper::getWordsNumber(line) < 3) {
				continue;
			}

			ConfigVar cv;
			cv.name = StringHelper::getWord(line, 1);
			cv.value = StringHelper::getWord(line, 3);

			vars.push_back(cv);
		}
	}

	void Config::save(const String &path) {
		FILE *cFile = fopen(path.c_str(), "wt");

		for (int i = 0; i < vars.size(); i++) {
			String line = vars[i].name + " = " + vars[i].value + "\n";
			fprintf(cFile, "%s", line.c_str());
		}

		fclose(cFile);
	}

	const float Config::getFloat(const String &name, const float def) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name)
				return StringHelper::toFloat(vars[i].value);
		}
		return def;
	}

	const int Config::getInt(const String &name, const int def) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name)
				return StringHelper::toInt(vars[i].value);
		}
		return def;
	}

	const bool Config::getBool(const String &name, const bool def) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name)
				return StringHelper::toInt(vars[i].value);
		}
		return def;
	}

	void Config::setFloat(const String &name, const float value) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name) {
				vars[i].value = StringHelper::fromFloat(value);
				return;
			}
		}
	}

	void Config::setInt(const String &name, const int value) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name) {
				vars[i].value = StringHelper::fromInt(value);
				return;
			}
		}
	}

	void Config::setBool(const String &name, const bool value) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name) {
				vars[i].value = StringHelper::fromBool(value);
				return;
			}
		}
	}

	void Config::setString(const String &name, const String &value) {
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i].name == name) {
				vars[i].value = value;
				return;
			}
		}
	}

}