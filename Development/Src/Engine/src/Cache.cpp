/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    Cache.cpp
* Desc:    Cache impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Cache.h"
#include "Log.h"
#include "CvarManager.h"
//**************************************

namespace NGTech {

	Cache::Cache(CVARManager*_cvars) : cvars(_cvars) {}

	Material *Cache::loadMaterial(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<Material*, int>>::iterator it = materials.find(path);
		if (it == materials.end() || it->second.first == NULL) {
			Debug("[Cache] loadMaterial: " + path);
			Material *material = new Material(path);
			materials[path].first = material;
			materials[path].second = 1;
			return material;
		}

		it->second.second++;
		return it->second.first;
	}

	Model *Cache::loadModel(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<Model*, int>>::iterator it = models.find(path);
		if (it == models.end() || it->second.first == NULL) {
			Model *model = new Model(path);
			models[path].first = model;
			models[path].second = 1;
			return model;
		}
		it->second.second++;
		return it->second.first;
	}

	ALSound *Cache::loadSound(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<ALSound*, int>>::iterator it = sounds.find(path);
		if (it == sounds.end() || it->second.first == NULL) {
			ALSound *sound = ALSound::create(path);
			sounds[path].first = sound;
			sounds[path].second = 1;
			return sound;
		}
		it->second.second++;
		return it->second.first;
	}

	I_Texture *Cache::loadTexture2d(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<I_Texture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			I_Texture *texture = GetRender()->TextureCreate2D(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	I_Texture *Cache::loadTexture2d(I_ILImage *image, const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<I_Texture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			I_Texture *texture = GetRender()->TextureCreate2D(image);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	I_Texture *Cache::loadTextureCube(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<I_Texture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			I_Texture *texture = GetRender()->TextureCreateCube(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	I_Shader *Cache::loadShader(const String &path, const String &defines) {

		if (path == "") return NULL;
		std::map<String, std::pair<I_Shader*, int>>::iterator it = shaders.find(path);
		if (it == shaders.end() || it->second.first == NULL) {
			String defines = "";
			if (cvars->r_reflections) defines += "#define REFLECTIONS\n";
			if (cvars->r_parallax) defines += "#define PARALLAX\n";
			if (cvars->r_specular) defines += "#define SPECULAR\n";
			if (cvars->r_shadowtype == 1) defines += "#define SM_SHADOWS\n";
			if (cvars->r_shadowtype == 2) defines += "#define SM_SHADOWS_PCF_2\n";
			if (cvars->r_shadowtype == 3) defines += "#define SM_SHADOWS_PCF_3\n";
			if (cvars->r_shadowtype == 4) defines += "#define VSM_SHADOWS\n";

			I_Shader *shader = GetRender()->ShaderCreate(path, defines);
			shaders[path].first = shader;
			shaders[path].second = 1;
			return shader;
		}
		it->second.second++;
		return it->second.first;
	}

	void Cache::reloadShaders() {
		String defines = "";
		if (cvars->r_reflections) defines += "#define REFLECTIONS\n";
		if (cvars->r_parallax) defines += "#define PARALLAX\n";
		if (cvars->r_specular) defines += "#define SPECULAR\n";
		if (cvars->r_shadowtype == 1) defines += "#define SM_SHADOWS\n";
		if (cvars->r_shadowtype == 2) defines += "#define SM_SHADOWS_PCF_2\n";
		if (cvars->r_shadowtype == 3) defines += "#define SM_SHADOWS_PCF_3\n";
		if (cvars->r_shadowtype == 4) defines += "#define VSM_SHADOWS\n";

		std::map<String, std::pair<I_Shader*, int>>::iterator it;
		for (it = shaders.begin(); it != shaders.end(); it++) {
			delete it->second.first;

			it->second.first = GetRender()->ShaderCreate(it->first, defines);
		}
	}



	void Cache::deleteMaterial(Material *material) {
		std::map<String, std::pair<Material*, int>>::iterator it;
		for (it = materials.begin(); it != materials.end(); it++) {
			if (it->second.first == material && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cache::deleteModel(Model *model) {
		std::map<String, std::pair<Model*, int>>::iterator it;
		for (it = models.begin(); it != models.end(); it++) {
			if (it->second.first == model && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cache::deleteSound(ALSound *sound) {
		std::map<String, std::pair<ALSound*, int>>::iterator it;
		for (it = sounds.begin(); it != sounds.end(); it++) {
			if (it->second.first == sound && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cache::deleteTexture(I_Texture *texture) {
		std::map<String, std::pair<I_Texture*, int>>::iterator it;
		for (it = textures.begin(); it != textures.end(); it++) {
			if (it->second.first == texture && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cache::deleteShader(I_Shader *shader) {
		std::map<String, std::pair<I_Shader*, int>>::iterator it;
		for (it = shaders.begin(); it != shaders.end(); it++) {
			if (it->second.first == shader && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}
}