#include <assert.h>
#include "Log.h"
#include "scripting.h"
#include "WrappedScriptFunctions.h"
#include "../MathLib.h"
#include "../../Externals/angelscriptsdk/angelscript/include/angelscript.h"

namespace VEGA{
	/**
	*/
	void CoreScriptInterp::regCoreFunctions(asIScriptEngine*_engine){
		int r;

		//
		//!Logging
		//
		r = _engine->RegisterGlobalFunction("void LogPrintf(const string val)", asFUNCTION(ScriptLogPrintf), asCALL_CDECL); assert( r >= 0 );
		r = _engine->RegisterGlobalFunction("void Warning(const string val)", asFUNCTION(ScriptWarning), asCALL_CDECL); assert( r >= 0 );
		r = _engine->RegisterGlobalFunction("void Debug(const string val)", asFUNCTION(ScriptDebug), asCALL_CDECL); assert( r >= 0 );
		r = _engine->RegisterGlobalFunction("void Error(const string val, bool val)", asFUNCTION(ScriptError), asCALL_CDECL); assert( r >= 0 );
		//
		//!Math
		//

		//
		//Vec2
		//
		/*	r = _engine->RegisterObjectType("Vec2",0,asOBJ_REF);assert( r >= 0 );
		r = _engine->RegisterObjectProperty("float","float x",offsetof(Vec2,x)); assert( r >= 0 );
		r = _engine->RegisterObjectProperty("float","float y",offsetof(Vec2,y)); assert( r >= 0 );

		r = _engine->RegisterObjectMethod("Vec2","void length()",asMETHOD(Vec2, length),asCALL_THISCALL); assert( r >= 0 );
		r = _engine->RegisterGlobalFunction("Vec2 normalize(const Vec2 &in)", asFUNCTION(Vec2::normalize), asCALL_CDECL); assert( r >= 0 );
		r = _engine->RegisterGlobalFunction("Vec2 dot(const Vec2 &in, const Vec2 &in)", asFUNCTION(Vec2::dot), asCALL_CDECL); assert( r >= 0 );*/
		//ALL:TODO:Сделать экспорт операторов

		//_engine->RegisterObjectBehaviour("Vec2",asBEHAVE_FACTORY,"float3@ new_float3()",asFUNCTION(Float3FactoryE),asCALL_CDECL);
		//_engine->RegisterObjectBehaviour("Vec2",asBEHAVE_ADDREF,"void AddRef()",asMETHOD(Vec2,AddRef),asCALL_THISCALL);
		//_engine->RegisterObjectBehaviour("Vec2",asBEHAVE_RELEASE,"void Release()",asMETHOD(Vec2,Release),asCALL_THISCALL);

		//r = engine->RegisterObjectMethod("AxisAlignedBox", "const Vector3& getMaximum()",       asMETHODPR(Ogre::AxisAlignedBox, getMaximum, (void) const, const Ogre::Vector3&),       asCALL_THISCALL); assert(r >= 0);
		//r = engine->RegisterObjectMethod("AxisAlignedBox", "void setMinimum(const Vector3 &in)",        asMETHODPR(Ogre::AxisAlignedBox, setMinimum, (const Ogre::Vector3&), void),     asCALL_THISCALL); assert(r >= 0);


		// The game engine will determine the class that represents the controller
		// by checking if the class implements the IController interface. No methods
		// are registered for this interface, as the script shouldn't be required to
		// implement the methods. This will allow the game engine to avoid calling
		// methods that doesn't do anything, thus improving performance.
		//r = engine->RegisterInterface("IController"); assert( r >= 0 );
	}	

}