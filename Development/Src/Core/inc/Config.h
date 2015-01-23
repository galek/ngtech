#pragma once

//**************************************
#ifndef __E_STRING_H__
#include "../../Common/StringHelper.h"
#endif
#include <vector>
//**************************************

namespace NGTech {

	class CORE_API Config {
	public:
		Config(const String &path);
		void save(const String &path);

		const float getFloat(const String &name, const float def = 0.0f);
		const int getInt(const String &name, const int def = 0);
		const bool getBool(const String &name, const bool def = false);
		const String &getString(const String &name);

		void setFloat(const String &name, const float value);
		void setInt(const String &name, const int value);
		void setBool(const String &name, const bool value);
		void setString(const String &name, const String &value);

	private:
		struct ConfigVar {
			ConfigVar() {
				name.clear();
				value.clear();
			}

			String name;
			String value;
		};

		std::vector<ConfigVar> vars;
	};

}