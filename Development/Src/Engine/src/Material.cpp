/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#include "EnginePrivate.h"
//***************************************************************************
#include "Engine.h"
#include "Material.h"
#include "Log.h"
#include "Error.h"
#include "Config.h"
#include "Cache.h"
#include "Scene.h"
#include "CvarManager.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates new material from file
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Material::Material(String path) {
		if (path == ""){
			Warning("[Material::Material]Material loading from blank path,automatic deletion.Check your material list");
			return;
			delete this;
		}
		path = "../data/materials/" + path;

		//begin loading
		VFile mFile(path.c_str());
		while (!mFile.EndOfFile()) {
			String line = mFile.GetLine();

			if (StringHelper::getWord(line, 1) == "pass") {
				Pass *pass = new Pass();
				passes.push_back(pass);

				//pass name
				pass->name = StringHelper::getWord(line, 2);

				pass->hasBlending = false;
				pass->hasAlphaTest = false;
				pass->castShadows = true;
				pass->recieveShadows = true;

				while (StringHelper::getWord(line, 1) != "}" && !mFile.EndOfFile()) {
					line = mFile.GetLine();

					if (line[0] == '/' && line[1] == '/')
						continue;

					//cast_shadows
					if (StringHelper::getWord(line, 1) == "cast_shadows") {
						pass->castShadows = StringHelper::getInt(line, 3);
					}

					//recieve_shadows
					if (StringHelper::getWord(line, 1) == "recieve_shadows") {
						pass->recieveShadows = StringHelper::getInt(line, 3);
					}

					//blending
					if (StringHelper::getWord(line, 1) == "blending") {
						pass->hasBlending = true;

						if (StringHelper::getWord(line, 3) == "ONE") {
							pass->src = I_Render::ONE;
						}
						else if (StringHelper::getWord(line, 3) == "ZERO") {
							pass->src = I_Render::ZERO;
						}
						else if (StringHelper::getWord(line, 3) == "SRC_COLOR") {
							pass->src = I_Render::SRC_COLOR;
						}
						else if (StringHelper::getWord(line, 3) == "DST_COLOR") {
							pass->src = I_Render::DST_COLOR;
						}
						else if (StringHelper::getWord(line, 3) == "SRC_ALPHA") {
							pass->src = I_Render::SRC_ALPHA;
						}
						else if (StringHelper::getWord(line, 3) == "DST_ALPHA") {
							pass->src = I_Render::DST_ALPHA;
						}
						else if (StringHelper::getWord(line, 3) == "ONE_MINUS_SRC_COLOR") {
							pass->src = I_Render::ONE_MINUS_SRC_COLOR;
						}
						else if (StringHelper::getWord(line, 3) == "ONE_MINUS_DST_COLOR") {
							pass->src = I_Render::ONE_MINUS_DST_COLOR;
						}
						else if (StringHelper::getWord(line, 3) == "ONE_MINUS_SRC_ALPHA") {
							pass->src = I_Render::ONE_MINUS_SRC_ALPHA;
						}
						else if (StringHelper::getWord(line, 3) == "ONE_MINUS_DST_ALPHA") {
							pass->src = I_Render::ONE_MINUS_DST_ALPHA;
						}
						else {
							pass->hasBlending = false;
						}

						if (StringHelper::getWord(line, 4) == "ONE") {
							pass->dst = I_Render::ONE;
						}
						else if (StringHelper::getWord(line, 4) == "ZERO") {
							pass->dst = I_Render::ZERO;
						}
						else if (StringHelper::getWord(line, 4) == "SRC_COLOR") {
							pass->dst = I_Render::SRC_COLOR;
						}
						else if (StringHelper::getWord(line, 4) == "DST_COLOR") {
							pass->dst = I_Render::DST_COLOR;
						}
						else if (StringHelper::getWord(line, 4) == "SRC_ALPHA") {
							pass->dst = I_Render::SRC_ALPHA;
						}
						else if (StringHelper::getWord(line, 4) == "DST_ALPHA") {
							pass->dst = I_Render::DST_ALPHA;
						}
						else if (StringHelper::getWord(line, 4) == "ONE_MINUS_SRC_COLOR") {
							pass->dst = I_Render::ONE_MINUS_SRC_COLOR;
						}
						else if (StringHelper::getWord(line, 4) == "ONE_MINUS_DST_COLOR") {
							pass->dst = I_Render::ONE_MINUS_DST_COLOR;
						}
						else if (StringHelper::getWord(line, 4) == "ONE_MINUS_SRC_ALPHA") {
							pass->dst = I_Render::ONE_MINUS_SRC_ALPHA;
						}
						else if (StringHelper::getWord(line, 4) == "ONE_MINUS_DST_ALPHA") {
							pass->dst = I_Render::ONE_MINUS_DST_ALPHA;
						}
						else {
							pass->hasBlending = false;
						}
					}

					//alpha test
					if (StringHelper::getWord(line, 1) == "alpha_test") {
						pass->hasAlphaTest = true;
						if (StringHelper::getWord(line, 3) == "NEVER") {
							pass->alphaFunc = I_Render::NEVER;
						}
						else if (StringHelper::getWord(line, 3) == "LESS") {
							pass->alphaFunc = I_Render::LESS;
						}
						else if (StringHelper::getWord(line, 3) == "EQUAL") {
							pass->alphaFunc = I_Render::EQUAL;
						}
						else if (StringHelper::getWord(line, 3) == "LEQUAL") {
							pass->alphaFunc = I_Render::LEQUAL;
						}
						else if (StringHelper::getWord(line, 3) == "GREATER") {
							pass->alphaFunc = I_Render::GREATER;
						}
						else if (StringHelper::getWord(line, 3) == "NOTEQUAL") {
							pass->alphaFunc = I_Render::NOTEQUAL;
						}
						else if (StringHelper::getWord(line, 3) == "GEQUAL") {
							pass->alphaFunc = I_Render::GEQUAL;
						}
						else if (StringHelper::getWord(line, 3) == "ALWAYS") {
							pass->alphaFunc = I_Render::ALWAYS;
						}
						else {
							pass->hasAlphaTest = false;
						}

						pass->alphaRef = StringHelper::getFloat(line, 4);
					}

					//shader
					if (StringHelper::getWord(line, 1) == "shader") {
						String name = StringHelper::getWord(line, 3);

						String defines = "";
						pass->shader = GetCache()->loadShader(name, defines);
					}

					//sampler
					if (StringHelper::getWord(line, 1) == "sampler2D") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "scene::shadow_map") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matShadowMap));
						}
						else if (StringHelper::getWord(line, 4) == "scene::viewport_map") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewportMap));
						}
						else if (StringHelper::getWord(line, 4) == "scene::spot_map") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matSpotMap));

							//user params
						}
						else if (StringHelper::getWord(line, 4) == "normal_map()") {
							I_Texture *sampler;
							ILImage *map = ILImage::create2d("../data/textures/" + StringHelper::getWord(line, 5));
							map->toNormalMap(4);
							sampler = GetCache()->loadTexture2d(map, "../data/textures/" + StringHelper::getWord(line, 5) + "_NORMAL_MAP");
							pass->u_sampler2D.push_back(std::pair<String, I_Texture*>(name, sampler));
							delete map;
						}
						else {
							I_Texture *sampler;
							sampler = GetCache()->loadTexture2d("../data/textures/" + StringHelper::getWord(line, 4));
							pass->u_sampler2D.push_back(std::pair<String, I_Texture*>(name, sampler));
						}
					}

					//sampler
					if (StringHelper::getWord(line, 1) == "samplerCube") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "scene::shadow_map") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matShadowMap));

							//user params
						}
						else {
							I_Texture *sampler;
							sampler = GetCache()->loadTextureCube("../data/textures/" + StringHelper::getWord(line, 4));
							pass->u_samplerCube.push_back(std::pair<String, I_Texture*>(name, sampler));
						}
					}

					//float
					if (StringHelper::getWord(line, 1) == "float") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "scene::time") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matTime));
						}
						else if (StringHelper::getWord(line, 4) == "scene::light_iradius") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightIRadius));

							//user params
						}
						else {
							float value = StringHelper::getFloat(line, 4);
							pass->u_float.push_back(std::pair<String, float>(name, value));
						}
					}

					//Vec2
					if (StringHelper::getWord(line, 1) == "vec2") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "") {

							//user params
						}
						else {
							Vec2 value = Vec2(StringHelper::getFloat(line, 4),
								StringHelper::getFloat(line, 5));

							pass->u_Vec2.push_back(std::pair<String, Vec2>(name, value));
						}
					}

					//Vec3
					if (StringHelper::getWord(line, 1) == "vec3") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "scene::light_color") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightColor));
						}
						else if (StringHelper::getWord(line, 4) == "scene::light_position") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightPosition));
						}
						else if (StringHelper::getWord(line, 4) == "scene::light_direction") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightDirection));
						}
						else if (StringHelper::getWord(line, 4) == "scene::view_position") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewPosition));

							//user params
						}
						else {

							Vec3 value = Vec3(StringHelper::getFloat(line, 4),
								StringHelper::getFloat(line, 5),
								StringHelper::getFloat(line, 6));

							pass->u_Vec3.push_back(std::pair<String, Vec3>(name, value));
						}
					}

					//Vec4
					if (StringHelper::getWord(line, 1) == "vec4") {
						String name = StringHelper::getWord(line, 2);
						//standart params
						if (StringHelper::getWord(line, 4) == "") {

							//user params
						}
						else {
							Vec4 value = Vec4(StringHelper::getFloat(line, 4),
								StringHelper::getFloat(line, 5),
								StringHelper::getFloat(line, 6),
								StringHelper::getFloat(line, 7));

							pass->u_Vec4.push_back(std::pair<String, Vec4>(name, value));
						}
					}

					//Mat4
					if (StringHelper::getWord(line, 1) == "mat4") {
						String name = StringHelper::getWord(line, 2);

						//standart params
						if (StringHelper::getWord(line, 4) == "scene::mvp") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matMVP));
						}
						else if (StringHelper::getWord(line, 4) == "scene::world") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matWorld));
						}
						else if (StringHelper::getWord(line, 4) == "scene::spot_transform") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matSpotTransform));
						}
						else if (StringHelper::getWord(line, 4) == "scene::viewport_transform") {
							pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewportTransform));

							//user params
						}
						else {
							Mat4 value = Mat4(StringHelper::getFloat(line, 4),
								StringHelper::getFloat(line, 5),
								StringHelper::getFloat(line, 6),
								StringHelper::getFloat(line, 7),
								StringHelper::getFloat(line, 8),
								StringHelper::getFloat(line, 9),
								StringHelper::getFloat(line, 10),
								StringHelper::getFloat(line, 11),
								StringHelper::getFloat(line, 12),
								StringHelper::getFloat(line, 13),
								StringHelper::getFloat(line, 14),
								StringHelper::getFloat(line, 15),
								StringHelper::getFloat(line, 16),
								StringHelper::getFloat(line, 17),
								StringHelper::getFloat(line, 18),
								StringHelper::getFloat(line, 19));

							pass->u_Mat4.push_back(std::pair<String, Mat4>(name, value));
						}
					}
				}
			}
		}
//		fclose(mFile);
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys material
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Material::~Material() {
		for (int p = 0; p < passes.size(); p++) {
			Pass *pass = passes[p];
			for (int i = 0; i < pass->u_sampler2D.size(); i++) {
				GetCache()->deleteTexture(pass->u_sampler2D[i].second);
			}
			pass->u_sampler2D.clear();

			for (int i = 0; i < pass->u_samplerCube.size(); i++) {
				GetCache()->deleteTexture(pass->u_samplerCube[i].second);
			}
			pass->u_samplerCube.clear();

			pass->u_float.clear();
			pass->u_Vec2.clear();
			pass->u_Vec3.clear();
			pass->u_Vec4.clear();
			pass->u_Mat4.clear();

			GetCache()->deleteShader(pass->shader);
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets material pass
	//Params:  name - pass name
	//Returns: true - if pass found
	//---------------------------------------------------------------------------
	bool Material::setPass(const String &name) {
		Pass *p = NULL;

		for (int i = 0; i < passes.size(); i++) {
			if (passes[i]->name == name) {
				p = passes[i];
			}
		}
		if (!p) return false;

		currentPass = p;

		p->maxUnit = 0;

		p->shader->set();

		for (int i = 0; i < p->u_float.size(); i++) {
			p->shader->sendFloat(p->u_float[i].first, p->u_float[i].second);
		}

		for (int i = 0; i < p->u_Vec2.size(); i++) {
			p->shader->sendVec2(p->u_Vec2[i].first, p->u_Vec2[i].second);
		}

		for (int i = 0; i < p->u_Vec3.size(); i++) {
			p->shader->sendVec3(p->u_Vec3[i].first, p->u_Vec3[i].second);
		}

		for (int i = 0; i < p->u_Vec4.size(); i++) {
			p->shader->sendVec4(p->u_Vec4[i].first, p->u_Vec4[i].second);
		}

		for (int i = 0; i < p->u_Mat4.size(); i++) {
			p->shader->sendMat4(p->u_Mat4[i].first, p->u_Mat4[i].second);
		}

		for (int i = 0; i < p->u_sampler2D.size(); i++) {
			p->shader->sendInt(p->u_sampler2D[i].first, p->maxUnit + i);
			p->u_sampler2D[i].second->set(p->maxUnit + i);
		}
		p->maxUnit += p->u_sampler2D.size();

		for (int i = 0; i < p->u_samplerCube.size(); i++) {
			p->shader->sendInt(p->u_samplerCube[i].first, p->maxUnit + i);
			p->u_samplerCube[i].second->set(p->maxUnit + i);
		}
		p->maxUnit += p->u_samplerCube.size();

		for (int i = 0; i < p->u_scene_params.size(); i++) {
			Pass::SceneParam type = p->u_scene_params[i].second;
			String name = p->u_scene_params[i].first;

			if (type == Pass::matTime)          { p->shader->sendFloat(name, GetScene()->matTime); }
			if (type == Pass::matLightIRadius) { p->shader->sendFloat(name, GetScene()->matLightIRadius); }

			if (type == Pass::matLightColor)    { p->shader->sendVec3(name, GetScene()->matLightColor); }
			if (type == Pass::matLightPosition) { p->shader->sendVec3(name, GetScene()->matLightPosition); }
			if (type == Pass::matLightDirection){ p->shader->sendVec3(name, GetScene()->matLightDirection); }
			if (type == Pass::matViewPosition)  { p->shader->sendVec3(name, GetScene()->matViewPosition); }

			if (type == Pass::matMVP) { p->shader->sendMat4(name, GetScene()->matMVP); }
			if (type == Pass::matWorld) { p->shader->sendMat4(name, GetScene()->matWorld); }
			if (type == Pass::matSpotTransform) { p->shader->sendMat4(name, GetScene()->matSpotTransform); }
			if (type == Pass::matViewportTransform) { p->shader->sendMat4(name, GetScene()->matViewportTransform); }

			if (type == Pass::matShadowMap && (GetCvars()->r_shadowtype)) {
				p->shader->sendInt(name, p->maxUnit); //Nick:TODO:Replace
				GetScene()->matShadowMap->set(p->maxUnit);
			}

			if (type == Pass::matViewportMap) {
				p->shader->sendInt(name, p->maxUnit + 1);
				GetScene()->matViewportMap->set(p->maxUnit + 1);
			}

			if (type == Pass::matSpotMap) {
				p->shader->sendInt(name, p->maxUnit + 2);
				GetScene()->matSpotMap->set(p->maxUnit + 2);
			}
		}

		return true;
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets material current pass
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Material::unsetPass() {
		Pass *p = currentPass;
		p->maxUnit = 0;

		p->shader->unset();

		for (int i = 0; i < p->u_sampler2D.size(); i++) {
			p->u_sampler2D[i].second->unset(p->maxUnit + i);
		}
		p->maxUnit += p->u_sampler2D.size();

		for (int i = 0; i < p->u_samplerCube.size(); i++) {
			p->u_samplerCube[i].second->unset(p->maxUnit + i);
		}
		p->maxUnit += p->u_samplerCube.size();

		if (GetCvars()->r_shadowtype)
			if (GetScene()->matShadowMap) GetScene()->matShadowMap->unset(p->maxUnit);

		if (GetScene()->matViewportMap) GetScene()->matViewportMap->unset(p->maxUnit + 1);
		if (GetScene()->matSpotMap) GetScene()->matSpotMap->unset(p->maxUnit + 2);
	}


	bool Material::passHasBlending(const String &name) {
		Pass *p = NULL;

		for (int i = 0; i < passes.size(); i++) {
			if (passes[i]->name == name) {
				p = passes[i];
			}
		}
		if (!p) return false;

		return p->hasBlending;
	}

	void Material::setPassBlending(const String &name) {
		Pass *p = NULL;

		for (int i = 0; i < passes.size(); i++) {
			if (passes[i]->name == name) {
				p = passes[i];
			}
		}
		if (!p) return;

		if (p->hasBlending) {
			GetRender()->depthMask(false);
			GetRender()->enableBlending(p->src, p->dst);
		}
	}

	void Material::setPassAlphaTest() {
	}

	void Material::unsetPassBlending() {
		Pass *p = currentPass;

		if (p->hasBlending) {
			GetRender()->disableBlending();
			GetRender()->depthMask(true);
		}
	}

	void Material::unsetPassAlphaTest() {
	}

}








