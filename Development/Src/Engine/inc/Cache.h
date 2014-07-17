/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "../Common/EString.h"
#include "Model.h"
#include "Material.h"
#include "ALSound.h"
#include <vector>
#include <map>
//**************************************

namespace NGTech {

	class Cache {
	public:
		explicit Cache(CVARManager*_cvars);

		Material *loadMaterial(const String &path);
		Model *loadModel(const String &path);
		ALSound *loadSound(const String &path);
		I_Texture *loadTexture2d(const String &path);
		I_Texture *loadTexture2d(I_ILImage *image, const String &path);
		I_Texture *loadTextureCube(const String &path);

		GLShader *loadShader(const String &path, const String &defines);
		void reloadShaders();

		void deleteMaterial(Material *material);
		void deleteModel(Model *model);
		void deleteSound(ALSound *sound);
		void deleteTexture(I_Texture *texture);
		void deleteShader(GLShader *shader);

	private:
		std::map<String, std::pair<Material*, int>> materials;
		std::map<String, std::pair<Model*, int>> models;
		std::map<String, std::pair<ALSound*, int>> sounds;
		std::map<String, std::pair<I_Texture*, int>> textures;
		std::map<String, std::pair<GLShader*, int>> shaders;
		struct CVARManager*cvars;
	};
}