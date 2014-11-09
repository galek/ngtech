#pragma once

//**************************************
#ifndef __E_STRING_H__
#include "..\Common\StringHelper.h"
#endif
#include <vector>
//**************************************

namespace NGTech {

	class CORE_API Config {
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
				name.clear();
				value.clear();
			}

			String name;
			String value;
		};

		std::vector<ConfigVar> vars;
	};

}