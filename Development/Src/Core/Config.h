/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#ifndef __E_STRING_H__
#include "..\Common\EString.h"
#endif
#include <vector>
//**************************************

namespace VEGA {

	class Config {
	public:
		Config(const String &path);
		void save(const String &path);

		float getFloat(const String &name);
		int getInt(const String &name);
		bool getBool(const String &name);
		const String &getString(const String &name); 

		void setFloat(const String &name, float value);
		void setInt(const String &name, int value);
		void setBool(const String &name, bool value);
		void setString(const String &name, const String &value);

	private:
		struct ConfigVar {
			ConfigVar() {
				name = "";
				value = "";
			};

			String name;
			String value;
		};

		std::vector<ConfigVar> vars;
	};

};


