/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#include "Light.h"
#include "ParticleSystem.h"
#include "Object.h"
#include "Terrain.h"
#include "Camera.h"
#include "Water.h"
#include <vector>
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the scene. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API Scene {
	public:
		explicit Scene(CVARManager*_cvars);

		~Scene();

		void clear();

		void initialise();
		void Update();

		void setGravity(const Vec3 &gravity);

		void setAmbient(const Vec3 &color);
		void setCamera(Camera *camera);
		void setWater(float depth, float size);
		void setTerrain(const String &path, float step, float height, bool physics);
		void setTerrainLods(const Vec3 &lods);
		void setTerrainMaterial(const String &path);

		void addObject(Object *object);
		void addLight(Light *light);
		void addParticleSystem(ParticleSystem *system);

		void deleteObjectMesh(ObjectMesh *object);
		void deleteLight(Light *light);
		void deleteParticleSystem(ParticleSystem *system);

		void reloadShaders();

		ENGINE_INLINE Camera *GetActiveCamera() { return camera; }
	private:
		void drawAmbient(bool blended);

		void drawOmni(LightOmni *light, bool blended);
		void drawSpot(LightSpot *light, bool blended);
		void drawDirect(LightDirect *light, bool blended);

		void getOmniShadowMap(LightOmni *light);
		void getSpotShadowMap(LightSpot *light);

		void checkOmniVisibility(LightOmni *light);
		void checkSpotVisibility(LightSpot *light);

		void drawWater();

		Vec3 ambient;
		Camera *camera;
		Water *water;
		Terrain *terrain;
		Vec3 terrainLods;

		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::vector<ParticleSystem*> systems;

		I_FBOManager *viewportFBO;
		I_FBOManager *shadowFBO;

		I_Texture *viewportCopy;
		I_Texture *viewportCopy_brightPass;
		I_Texture *viewportCopy_brightPass_blured;

		Material *depthPass;
		Material *waterMtr;
		Material *hdr;

		float matTime;
		float matLightIRadius;

		Vec3 matLightColor;
		Vec3 matLightPosition;
		Vec3 matLightDirection;
		Vec3 matViewPosition;

		Mat4 matMVP;
		Mat4 matWorld;
		Mat4 matViewportTransform;
		Mat4 matSpotTransform;

		I_Texture *matShadowMap;
		I_Texture *matViewportMap;
		I_Texture *matSpotMap;

		I_OcclusionQuery *query;

		ObjectMesh *sphere;
		struct CVARManager*cvars;

		friend class ParticleSystem;
		friend class Flare;
		friend class ParticleSystem;

		friend class Light;
		friend class LightOmni;
		friend class LightSpot;
		friend class LightDirect;

		friend class Material;
	};
}