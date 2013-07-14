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
#include "Cash.h"
#include "Scene.h"
 //***************************************************************************

namespace NGEngine {

//---------------------------------------------------------------------------
//Desc:    creates new material from file
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
Material::Material(String path) {
	path = "data/materials/" + path;

	//Check if exist
	if(!FileHelper::fileExist(path)) {
		Error::showAndExit("Material::Material() error: material file '" + path + "' not found");
		return;
	}
	
	//begin loading
	FILE *mFile = fopen(path.c_str(), "rt");
	
	while(!feof(mFile)) {
		String line = FileHelper::readString(mFile);
		
		if(StringHelper::getWord(line, 1) == "pass") {
			Pass *pass = new Pass();
			passes.push_back(pass);
			
			//pass name
			pass->name = StringHelper::getWord(line, 2);

			pass->hasBlending = false;
			pass->hasAlphaTest = false;
			pass->castShadows = true;
			pass->recieveShadows = true;
			
			while(StringHelper::getWord(line, 1) != "}" && !feof(mFile)) {
				line = FileHelper::readString(mFile);

				if(line[0] == '/' && line[1] == '/')
					continue;

				//cast_shadows
				if(StringHelper::getWord(line, 1) == "cast_shadows") {
					pass->castShadows = StringHelper::getInt(line, 3);
				}
				
				//recieve_shadows
				if(StringHelper::getWord(line, 1) == "recieve_shadows") {
					pass->recieveShadows = StringHelper::getInt(line, 3);
				}

				//blending
				if(StringHelper::getWord(line, 1) == "blending") {
					pass->hasBlending = true;
					
					if(StringHelper::getWord(line, 3) ==  "ONE") {
						pass->src = GLSystem::ONE;
					} else if(StringHelper::getWord(line, 3) ==  "ZERO") {
						pass->src = GLSystem::ZERO;
					} else if(StringHelper::getWord(line, 3) ==  "SRC_COLOR") {
						pass->src = GLSystem::SRC_COLOR;
					} else if(StringHelper::getWord(line, 3) ==  "DST_COLOR") {
						pass->src = GLSystem::DST_COLOR;
					} else if(StringHelper::getWord(line, 3) ==  "SRC_ALPHA") {
						pass->src = GLSystem::SRC_ALPHA;
					} else if(StringHelper::getWord(line, 3) ==  "DST_ALPHA") {
						pass->src = GLSystem::DST_ALPHA;
					} else if(StringHelper::getWord(line, 3) ==  "ONE_MINUS_SRC_COLOR") {
						pass->src = GLSystem::ONE_MINUS_SRC_COLOR;
					} else if(StringHelper::getWord(line, 3) ==  "ONE_MINUS_DST_COLOR") {
						pass->src = GLSystem::ONE_MINUS_DST_COLOR;
					} else if(StringHelper::getWord(line, 3) ==  "ONE_MINUS_SRC_ALPHA") {
						pass->src = GLSystem::ONE_MINUS_SRC_ALPHA;
					} else if(StringHelper::getWord(line, 3) ==  "ONE_MINUS_DST_ALPHA") {
						pass->src = GLSystem::ONE_MINUS_DST_ALPHA;
					} else {
						pass->hasBlending = false;
					}

					if(StringHelper::getWord(line, 4) ==  "ONE") {
						pass->dst = GLSystem::ONE;
					} else if(StringHelper::getWord(line, 4) ==  "ZERO") {
						pass->dst = GLSystem::ZERO;
					} else if(StringHelper::getWord(line, 4) ==  "SRC_COLOR") {
						pass->dst = GLSystem::SRC_COLOR;
					} else if(StringHelper::getWord(line, 4) ==  "DST_COLOR") {
						pass->dst = GLSystem::DST_COLOR;
					} else if(StringHelper::getWord(line, 4) ==  "SRC_ALPHA") {
						pass->dst = GLSystem::SRC_ALPHA;
					} else if(StringHelper::getWord(line, 4) ==  "DST_ALPHA") {
						pass->dst = GLSystem::DST_ALPHA;
					} else if(StringHelper::getWord(line, 4) ==  "ONE_MINUS_SRC_COLOR") {
						pass->dst = GLSystem::ONE_MINUS_SRC_COLOR;
					} else if(StringHelper::getWord(line, 4) ==  "ONE_MINUS_DST_COLOR") {
						pass->dst = GLSystem::ONE_MINUS_DST_COLOR;
					} else if(StringHelper::getWord(line, 4) ==  "ONE_MINUS_SRC_ALPHA") {
						pass->dst = GLSystem::ONE_MINUS_SRC_ALPHA;
					} else if(StringHelper::getWord(line, 4) ==  "ONE_MINUS_DST_ALPHA") {
						pass->dst = GLSystem::ONE_MINUS_DST_ALPHA;
					} else {
						pass->hasBlending = false;
					}
				}

				//alpha test
				if(StringHelper::getWord(line, 1) == "alpha_test") {
					pass->hasAlphaTest = true;
					if(StringHelper::getWord(line, 3) ==  "NEVER") {
						pass->alphaFunc = GLSystem::NEVER;
					} else if(StringHelper::getWord(line, 3) ==  "LESS") {
						pass->alphaFunc = GLSystem::LESS;
					} else if(StringHelper::getWord(line, 3) ==  "EQUAL") {
						pass->alphaFunc = GLSystem::EQUAL;
					} else if(StringHelper::getWord(line, 3) ==  "LEQUAL") {
						pass->alphaFunc = GLSystem::LEQUAL;
					} else if(StringHelper::getWord(line, 3) ==  "GREATER") {
						pass->alphaFunc = GLSystem::GREATER;
					} else if(StringHelper::getWord(line, 3) ==  "NOTEQUAL") {
						pass->alphaFunc = GLSystem::NOTEQUAL;
					} else if(StringHelper::getWord(line, 3) ==  "GEQUAL") {
						pass->alphaFunc = GLSystem::GEQUAL;
					} else if(StringHelper::getWord(line, 3) ==  "ALWAYS") {
						pass->alphaFunc = GLSystem::ALWAYS;
					} else {
						pass->hasAlphaTest = false;
					}

					pass->alphaRef = StringHelper::getFloat(line, 4);
				}

				//shader
				if(StringHelper::getWord(line, 1) == "shader") {
					String name = StringHelper::getWord(line, 3);

					String defines = "";
					pass->shader = engine.cash->loadShader("data/shaders/" + name, defines);
				}

				//sampler
				if(StringHelper::getWord(line, 1) == "sampler2D") {
					String name = StringHelper::getWord(line, 2);
					
					//standart params
					if(StringHelper::getWord(line, 4) == "scene::shadow_map") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matShadowMap)); 
					} else if(StringHelper::getWord(line, 4) == "scene::viewport_map") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewportMap));
					} else if(StringHelper::getWord(line, 4) == "scene::spot_map") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matSpotMap));

					//user params
					} else if(StringHelper::getWord(line, 4) == "normal_map()") {
						GLTexture *sampler;
						ILImage *map = ILImage::create2d("data/textures/" + StringHelper::getWord(line, 5));
						map->toNormalMap(4);
						sampler = engine.cash->loadTexture2d(map, "data/textures/" + StringHelper::getWord(line, 5) + "_NORMAL_MAP");
						pass->u_sampler2D.push_back(std::pair<String, GLTexture*>(name, sampler));
						delete map;
					} else {
						GLTexture *sampler;
						sampler = engine.cash->loadTexture2d("data/textures/" + StringHelper::getWord(line, 4));
						pass->u_sampler2D.push_back(std::pair<String, GLTexture*>(name, sampler));
					}
				}

				//sampler
				if(StringHelper::getWord(line, 1) == "samplerCube") {
					String name = StringHelper::getWord(line, 2);
					
					//standart params
					if(StringHelper::getWord(line, 4) == "scene::shadow_map") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matShadowMap)); 
					
					//user params
					} else {
						GLTexture *sampler;
						sampler = engine.cash->loadTextureCube("data/textures/" + StringHelper::getWord(line, 4));
						pass->u_samplerCube.push_back(std::pair<String, GLTexture*>(name, sampler));
					}
				}

				//float
				if(StringHelper::getWord(line, 1) == "float") {
					String name = StringHelper::getWord(line, 2);

					//standart params
					if(StringHelper::getWord(line, 4) == "scene::time") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matTime)); 
					} else if(StringHelper::getWord(line, 4) == "scene::light_iradius") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightIRadius));
					
					//user params
					} else {
						float value = StringHelper::getFloat(line, 4);
						pass->u_float.push_back(std::pair<String, float>(name, value));
					}
				}

				//Vec2
				if(StringHelper::getWord(line, 1) == "vec2") {
					String name = StringHelper::getWord(line, 2);

					//standart params
					if(StringHelper::getWord(line, 4) == "") {
					
					//user params
					} else {
						Vec2 value = Vec2(StringHelper::getFloat(line, 4), 
							StringHelper::getFloat(line, 5));

						pass->u_Vec2.push_back(std::pair<String, Vec2>(name, value));
					}
				}

				//Vec3
				if(StringHelper::getWord(line, 1) == "vec3") {
					String name = StringHelper::getWord(line, 2);

					//standart params
					if(StringHelper::getWord(line, 4) == "scene::light_color") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightColor)); 
					} else if(StringHelper::getWord(line, 4) == "scene::light_position") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightPosition));
					} else if(StringHelper::getWord(line, 4) == "scene::light_direction") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matLightDirection));
					}  else if(StringHelper::getWord(line, 4) == "scene::view_position") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewPosition));
		
					//user params
					} else {
					
						Vec3 value = Vec3(StringHelper::getFloat(line, 4), 
							StringHelper::getFloat(line, 5),
							StringHelper::getFloat(line, 6));

						pass->u_Vec3.push_back(std::pair<String, Vec3>(name, value));
					}
				}

				//Vec4
				if(StringHelper::getWord(line, 1) == "vec4") {
					String name = StringHelper::getWord(line, 2);
					//standart params
					if(StringHelper::getWord(line, 4) == "") {
					
					//user params
					} else {
						Vec4 value = Vec4(StringHelper::getFloat(line, 4), 
							StringHelper::getFloat(line, 5),
							StringHelper::getFloat(line, 6),
							StringHelper::getFloat(line, 7));

						pass->u_Vec4.push_back(std::pair<String, Vec4>(name, value));
					}
				}

				//Mat4
				if(StringHelper::getWord(line, 1) == "mat4") {
					String name = StringHelper::getWord(line, 2);
					
					//standart params
					if(StringHelper::getWord(line, 4) == "scene::mvp") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matMVP)); 
					} else if(StringHelper::getWord(line, 4) == "scene::world") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matWorld));
					} else if(StringHelper::getWord(line, 4) == "scene::spot_transform") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matSpotTransform));
					}  else if(StringHelper::getWord(line, 4) == "scene::viewport_transform") {
						pass->u_scene_params.push_back(std::pair<String, Pass::SceneParam>(name, Pass::matViewportTransform));

					//user params
					} else {
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
	fclose(mFile);
}

//---------------------------------------------------------------------------
//Desc:    destroys material
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
Material::~Material() {
	for(int p =0; p < passes.size(); p++) {
		Pass *pass = passes[p];
		for(int i = 0; i < pass->u_sampler2D.size(); i++) {
			engine.cash->deleteTexture(pass->u_sampler2D[i].second);
		}
		pass->u_sampler2D.clear();

		for(int i = 0; i < pass->u_samplerCube.size(); i++) {
			engine.cash->deleteTexture(pass->u_samplerCube[i].second);
		}
		pass->u_samplerCube.clear();

		pass->u_float.clear();
		pass->u_Vec2.clear();
		pass->u_Vec3.clear();
		pass->u_Vec4.clear();
		pass->u_Mat4.clear();

		engine.cash->deleteShader(pass->shader);
	}
}

//---------------------------------------------------------------------------
//Desc:    sets material pass
//Params:  name - pass name
//Returns: true - if pass found
//---------------------------------------------------------------------------
bool Material::setPass(const String &name) {
	Pass *p = NULL;

	for(int i = 0; i < passes.size(); i++) {
		if(passes[i]->name == name) {
			p = passes[i];
		}
	}
	if(!p) return false;
	
	currentPass = p;

	p->maxUnit = 0;

	p->shader->set();

	for(int i = 0; i < p->u_float.size(); i++) {
		p->shader->sendFloat(p->u_float[i].first, p->u_float[i].second);
	}

	for(int i = 0; i < p->u_Vec2.size(); i++) {
		p->shader->sendVec2(p->u_Vec2[i].first, p->u_Vec2[i].second);
	}

	for(int i = 0; i < p->u_Vec3.size(); i++) {
		p->shader->sendVec3(p->u_Vec3[i].first, p->u_Vec3[i].second);
	}

	for(int i = 0; i < p->u_Vec4.size(); i++) {
		p->shader->sendVec4(p->u_Vec4[i].first, p->u_Vec4[i].second);
	}

	for(int i = 0; i < p->u_Mat4.size(); i++) {
		p->shader->sendMat4(p->u_Mat4[i].first, p->u_Mat4[i].second);
	}

	for(int i = 0; i < p->u_sampler2D.size(); i++) {
		p->shader->sendInt(p->u_sampler2D[i].first, p->maxUnit + i);
		p->u_sampler2D[i].second->set(p->maxUnit + i);
	}
	p->maxUnit += p->u_sampler2D.size();

	for(int i = 0; i < p->u_samplerCube.size(); i++) {
		p->shader->sendInt(p->u_samplerCube[i].first, p->maxUnit + i);
		p->u_samplerCube[i].second->set(p->maxUnit + i);
	}
	p->maxUnit += p->u_samplerCube.size();

	for(int i = 0; i < p->u_scene_params.size(); i++) {
		Pass::SceneParam type = p->u_scene_params[i].second;
		String name = p->u_scene_params[i].first;

		if(type == Pass::matTime)          { p->shader->sendFloat(name, engine.scene->matTime); }
		if(type == Pass::matLightIRadius) { p->shader->sendFloat(name, engine.scene->matLightIRadius); }
		
		if(type == Pass::matLightColor)    { p->shader->sendVec3(name, engine.scene->matLightColor); }
		if(type == Pass::matLightPosition) { p->shader->sendVec3(name, engine.scene->matLightPosition); }
		if(type == Pass::matLightDirection){ p->shader->sendVec3(name, engine.scene->matLightDirection); }
		if(type == Pass::matViewPosition)  { p->shader->sendVec3(name, engine.scene->matViewPosition); }

		if(type == Pass::matMVP) { p->shader->sendMat4(name, engine.scene->matMVP); }
		if(type == Pass::matWorld) { p->shader->sendMat4(name, engine.scene->matWorld); }
		if(type == Pass::matSpotTransform) { p->shader->sendMat4(name, engine.scene->matSpotTransform); }
		if(type == Pass::matViewportTransform) { p->shader->sendMat4(name, engine.scene->matViewportTransform); }

		if(type == Pass::matShadowMap && (engine.config->getInt("light_shadowtype"))) { p->shader->sendInt(name, p->maxUnit); 
			engine.scene->matShadowMap->set(p->maxUnit); }

		if(type == Pass::matViewportMap) { p->shader->sendInt(name, p->maxUnit+1); 
			engine.scene->matViewportMap->set(p->maxUnit+1); }

		if(type == Pass::matSpotMap) { p->shader->sendInt(name, p->maxUnit+2); 
			engine.scene->matSpotMap->set(p->maxUnit+2); }
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

	for(int i = 0; i < p->u_sampler2D.size(); i++) {
		p->u_sampler2D[i].second->unset(p->maxUnit+i);
	}
	p->maxUnit += p->u_sampler2D.size();

	for(int i = 0; i < p->u_samplerCube.size(); i++) {
		p->u_samplerCube[i].second->unset(p->maxUnit + i);
	}
	p->maxUnit += p->u_samplerCube.size();

	if(engine.config->getInt("light_shadowtype")) 
		if(engine.scene->matShadowMap) engine.scene->matShadowMap->unset(p->maxUnit);

	if(engine.scene->matViewportMap) engine.scene->matViewportMap->unset(p->maxUnit+1);
	if(engine.scene->matSpotMap) engine.scene->matSpotMap->unset(p->maxUnit+2);
}


bool Material::passHasBlending(const String &name) {
	Pass *p = NULL;

	for(int i = 0; i < passes.size(); i++) {
		if(passes[i]->name == name) {
			p = passes[i];
		}
	}
	if(!p) return false;

	return p->hasBlending;
}

void Material::setPassBlending(const String &name) {
	Pass *p = NULL;

	for(int i = 0; i < passes.size(); i++) {
		if(passes[i]->name == name) {
			p = passes[i];
		}
	}
	if(!p) return;

	if(p->hasBlending) {
		engine.iRender->depthMask(false);
		engine.iRender->enableBlending(p->src, p->dst);
	}
}

void Material::setPassAlphaTest() {
}

void Material::unsetPassBlending() {
	Pass *p = currentPass;

	if(p->hasBlending) {
		engine.iRender->disableBlending();
		engine.iRender->depthMask(true);
	}
}

void Material::unsetPassAlphaTest() {
}

}








