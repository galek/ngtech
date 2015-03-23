#include "CorePrivate.h"
#include "Scripting.h"
//***************************************************
#include "lua/luabind.hpp"
#include "lua/operator.hpp"
#include "lua/lua.hpp"
//***************************************************
#include "../../Common/IAudio.h"
#include "../../Common/IDataManager.h"
//***************************************************
#include "WrappedScriptFunctions.h"
#include "SteamWorks_API.h"
//***************************************************

namespace NGTech{

	/**
	*/
	CoreScriptInterp::CoreScriptInterp()
		:mLuaState(nullptr)
	{
	}

	/**
	*/
	void CoreScriptInterp::initialise(){
		using namespace luabind;
		// Create a new lua state
		mLuaState = luaL_newstate();
		luaL_openlibs(mLuaState);

		// Connect LuaBind to this lua state
		open(mLuaState);
		bindLogFunctions();
		bindMathFunctions();
		bindCoreFunctions();
	}

	/**
	*/
	int CoreScriptInterp::doString(const char* code) const {
		DebugM("Loaded script: %s \n", code);
		String str = code;
		if (luaL_loadbuffer(mLuaState, code, str.size(), "line")) {
			Warning("lua couldn't parse '%i': %s.\n",
				str.size(), lua_tostring(mLuaState, -1));
			lua_pop(mLuaState, 1);
			return 1;
		}
		else {
			if (lua_pcall(mLuaState, 0, 1, 0)) {
				Warning("lua couldn't execute '%i': %s.\n",
					str.size(), lua_tostring(mLuaState, -1));
				lua_pop(mLuaState, 1);
			}
			else
				lua_pop(mLuaState, lua_gettop(mLuaState));
			return 1;
		}

		return 0;
	}

	/**
	*/
	int CoreScriptInterp::runScript(const char* _filename){
		VFile file(_filename);
		return doString(file.LoadFile());
	}

	/**
	*/
	CoreScriptInterp::~CoreScriptInterp() {
		lua_close(mLuaState);
	}

	/**
	*/
	void CoreScriptInterp::bindMathFunctions()
	{
		using namespace luabind;
		using namespace NGTech;

		module(mLuaState)
			[
				//Math
				class_<Math>("Math")
				.def("clamp", &Math::clamp)
				.def("angleBetweenVec", &Math::angleBetweenVec)
				.def("insidePolygon", &Math::insidePolygon)
				.def("intersectPlaneByRay", &Math::intersectPlaneByRay)
				.def("intersectPolygonByRay", &Math::intersectPolygonByRay)
				.def("intersectSphereByRay", &Math::intersectSphereByRay)

				//Vec2
				, class_<Vec2>("Vec2")
				.def(constructor<>())
				.def(constructor<float, float>())
				.def(constructor<const Vec2 &>())
				.def("normalize", &Vec2::normalize)
				.def("dot", &Vec2::dot)
				.def("length", &Vec2::length)
				//operators
				.def(self - Vec2())
				.def(self + Vec2())
				.def(const_self / const_self)
				.def(const_self / other<float>())
				.def(other<float>() / const_self)
				.def(const_self * other<float>())
				.def(const_self * Vec2())//Сомневаюсь,что правильно
				.def(const_self + const_self)
				.def(const_self - const_self)
				//Added:Right?
				.def(self == const_self)
				.def(const_self == const_self)
				//Vec3
				, class_<Vec3>("Vec3")
				.def(constructor<>())
				.def(constructor<float, float, float>())
				.def(constructor<const Vec3 &>())
				.def(constructor<const Vec4 &>())
				.def("normalize", &Vec3::normalize)
				.def("dot", &Vec3::dot)
				.def("length", &Vec3::length)
				.def("cross", &Vec3::cross)
				//operators
				.def(self - Vec3())
				.def(self + Vec3())
				.def(const_self / const_self)
				.def(const_self / other<float>())
				.def(other<float>() / const_self)
				.def(const_self * other<float>())
				.def(const_self * Vec3())//Сомневаюсь,что правильно
				.def(const_self + const_self)
				.def(const_self - const_self)
				//Added:Right?
				.def(self == const_self)
				.def(const_self == const_self)
				//Vec4
				, class_<Vec4>("Vec4")
				.def(constructor<>())
				.def(constructor<float, float, float, float>())
				.def(constructor<const Vec4 &>())
				.def(constructor<const Vec3 &>())
				.def(constructor<const Vec3 &, float>())
				.def("normalize", &Vec4::normalize)
				.def("dot", &Vec4::dot)
				.def("length", &Vec4::length)
				//operators
				.def(self - Vec4())
				.def(self + Vec4())
				.def(const_self / const_self)
				.def(const_self / other<float>())
				.def(other<float>() / const_self)
				.def(const_self * other<float>())
				.def(const_self * Vec4())//Сомневаюсь,что правильно
				.def(const_self + const_self)
				.def(const_self - const_self)
				//Added:Right?
				.def(self == const_self)
				.def(const_self == const_self)
				//TBNComputer
				, class_<TBNComputer>("TBNComputer")
				.def("computeN", &TBNComputer::computeN)
				.def("computeTBN", &TBNComputer::computeTBN)
			];

		/*
		Vec2& operator=(const Vec2 &in);

		float& operator[](int index);
		float operator[](int index) const;

		operator float*();
		operator const float*() const;

		Not Exist in LUA
		Vec2& operator+=(const Vec2 &v);
		Vec2& operator-=(const Vec2 &v);
		Vec2& operator*=(const Vec2 &v);
		Vec2& operator/=(const Vec2 &v);

		bool operator!=(const Vec2 &v) const;


		*/
	}

	/**
	*/
	void CoreScriptInterp::bindLogFunctions() {
		using namespace luabind;
		using namespace NGTech;
		module(mLuaState)
			[
				def("LogPrintf", &ScriptLogPrintf),
				def("Debug", &ScriptDebug),
				def("Warning", &ScriptWarning),
				def("Error", &ScriptError)
			];
	}

	/**
	*/
	void CoreScriptInterp::bindCoreFunctions() {
		using namespace luabind;
		using namespace NGTech;

		
	}

	/**
	*/
	void CoreScriptInterp::bindSteamWorks() {
		using namespace luabind;
		using namespace NGTech;
		module(mLuaState)
			[
				def("API_SteamWorks_SetAchievement", &API_SteamWorks_SetAchievement),
				def("API_SteamWorks_DeleteAchievement", &API_SteamWorks_DeleteAchievement),
				def("API_SteamWorks_SetStat", &API_SteamWorks_SetStat),
				def("API_SteamWorks_GetStat", &API_SteamWorks_GetStat)
			];
	}

	/**
	*/
	void lua_stacktrace(lua_State* L)
	{
		lua_Debug entry;
		int depth = 0;

		while (lua_getstack(L, depth, &entry))
		{
			int status = lua_getinfo(L, "Sln", &entry);
			assert(status);

			DebugM("%s(%d): %s\n", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
			depth++;
		}
	}
}