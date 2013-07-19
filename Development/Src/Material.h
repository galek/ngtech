/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once
 
//***************************************************************************
#include "Common/EString.h"
#include "File.h"
#include "GLSystem.h"
#include <vector>
//***************************************************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: Material struct
//---------------------------------------------------------------------------
class Material {
public:
	Material(String path);
	~Material();

	bool setPass(const String &name);

	bool passHasBlending(const String &name);
	void setPassBlending(const String &name);
	void setPassAlphaTest();

	void unsetPassBlending();
	void unsetPassAlphaTest();

	void unsetPass();
		
private:
	struct Pass {
		enum SceneParam {
			matTime,
			matLightIRadius,

			matLightColor, 
			matLightPosition,
			matLightDirection,
			matViewPosition,
				
			matMVP,
			matWorld,
			matViewportTransform,
			matSpotTransform,

			matShadowMap,
			matViewportMap,
			matSpotMap,
		};
		
		String name;

		GLShader *shader;

		std::vector<std::pair<String, GLTexture*>> u_sampler2D;
		std::vector<std::pair<String, GLTexture*>> u_samplerCube;
		std::vector<std::pair<String, float>> u_float;
		std::vector<std::pair<String, Vec2>> u_Vec2;
		std::vector<std::pair<String, Vec3>> u_Vec3;
		std::vector<std::pair<String, Vec4>> u_Vec4;
		std::vector<std::pair<String, Mat4>> u_Mat4;
		std::vector<std::pair<String, SceneParam>> u_scene_params;

		bool hasBlending;
		IRender::BlendParam src, dst;

		bool hasAlphaTest;
		IRender::CompareType alphaFunc;
		float alphaRef;

		bool depthMask;

		bool castShadows;
		bool recieveShadows;

		int shadowMapUnit;
		int viewportMapUnit;
		int spotMapUnit;
		int maxUnit;
	};
	std::vector<Pass*> passes;
	Pass *currentPass;

	friend class Scene;
};

};
