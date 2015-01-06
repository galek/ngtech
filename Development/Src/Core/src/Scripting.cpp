#include "CorePrivate.h"
#include "Scripting.h"
//***************************************************
#include "lua/luabind.hpp"
#include "lua/operator.hpp"
#include "lua/lua.hpp"
//***************************************************
#include "../Common/IAudio.h"
#include "../Common/IDataManager.h"
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

		module(mLuaState)
			[
				//Interfaces
				//====Audio====
				/*class_<I_Audio>("I_Audio")
				.def("initialise", &I_Audio::initialise)
				.def("getVendor", &I_Audio::getVendor)
				.def("getRenderer", &I_Audio::getRenderer)
				.def("getVersion", &I_Audio::getVersion)
				.def("getExtensions", &I_Audio::getExtensions)
				.def("setListener", &I_Audio::setListener),*/
				//====IDataStream====
				class_<IDataStream>("IDataStream")
				.def("eof", &IDataStream::eof)
				.def("size", &IDataStream::size)
				.def("readline", &IDataStream::readline)
				.def("read", &IDataStream::read),
				//====DataManager====
				class_<DataManager>("DataManager")
				.def("getData", &DataManager::getData)
				.def("freeData", &DataManager::freeData)
				.def("isDataExist", &DataManager::isDataExist)
				.def("getDataListNames", &DataManager::getDataListNames)
				.def("getDataPath", &DataManager::getDataPath),
				//====ICallback====
				class_<ICallback>("ICallback")
				.def("Body", &ICallback::Body),
				//====IGam====e
				class_<IGame>("IGame")
				.def("initialise", &IGame::initialise)
				.def("update", &IGame::update)
				.def("runRenderCallback", &IGame::runRenderCallback)
				.def("runEventsCallback", &IGame::runEventsCallback)
				.def("setRenderCallback", &IGame::setRenderCallback)
				.def("setEventsCallback", &IGame::setEventsCallback),
				//====IGUI====
				class_<IGUI>("IGUI")
				.def("resize", &IGUI::resize)
				.def("update", &IGUI::update),
				//====I_ILImage====
				class_<I_ILImage>("I_ILImage")
				.def("toNormalMap", &I_ILImage::toNormalMap)
				.def("toGreyScale", &I_ILImage::toGreyScale)
				.def("getData", &I_ILImage::getData)
				.def("getWidth", &I_ILImage::getWidth)
				.def("getHeight", &I_ILImage::getHeight)
				.def("getDepth", &I_ILImage::getDepth)
				.def("getBPP", &I_ILImage::getBPP)
				.def("getFormat", &I_ILImage::getFormat)
				.def("is3D", &I_ILImage::is3D),

				//Render
				//====I_Texture====
				class_<I_Texture>("I_Texture")
				.def("setWrap", &I_Texture::setWrap)
				.def("setFilter", &I_Texture::setFilter)
				.def("setAniso", &I_Texture::setAniso)
				.def("set", &I_Texture::set)
				.def("unset", &I_Texture::unset)
				.def("beginRenderTo", &I_Texture::beginRenderTo)
				.def("copy", &I_Texture::copy)
				.def("endRenderTo", &I_Texture::endRenderTo),
				//====I_Render====
				class_<I_Render>("I_Render")
				.def("GetDL", &I_Render::GetDL)
				.def("GetOQ", &I_Render::GetOQ)
				//.def("TextureCreate2D", &I_Render::TextureCreate2D)
				//.def("TextureCreateCube", &I_Render::TextureCreateCube)
				//.def("TextureCreate2D", &I_Render::TextureCreate2D)
				//.def("TextureCreate3D", &I_Render::TextureCreate3D)
				//.def("TextureCreateCube", &I_Render::TextureCreateCube)
				//.def("TextureCreate3D", &I_Render::TextureCreate3D)

				.def("ShaderCreate", &I_Render::ShaderCreate)
				.def("CreateFBO", &I_Render::CreateFBO)
				.def("CreateIBO", &I_Render::CreateIBO)
				.def("CreateVBO", &I_Render::CreateVBO)
				.def("CreateImage2D", &I_Render::CreateImage2D)
				.def("CreateImageEmpty2D", &I_Render::CreateImageEmpty2D)
				.def("CreateImageNoise2D", &I_Render::CreateImageNoise2D)
				.def("CreateImageEmpty3D", &I_Render::CreateImageEmpty3D)
				.def("CreateImageNoise3D", &I_Render::CreateImageNoise3D)

				.def("requireExtension", &I_Render::requireExtension)
				.def("initialise", &I_Render::initialise)

				.def("reshape", &I_Render::reshape)
				.def("getViewport", &I_Render::getViewport)
				.def("clearColor", &I_Render::clearColor)
				.def("colorMask", &I_Render::colorMask)
				.def("clear", &I_Render::clear)
				.def("flush", &I_Render::flush)
				.def("viewport", &I_Render::viewport)
				//----------Using-textures----
				//.def("setColor", &I_Render::setColor)
				//.def("setColor", &I_Render::setColor)
				//----------2D/3D-mode----
				.def("enable2d", &I_Render::enable2d)
				.def("enable3d", &I_Render::enable3d)
				.def("drawRect", &I_Render::drawRect)
				//---Blending-------------------------------
				.def("blendFunc", &I_Render::blendFunc)
				//				.def("enableBlending", &I_Render::enableBlending)
				//.def("enableBlending", &I_Render::enableBlending)
				.def("disableBlending", &I_Render::disableBlending)
				//---Alpha-test-----------------------------
				.def("alphaTestFunc", &I_Render::alphaTestFunc)
				//				.def("enableAlphaTest", &I_Render::enableAlphaTest)
				//.def("enableAlphaTest", &I_Render::enableAlphaTest)
				.def("disableAlphaTest", &I_Render::disableAlphaTest)
				//---Z-Buffer-------------------------------
				.def("depthFunc", &I_Render::depthFunc)
				//				.def("enableDepth", &I_Render::enableDepth)
				//.def("enableDepth", &I_Render::enableDepth)
				.def("disableDepth", &I_Render::disableDepth)
				.def("depthMask", &I_Render::depthMask)
				//---Scissor-test-------------------------------
				.def("scissorRect", &I_Render::scissorRect)
				//				.def("enableScissor", &I_Render::enableScissor)
				//				.def("enableScissor", &I_Render::enableScissor)
				.def("disableScissor", &I_Render::disableScissor)

				//---Z-Buffer-------------------------------
				.def("polygonOffsetFill", &I_Render::polygonOffsetFill)
				//				.def("enablePolygonOffsetFill", &I_Render::enablePolygonOffsetFill)
				//.def("enablePolygonOffsetFill", &I_Render::enablePolygonOffsetFill)
				.def("disablePolygonOffsetFill", &I_Render::disablePolygonOffsetFill)
				//---Culling-------------------------------
				//.def("cullFace", &I_Render::cullFace)
				//.def("cullFace", &I_Render::cullFace)
				//.def("enableCulling", &I_Render::enableCulling)
				//.def("enableCulling", &I_Render::enableCulling)
				//.def("enableCulling", &I_Render::enableCulling)
				.def("disableCulling", &I_Render::disableCulling)

				//---Clip-plains--------------------------
				.def("clipPlane", &I_Render::clipPlane)
				//.def("enableClipPlane", &I_Render::enableClipPlane)
				//.def("enableClipPlane", &I_Render::enableClipPlane)
				.def("disableClipPlane", &I_Render::disableClipPlane)

				//---Matrix-works-------------------------
				.def("setMatrixMode_Projection", &I_Render::setMatrixMode_Projection)
				.def("setMatrixMode_Modelview", &I_Render::setMatrixMode_Modelview)
				.def("pop", &I_Render::pop)
				.def("push", &I_Render::push)
				.def("identity", &I_Render::identity)
				.def("getMatrix_MVP", &I_Render::getMatrix_MVP)
				.def("getMatrix_Projection", &I_Render::getMatrix_Projection)
				.def("getMatrix_Modelview", &I_Render::getMatrix_Modelview)
				.def("multMatrix", &I_Render::multMatrix)
				.def("loadMatrix", &I_Render::loadMatrix)
				.def("translate", &I_Render::translate)
				.def("rotate", &I_Render::rotate)
				.def("scale", &I_Render::scale)

				//---Draw-Geom-Stream---------------------
				.def("drawIndexedGeometry", &I_Render::drawIndexedGeometry)
				.def("drawGeometry", &I_Render::drawGeometry),

				//====ISHADER====
				class_<I_Shader>("I_Shader")
				.def("Enable", &I_Shader::Enable)
				.def("Disable", &I_Shader::Disable)
				.def("sendMat4", &I_Shader::sendMat4)
				.def("sendVec4", &I_Shader::sendVec4)
				.def("sendVec3", &I_Shader::sendVec3)
				.def("sendVec2", &I_Shader::sendVec2)
				.def("sendFloat", &I_Shader::sendFloat)
				.def("sendInt", &I_Shader::sendInt),

				//====I_VBManager====
				class_<I_VBManager>("I_VBManager")
				.def("setVertexSource", &I_VBManager::setVertexSource)
				.def("setNormalSource", &I_VBManager::setNormalSource)
				.def("setTexCoordSource", &I_VBManager::setTexCoordSource)
				.def("setIndexSource", &I_VBManager::setIndexSource)
				.def("unsetVertexSource", &I_VBManager::unsetVertexSource)
				.def("unsetNormalSource", &I_VBManager::unsetNormalSource)
				.def("unsetTexCoordSource", &I_VBManager::unsetTexCoordSource)
				.def("unsetIndexSource", &I_VBManager::unsetIndexSource)
				.def("set", &I_VBManager::set)
				.def("unset", &I_VBManager::unset),

				//====I_OcclusionQuery====
				class_<I_OcclusionQuery>("I_OcclusionQuery")
				.def("beginRendering", &I_OcclusionQuery::beginRendering)
				.def("endRendering", &I_OcclusionQuery::endRendering)
				.def("getResult", &I_OcclusionQuery::getResult),

				//====I_DisplayList====
				class_<I_DisplayList>("I_DisplayList")
				.def("beginBuild", &I_DisplayList::beginBuild)
				.def("endBuild", &I_DisplayList::endBuild)
				.def("call", &I_DisplayList::call),

				//====I_FBOManager====
				class_<I_FBOManager>("I_FBOManager")
				.def("createColorAttachment", &I_FBOManager::createColorAttachment)
				.def("createDepthAttachment", &I_FBOManager::createDepthAttachment)
				.def("createStencilAttachment", &I_FBOManager::createStencilAttachment)
				.def("setColorTarget", &I_FBOManager::setColorTarget)
				.def("setDepthTarget", &I_FBOManager::setDepthTarget)
				.def("set", &I_FBOManager::set)
				.def("unset", &I_FBOManager::unset)
				.def("clear", &I_FBOManager::clear)
				.def("flush", &I_FBOManager::flush)/*,

				//Globals
				def("GetCore", &GetCore),
				def("GetVFS", &GetVFS),
				def("GetWindow", &GetWindow),
				def("GetCvars", &GetCvars),
				def("GetRender", &GetRender),
				def("GetAudio", &GetAudio),
				def("GetPhysics", &GetPhysics),
				def("GetCache", &GetCache),
				def("GetGUI", &GetGUI),
				def("GetScene", &GetScene),
				def("GetGame", &GetGame),
				def("GetScripting", &GetScripting)*/
			];
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
}