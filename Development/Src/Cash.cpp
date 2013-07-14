/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Cash.h"
#include "Log.h"
#include "Config.h"
//**************************************

namespace NGEngine {

	Cash::Cash() {}

	Material *Cash::loadMaterial(const String &path) {
		std::map<String, std::pair<Material*, int>>::iterator it = materials.find(path);
		if(it == materials.end() || it->second.first == NULL) {
			Material *material = new Material(path);
			materials[path].first = material;
			materials[path].second = 1;
			return material;
		}

		it->second.second++;
		return it->second.first;
	}

	Model *Cash::loadModel(const String &path) {
		std::map<String, std::pair<Model*, int>>::iterator it = models.find(path);
		if(it == models.end() || it->second.first == NULL) {
			Model *model = new Model(path);
			models[path].first = model;
			models[path].second = 1;
			return model;
		}
		it->second.second++;
		return it->second.first;
	}

	ALSound *Cash::loadSound(const String &path) {
		std::map<String, std::pair<ALSound*, int>>::iterator it = sounds.find(path);
		if(it == sounds.end() || it->second.first == NULL) {
			ALSound *sound = ALSound::create(path);
			sounds[path].first = sound;
			sounds[path].second = 1;
			return sound;
		}
		it->second.second++;
		return it->second.first;
	}

	GLTexture *Cash::loadTexture2d(const String &path) {
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if(it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::create2d(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLTexture *Cash::loadTexture2d(ILImage *image, const String &path) {
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if(it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::create2d(image);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLTexture *Cash::loadTextureCube(const String &path) {
		std::map<String, std::pair<GLTexture*, int>>::iterator it = textures.find(path);
		if(it == textures.end() || it->second.first == NULL) {
			GLTexture *texture = GLTexture::createCube(path);
			textures[path].first = texture;
			textures[path].second = 1;
			return texture;
		}
		it->second.second++;
		return it->second.first;
	}

	GLShader *Cash::loadShader(const String &path, const String &defines) {
		std::map<String, std::pair<GLShader*, int>>::iterator it = shaders.find(path);
		if(it == shaders.end() || it->second.first == NULL) {
			String defines = "";
			if(engine.config->getBool("light_specular") == true) defines += "#define SPECULAR\n";
			if(engine.config->getInt("light_shadowtype") == 1) defines += "#define SM_SHADOWS\n";
			if(engine.config->getInt("light_shadowtype") == 2) defines += "#define SM_SHADOWS_PCF_2\n";
			if(engine.config->getInt("light_shadowtype") == 3) defines += "#define SM_SHADOWS_PCF_3\n";

			GLShader *shader= GLShader::create(path, defines);
			shaders[path].first = shader;
			shaders[path].second = 1;
			return shader;
		}
		it->second.second++;
		return it->second.first;
	}

	void Cash::reloadShaders() {
		String defines = "";
		if(engine.config->getBool("light_specular") == true) defines += "#define SPECULAR\n";
		if(engine.config->getInt("light_shadowtype") == 1) defines += "#define SM_SHADOWS\n";
		if(engine.config->getInt("light_shadowtype") == 2) defines += "#define SM_SHADOWS_PCF_2\n";
		if(engine.config->getInt("light_shadowtype") == 3) defines += "#define SM_SHADOWS_PCF_3\n";

		std::map<String, std::pair<GLShader*, int>>::iterator it;
		for(it = shaders.begin(); it != shaders.end(); it++) {
			delete it->second.first;

			it->second.first = GLShader::create(it->first, defines);
		}
	}



	void Cash::deleteMaterial(Material *material) {
		std::map<String, std::pair<Material*, int>>::iterator it;
		for(it = materials.begin(); it != materials.end(); it++) {
			if(it->second.first == material && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cash::deleteModel(Model *model) {
		std::map<String, std::pair<Model*, int>>::iterator it;
		for(it = models.begin(); it != models.end(); it++) {
			if(it->second.first == model && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cash::deleteSound(ALSound *sound) {
		std::map<String, std::pair<ALSound*, int>>::iterator it;
		for(it = sounds.begin(); it != sounds.end(); it++) {
			if(it->second.first == sound && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cash::deleteTexture(GLTexture *texture) {
		std::map<String, std::pair<GLTexture*, int>>::iterator it;
		for(it = textures.begin(); it != textures.end(); it++) {
			if(it->second.first == texture && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

	void Cash::deleteShader(GLShader *shader) {
		std::map<String, std::pair<GLShader*, int>>::iterator it;
		for(it = shaders.begin(); it != shaders.end(); it++) {
			if(it->second.first == shader && it->second.second <= 1) {
				delete it->second.first;
				it->second.first = NULL;
			}
		}
	}

}