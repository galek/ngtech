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
#include <vector>
//**************************************

namespace NGTech {

	class AudioUpdateJob;
	//---------------------------------------------------------------------------
	//Desc: class of the scene. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API Scene {
	public:
		explicit Scene(CVARManager*_cvars);

		~Scene();

		void clear();

		void initialise();
		/**
		main update function
		*/
		void update(bool _paused);
		void updateSound();
		/**
		main draw function
		*/
		void render();

		void setGravity(const Vec3 &gravity);

		void setAmbient(const Vec3 &color);
		void setCamera(Camera *camera);
		void setTerrain(const String &path, float step, float height, bool physics);
		void setTerrainLods(const Vec3 &lods);
		void setTerrainMaterial(const String &path);

		void addObject(Object *object);
		void addLight(Light *light);
		void addParticleSystem(ParticleSystem *system);

		void deleteObjectMesh(ObjectMesh *object);
		void deleteLight(Light *light);
		void deleteParticleSystem(ParticleSystem *system);

		bool _cameraCheckValid();

		void reloadShaders();

		/**
		*/
		ENGINE_INLINE size_t ObjCount() { return objects.size(); }
		/**
		*/
		ENGINE_INLINE size_t LightCount() { return lights.size(); }
		/**
		*/
		ENGINE_INLINE Light *GetCurrentLight() { return currentLight; }
		/**
		*/
		ENGINE_INLINE Camera *GetCurrentCamera() { return currentCamera; }
	public:
		void runUpdate();
		void waitUpdate();

	private:
		void testInteserction();
		void drawAmbient(bool blended);

		void drawPoint(LightPoint *light, bool blended);
		void drawSpot(LightSpot *light, bool blended);
		void drawDirect(LightDirect *light, bool blended);

		void getPointShadowMap(LightPoint *light);
		void getSpotShadowMap(LightSpot *light);

		void checkPointVisibility(LightPoint *light);
		void checkSpotVisibility(LightSpot *light);
		void _CheckFrustum();
		void _RenderLight(Light* _light, bool _blend);
		void _RenderAnimation();

		Vec3 ambient;
		Frustum *frustum;
		Terrain *terrain;
		Vec3 terrainLods;

		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::vector<ParticleSystem*> systems;

		I_FBOManager *viewportFBO;
		I_FBOManager *shadowFBO;

		I_Texture *viewportCopy;
		I_Texture *hdrViewportCopy;
		I_Texture *viewportCopy_brightPass_blured;

		Material *depthPass;
		Material *hdr;

		float matTime;
		float matLightIRadius;

		Vec4 matLightColor;
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
		//current state
		Camera *currentCamera;
		Light *currentLight;
		Material *currentMaterial;
	private:
		friend class ParticleSystem;
		friend class Flare;
		friend class ParticleSystem;

		friend class Light;
		friend class LightPoint;
		friend class LightSpot;
		friend class LightDirect;

		friend class Material;
	private:
		AudioUpdateJob* mUpdateJob;
		int update_id; // update identifier
	};
}