/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    Cache.cpp
* Desc:    Cache impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Cache.h"
#include "Log.h"
#include "CvarManager.h"
//**************************************

namespace VEGA {

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

	GLTexture *Cache::loadTexture2d(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::create2d(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLTexture *Cache::loadTexture2d(ILImage *image, const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::create2d(image);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLTexture *Cache::loadTextureCube(const String &path) {

		if (path == "") return NULL;
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if (it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::createCube(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLShader *Cache::loadShader(const String &path, const String &defines) {

		if (path == "") return NULL;
		std::map<String, std::pair<GLShader*, int>>::iterator it = shaders.find(path);
		if (it == shaders.end() || it->second.first == NULL) {
			String defines = "";
			if (cvars->r_reflections) defines += "#define REFLECTIONS\n";
			if (cvars->r_parallax) defines += "#define PARALLAX\n";
			if (cvars->r_specular) defines += "#define SPECULAR\n";
			if (cvars->r_shadowtype == 1) defines += "#define SM_SHADOWS\n";
			if (cvars->r_shadowtype == 2) defines += "#define SM_SHADOWS_PCF_2\n";
			if (cvars->r_shadowtype == 3) defines += "#define SM_SHADOWS_PCF_3\n";

			GLShader *shader = GLShader::create(path, defines);
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

		std::map<String, std::pair<GLShader*, int>>::iterator it;
		for (it = shaders.begin(); it != shaders.end(); it++) {
			delete it->second.first;

			it->second.first = GLShader::create(it->first, defines);
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

	void Cache::deleteTexture(GLTexture *texture) {
		std::map<String, std::pair<GLTexture*, int>>::iterator it;
		for (it = textures.begin(); it != textures.end(); it++) {
			if (it->second.first == texture && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cache::deleteShader(GLShader *shader) {
		std::map<String, std::pair<GLShader*, int>>::iterator it;
		for (it = shaders.begin(); it != shaders.end(); it++) {
			if (it->second.first == shader && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

}