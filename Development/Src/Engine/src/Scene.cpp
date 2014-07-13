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
#include "GUI.h"
#include "Scene.h"
#include "Config.h"
#include "Log.h"
#include "ALSystem.h"
#include "WindowSystem.h"
#include "Cache.h"
#include "CvarManager.h"
#include "SkinnedModel.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates new scene with ambient light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Scene::Scene(CVARManager*_cvars) : cvars(_cvars),camera(nullptr) {
		water = NULL;
		terrain = NULL;
	}
	//---------------------------------------------------------------------------
	//Desc:    Init engine sub-systems
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------

	void Scene::initialise(){
		viewportFBO = GLFBO::create(512, 512);
		viewportFBO->createDepthAttachment();

		int size = GetEngine()->cvars->r_shadowsize;
		shadowFBO = GLFBO::create(size, size);
		shadowFBO->createDepthAttachment();

		viewportCopy = GLTexture::create2d(512, 512, GLTexture::RGBA);
		viewportCopy->setWrap(GLTexture::CLAMP_TO_EDGE);
		viewportCopy->setFilter(GLTexture::LINEAR);

		viewportCopy_brightPass = GLTexture::create2d(512, 512, GLTexture::RGBA);
		viewportCopy_brightPass->setWrap(GLTexture::CLAMP_TO_EDGE);
		viewportCopy_brightPass->setFilter(GLTexture::LINEAR);

		viewportCopy_brightPass_blured = GLTexture::create2d(512, 512, GLTexture::RGBA);
		viewportCopy_brightPass_blured->setWrap(GLTexture::CLAMP_TO_EDGE);
		viewportCopy_brightPass_blured->setFilter(GLTexture::LINEAR);

		query = new GLOcclusionQuery();

		sphere = new ObjectMesh("sphere.amdl");

		matViewportMap = NULL;
		matShadowMap = NULL;
		matSpotMap = NULL;

		depthPass = new Material("engine_materials/depth_pass.amtr");
		waterMtr = new Material("engine_materials/water.amtr");
		hdr = new Material("engine_materials/hdr.amtr");
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys scene
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Scene::~Scene() {
		clear();
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys scene
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::clear() {
		systems.clear();
		objects.clear();
		lights.clear();

		delete terrain;
		delete water;
	}

	//---------------------------------------------------------------------------
	//Desc:    reload all shaders
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::reloadShaders() {
		GetEngine()->cache->reloadShaders();
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the scene gravity value
	//Params:  gravity - gravity value
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setGravity(const Vec3 &gravity) {
		this->gravity = gravity;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the ambient color of the scene
	//Params:  ambient - pointer to LightAmbient
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setAmbient(const Vec3 &color) {
		ambient = color;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the camera of the scene
	//Params:  camera - pointer to camera
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setCamera(Camera *camera) {
		delete this->camera;
		this->camera = camera;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets water depth and size
	//Params:  
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setWater(float depth, float size) {
		if (!water) water = new Water();
		water->setDepth(depth);
		water->setSize(size);
	}

	//---------------------------------------------------------------------------
	//Desc:    creates terrain
	//Params:  
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setTerrain(const String &path, float step, float height, bool physics) {
		terrain = new Terrain(path, step, height, 16);
		terrainLods = Vec3(100, 200, 400);
		if (physics) terrain->setPhysics();
	}

	//---------------------------------------------------------------------------
	//Desc:    sets LODS for the terrain
	//Params:  lods - lods vector
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setTerrainLods(const Vec3 &lods) {
		terrain->setLods(lods);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets material for the terrain
	//Params:  mtr - material to set
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::setTerrainMaterial(const String &path) {
		if (terrain) terrain->setMaterial(path);
	}

	//---------------------------------------------------------------------------
	//Desc:    adds new light to scene
	//Params:  light - pointer to Light to add
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::addLight(Light *light) {
		lights.push_back(light);
	}


	//---------------------------------------------------------------------------
	//Desc:    adds new object to scene
	//Params:  object - pointer to ObjectMeshs to add
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::addObject(Object *object) {
		objects.push_back(object);
	}

	//---------------------------------------------------------------------------
	//Desc:    adds new system to scene
	//Params:  system - pointer to ParticleSystem to add
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::addParticleSystem(ParticleSystem *system) {
		systems.push_back(system);
	}

	//---------------------------------------------------------------------------
	//Desc:    removes Light from scene
	//Params:  light - pointer to Light to remove
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::deleteLight(Light *light) {
		for (int i = 0; i < lights.size(); i++) {
			if (light == lights[i]) {
				lights[i] = NULL;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    removes ObjectMesh from scene
	//Params:  object - pointer to ObjectMesh to remove
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::deleteObjectMesh(ObjectMesh *object) {
		for (int i = 0; i < objects.size(); i++) {
			if (object == objects[i]) {
				objects[i] = NULL;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    removes ParticleSystem from scene
	//Params:  system - pointer to ParticleSystem to remove
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::deleteParticleSystem(ParticleSystem *system) {
		for (int i = 0; i < systems.size(); i++) {
			if (system == systems[i]) {
				systems[i] = NULL;
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    draws the ambient pass
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::drawAmbient(bool blended) {
		Frustum frustum;
		matLightColor = ambient;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				frustum.get();

				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matLightColor = ambient;
				if (!mtr->setPass("Ambient")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum.isInside(terrain->getMin(n), terrain->getMax(n)))
						continue;

					terrain->drawNode(n, camera->getPosition());
				}

				mtr->unsetPass();
			}
		}

		//DRAW OBJECTS
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(object->getTransform());

			frustum.get();
			if (!frustum.isInside(object->getCenter(), object->getRadius())) {
				GetEngine()->iRender->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				if (!frustum.isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matWorld = object->getTransform();

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
					mtr->setPassBlending("Ambient");
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}

				if (!mtr->setPass("Ambient")) continue;

				object->drawSubset(s);

				if (mtr->passHasBlending("Ambient") && blended)
					mtr->unsetPassBlending();
				mtr->unsetPass();
			}

			GetEngine()->iRender->pop();
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    draws omni lights
	//Params:  light - omni light to draw
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::drawOmni(LightOmni *light, bool blended) {
		if (!light->visible) return;

		/*int x, y, z, w;
		light->getScissorRect(camera->getPosition(), x, y, z, w);
		GetEngine()->iRender->enableScissor(x, y, z, w);

		GetEngine()->iRender->enable2d(false);
		glColor3f(0, 1, 0);
		GetEngine()->iRender->disableDepth();
		z += x;
		w += y;
		glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x, w);
		glVertex2i(z, y);
		glVertex2i(z, w);

		glVertex2i(x, y);
		glVertex2i(z, y);
		glVertex2i(x, w);
		glVertex2i(z, w);
		glEnd();
		GetEngine()->iRender->enableDepth();
		GetEngine()->iRender->enable3d();*/

		Frustum frustum;

		//set material params
		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;
		matViewPosition = camera->getPosition();
		matLightColor = light->color;
		matShadowMap = light->shadowMap;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				frustum.get();

				matMVP = GetEngine()->iRender->getMatrix_MVP();
				if (!mtr->setPass("LightOmni")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum.isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}
					if ((light->position - terrain->getCenter(n)).length() >
						light->radius + terrain->getRadius(n)) {
							continue;
					}

					terrain->drawNode(n, camera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(object->getTransform());

			frustum.get();
			if (!frustum.isInside(object->getCenter(), object->getRadius())) {
				GetEngine()->iRender->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
					GetEngine()->iRender->pop();
					continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				frustum.get();
				if (!frustum.isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matWorld = object->getTransform();

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				if (!mtr->setPass("LightOmni")) continue;

				object->drawSubset(s);

				mtr->unsetPass();
			}
			GetEngine()->iRender->pop();
		}


		//GetEngine()->iRender->disableScissor();
	}

	//---------------------------------------------------------------------------
	//Desc:    gets omni light shadow map
	//Params:  light - omni light to get shadow map
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::getOmniShadowMap(LightOmni *light) {
		if (!light->visible || !light->castShadows || !cvars->r_shadowtype) {
			return;
		}

		Frustum frustum;

		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->push();
		GetEngine()->iRender->loadMatrix(Mat4::perspective(90, 1, 1, light->radius));

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->push();

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;

		for (int f = 0; f < 6; f++) {
			shadowFBO->set();
			shadowFBO->setColorTarget(light->shadowMap, f);
			shadowFBO->clear();
			//GetEngine()->iRender->clearColor(Vec3(1.0, 1.0, 1.0));

			GetEngine()->iRender->loadMatrix(Mat4::cube(light->position, f));

			//DRAW OBJECTS
			for (int m = 0; m < objects.size(); m++) {
				if (!objects[m]) {
					continue;
				}
				if (objects[m]->getType() == Object::OBJECT) {
					continue;
				}
				Object *object = objects[m];

				GetEngine()->iRender->push();
				GetEngine()->iRender->multMatrix(object->getTransform());

				frustum.get();
				if (!frustum.isInside(object->getCenter(), object->getRadius())) {
					GetEngine()->iRender->pop();
					continue;
				}
				if ((light->position - object->getTransform().getTranslation()).length() >
					light->radius + object->getRadius()) {
						GetEngine()->iRender->pop();
						continue;
				}

				//DRAW OBJECT SUBSETS
				for (int s = 0; s < object->getNumSubsets(); s++) {
					Material *mtr = object->getMaterial(s);
					if (!mtr) continue;

					frustum.get();
					if (!frustum.isInside(object->getCenter(s), object->getRadius(s))) {
						continue;
					}

					//set material params
					matMVP = GetEngine()->iRender->getMatrix_MVP();
					matWorld = object->getTransform();
					depthPass->setPass("DepthPass");

					object->drawSubset(s);

					depthPass->unsetPass();
				}
				GetEngine()->iRender->pop();
			}
			shadowFBO->flush();
			shadowFBO->unset();
		}

		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->pop();

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->pop();
	}

	//---------------------------------------------------------------------------
	//Desc:    draws spot light
	//Params:  light - spot light to draw
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::drawSpot(LightSpot *light, bool blended) {
		if (!light->visible) return;

		Frustum frustum;

		light->projTransform = Mat4::texBias() *
			Mat4::perspective(light->fov, 1, 1, light->radius) *
			Mat4::lookAt(light->position, light->position + light->direction, Vec3(0, 1, 0));

		//set material params
		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;
		matLightDirection = light->direction;
		matViewPosition = camera->getPosition();
		matLightColor = light->color;
		matShadowMap = light->shadowMap;
		matSpotMap = light->projMap;
		matSpotTransform = light->projTransform;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				frustum.get();

				//set material params
				matMVP = GetEngine()->iRender->getMatrix_MVP();
				if (!mtr->setPass("LightSpot")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{

					if (!frustum.isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}
					if ((light->position - terrain->getCenter(n)).length() >
						light->radius + terrain->getRadius(n)) {
							continue;
					}

					terrain->drawNode(n, camera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		//DRAW OBJECTS
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(object->getTransform());

			frustum.get();
			if (!frustum.isInside(object->getCenter(), object->getRadius())) {
				GetEngine()->iRender->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
					GetEngine()->iRender->pop();
					continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				frustum.get();
				if (!frustum.isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matWorld = object->getTransform();
				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				if (!mtr->setPass("LightSpot")) continue;

				object->drawSubset(s);

				mtr->unsetPass();
			}
			GetEngine()->iRender->pop();
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    draws spot light shadow map
	//Params:  light - spot light to get shadow map
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::getSpotShadowMap(LightSpot *light) {
		if (!light->visible || !light->castShadows || !cvars->r_shadowtype) {
			return;
		}

		Frustum frustum;

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;

		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->push();
		GetEngine()->iRender->loadMatrix(Mat4::perspective(light->fov, 1, 1, light->radius));

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->push();
		GetEngine()->iRender->loadMatrix(Mat4::lookAt(light->position, light->position + light->direction, Vec3(0, 1, 0)));

		shadowFBO->set();
		shadowFBO->setColorTarget(light->shadowMap);
		shadowFBO->clear();
		GetEngine()->iRender->clearColor(Vec3(1.0, 1.0, 1.0));

		//DRAW OBJECTS
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(object->getTransform());

			frustum.get();
			if (!frustum.isInside(object->getCenter(), object->getRadius())) {
				GetEngine()->iRender->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
					GetEngine()->iRender->pop();
					continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				Material *mtr = object->getMaterial(s);

				if (!mtr) continue;

				frustum.get();
				if (!frustum.isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matWorld = object->getTransform();
				depthPass->setPass("DepthPass");

				object->drawSubset(s);

				depthPass->unsetPass();
			}
			GetEngine()->iRender->pop();
		}

		shadowFBO->flush();
		shadowFBO->unset();

		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->pop();

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->pop();
	}

	//---------------------------------------------------------------------------
	//Desc:    draws direct light
	//Params:  light - direct light to draw
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::drawDirect(LightDirect *light, bool blended) {
		Frustum frustum;

		//set material params
		matLightDirection = light->direction;
		matViewPosition = camera->getPosition();
		matLightColor = light->color;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				//set material params
				matMVP = GetEngine()->iRender->getMatrix_MVP();
				if (!mtr->setPass("LightDirect")) return;

				frustum.get();

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum.isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}

					terrain->drawNode(n, camera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		//DRAW OBJECTS
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(object->getTransform());

			frustum.get();
			if (!frustum.isInside(object->getCenter(), object->getRadius())) {
				GetEngine()->iRender->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				if (!frustum.isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetEngine()->iRender->getMatrix_MVP();
				matWorld = object->getTransform();

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				if (!mtr->setPass("LightDirect")) continue;

				object->drawSubset(s);

				mtr->unsetPass();
			}
			GetEngine()->iRender->pop();
		}
	}


	//---------------------------------------------------------------------------
	//Desc:    checks omni light visibility
	//Params:  light - omni light to check
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::checkOmniVisibility(LightOmni *light) {
		Frustum frustum;

		frustum.get();
		if (!frustum.isInside(light->position, light->radius)) {
			light->visible = false;
			return;
		}

		if ((light->position - camera->getPosition()).length() > light->radius) {
			GetEngine()->iRender->colorMask(false, false, false, false);
			GetEngine()->iRender->depthMask(false);
			//glColor3f(0, 0, 1);
			query->beginRendering();

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(Mat4::translate(light->position) *
				Mat4::scale(Vec3(light->radius*0.2, light->radius*0.2, light->radius*0.2)));

			sphere->drawSubset(0);

			GetEngine()->iRender->pop();

			query->endRendering();

			GetEngine()->iRender->depthMask(true);
			GetEngine()->iRender->colorMask(true, true, true, true);

			if (query->getResult() < 2) {
				light->visible = false;
				return;
			}
		}
		light->visible = true;
	}

	//---------------------------------------------------------------------------
	//Desc:    checks spot light visibility
	//Params:  light - spot light to check
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::checkSpotVisibility(LightSpot *light) {
		Frustum frustum;

		frustum.get();
		if (!frustum.isInside(light->position, light->radius)) {
			light->visible = false;
			return;
		}

		if ((light->position - camera->getPosition()).length() > light->radius) {
			GetEngine()->iRender->colorMask(false, false, false, false);
			GetEngine()->iRender->depthMask(false);

			query->beginRendering();

			GetEngine()->iRender->push();
			GetEngine()->iRender->multMatrix(Mat4::translate(light->position) *
				Mat4::scale(Vec3(light->radius*0.2, light->radius*0.2, light->radius*0.2)));

			sphere->drawSubset(0);

			GetEngine()->iRender->pop();

			query->endRendering();

			GetEngine()->iRender->depthMask(true);
			GetEngine()->iRender->colorMask(true, true, true, true);

			if (query->getResult() < 2) {
				light->visible = false;
				return;
			}
		}
		light->visible = true;
	}


	//---------------------------------------------------------------------------
	//Desc:    main draw function
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Scene::Update() {
		//---------update-camera-----------------------------------
		camera->update();
		GetEngine()->alSystem->setListener(camera->getPosition(), camera->getDirection());

		//---------set-gravity-----------------------------------
		for (int k = 0; k < objects.size(); k++) {
			if (objects[k]->getPhysBody()) {
				objects[k]->getPhysBody()->AddTorque(gravity);
				if (water) {
					if (objects[k]->getTransform().getTranslation().y < water->getDepth() - 2.5) {
						float volume = 4 / 3 * PI * pow(objects[k]->getRadius(), 3.0f);
						float d = objects[k]->getPhysBody()->GetMass() / volume;
						objects[k]->getPhysBody()->AddTorque(-gravity / d * 0.02);
					}
				}
			}

			if (camera->getPhysBody()) camera->getPhysBody()->AddTorque(gravity);
		}

		//---------draw-scene--------------------------------
		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->loadMatrix(camera->getProjection());

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->loadMatrix(camera->getTransform());

		matTime = GetEngine()->iWindow->getETime();
		matViewportMap = viewportCopy;
		matViewportTransform = Mat4::texBias() * GetEngine()->iRender->getMatrix_MVP();

		drawAmbient(false);

		for (int i = 0; i < lights.size(); i++) {
			if (!lights[i]->enabled) continue;
			if (lights[i]->getType() == Light::LIGHT_OMNI) {
				checkOmniVisibility((LightOmni*) lights[i]);
			}
			else if (lights[i]->getType() == Light::LIGHT_SPOT) {
				checkSpotVisibility((LightSpot*) lights[i]);
			}
		}

		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->getType() == Light::LIGHT_OMNI) {
				getOmniShadowMap((LightOmni*) lights[i]);
			}
			else if (lights[i]->getType() == Light::LIGHT_SPOT) {
				getSpotShadowMap((LightSpot*) lights[i]);
			}
		}

		GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetEngine()->iRender->depthMask(false);

		//draw wireframe
		if (GetEngine()->cvars->r_wireframe) {
			glColor3f(1, 1, 1);//Nick:TODO:Replace
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Nick:TODO:Replace

			for (int i = 0; i < objects.size(); i++) {
				GetEngine()->iRender->push();
				GetEngine()->iRender->multMatrix(objects[i]->getTransform());
				for (int k = 0; k < objects[i]->getNumSubsets(); k++) {
					objects[i]->drawSubset(k);
				}
				GetEngine()->iRender->pop();
			}
			if (terrain) {
				for (int n = 0; n < terrain->getNumNodes(); n++) {
					terrain->drawNode(n, camera->getPosition());
				}
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//Nick:TODO:Replace
		}

		//draw lighting
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->getType() == Light::LIGHT_OMNI) {
				drawOmni((LightOmni*) lights[i], false);
			}
			else if (lights[i]->getType() == Light::LIGHT_SPOT) {
				drawSpot((LightSpot*) lights[i], false);
			}
			else if (lights[i]->getType() == Light::LIGHT_DIRECT) {
				drawDirect((LightDirect*) lights[i], false);
			}
		}

		GetEngine()->iRender->depthMask(true);
		GetEngine()->iRender->disableBlending();

		drawAmbient(true);

		/*GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetEngine()->iRender->depthMask(false);

		for(int i = 0; i < lights.size(); i++) {
		if(lights[i]->getType() == Light::LIGHT_OMNI) {
		drawOmni((LightOmni*)lights[i], true);
		} else if(lights[i]->getType() == Light::LIGHT_SPOT) {
		drawSpot((LightSpot*)lights[i], true);
		} else if(lights[i]->getType() == Light::LIGHT_DIRECT) {
		drawDirect((LightDirect*)lights[i], true);
		}
		}

		GetEngine()->iRender->depthMask(true);
		GetEngine()->iRender->disableBlending();*/

		//draw particle systems
		for (int k = 0; k < systems.size(); k++) {
			if (systems[k]) systems[k]->draw();
		}

		//---------draw-scene-into-viewport-copy-------------------------------
		viewportFBO->set();
		viewportFBO->setColorTarget(viewportCopy);
		viewportFBO->clear();

		//---------set-camera--------------------------------
		GetEngine()->iRender->setMatrixMode_Projection();
		GetEngine()->iRender->loadMatrix(camera->getProjection());

		GetEngine()->iRender->setMatrixMode_Modelview();
		GetEngine()->iRender->loadMatrix(camera->getTransform());

		drawAmbient(false);

		GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetEngine()->iRender->depthMask(false);

		if (GetEngine()->cvars->r_wireframe) {//Nick:TODO:Replace
			glColor3f(1, 1, 1);//Nick:TODO:Replace
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Nick:TODO:Replace

			for (int i = 0; i < objects.size(); i++) {
				GetEngine()->iRender->push();
				GetEngine()->iRender->multMatrix(objects[i]->getTransform());
				for (int k = 0; k < objects[i]->getNumSubsets(); k++) {
					objects[i]->drawSubset(k);
				}
				GetEngine()->iRender->pop();
			}
			if (terrain) {
				for (int n = 0; n < terrain->getNumNodes(); n++) {
					terrain->drawNode(n, camera->getPosition());
				}
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//Nick:TODO:Replace
		}

		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->getType() == Light::LIGHT_OMNI) {
				drawOmni((LightOmni*) lights[i], false);
			}
			else if (lights[i]->getType() == Light::LIGHT_SPOT) {
				drawSpot((LightSpot*) lights[i], false);
			}
			else if (lights[i]->getType() == Light::LIGHT_DIRECT) {
				drawDirect((LightDirect*) lights[i], false);
			}
		}

		drawAmbient(true);
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->getType() == Light::LIGHT_OMNI) {
				drawOmni((LightOmni*) lights[i], true);
			}
			else if (lights[i]->getType() == Light::LIGHT_SPOT) {
				drawSpot((LightSpot*) lights[i], true);
			}
			else if (lights[i]->getType() == Light::LIGHT_DIRECT) {
				drawDirect((LightDirect*) lights[i], true);
			}
		}

		GetEngine()->iRender->depthMask(true);
		GetEngine()->iRender->disableBlending();

		for (int k = 0; k < systems.size(); k++) {
			if (systems[k]) systems[k]->draw();
		}

		viewportFBO->flush();
		viewportFBO->unset();

		if (water) 	{
			matMVP = GetEngine()->iRender->getMatrix_MVP();
			waterMtr->setPass("Ambient");		
			water->draw();		
			waterMtr->unsetPass();
		}

		//Now GUI Update
		if (GetEngine()->gui)
			GetEngine()->gui->update();


		if (GetEngine()->cvars->r_hdr) {
			//---------bright-pass--------------------------------
			viewportFBO->set();
			viewportFBO->setColorTarget(viewportCopy_brightPass);
			viewportFBO->clear();

			GetEngine()->iRender->enable2d(true);

			matViewportMap = viewportCopy;
			hdr->setPass("BrightPass");
			GetEngine()->iRender->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetEngine()->iRender->enable3d();

			viewportFBO->flush();
			viewportFBO->unset();

			//---------bright-pass-blur--------------------------------
			viewportFBO->set();
			viewportFBO->setColorTarget(viewportCopy_brightPass_blured);
			viewportFBO->clear();

			GetEngine()->iRender->enable2d(true);

			matViewportMap = viewportCopy_brightPass;
			hdr->setPass("BlurPass");
			GetEngine()->iRender->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetEngine()->iRender->enable3d();

			viewportFBO->flush();
			viewportFBO->unset();

			//---------draw-bloom-------------------------------
			GetEngine()->iRender->enable2d(true);
			GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);

			matViewportMap = viewportCopy_brightPass_blured;
			hdr->setPass("BlurPass");
			GetEngine()->iRender->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetEngine()->iRender->disableBlending();
			GetEngine()->iRender->enable3d();
		}
	}
}
