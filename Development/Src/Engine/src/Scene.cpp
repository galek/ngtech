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
//**************************************
#include "SceneUpdateJob.h"
//**************************************
#include "../OGLDrv/inc/GLExtensions.h"//TODO

namespace NGTech {

	/**
	*/
	Scene::Scene(CVARManager*_cvars)
		: cvars(_cvars),
		mUpdateJob(nullptr),
		camera(new CameraFixed()),
		frustum(new Frustum()){
		terrain = NULL;
	}

	/**
	*/
	void Scene::initialise(){

		Log::writeHeader("-- Scene --");
		Debug("Scene::initialise");
		mUpdateJob = new SceneUpdateJob();

		viewportFBO = GetRender()->CreateFBO(512, 512);
		viewportFBO->createDepthAttachment();

		int size = GetCvars()->r_shadowsize;
		shadowFBO = GetRender()->CreateFBO(size, size);
		shadowFBO->createDepthAttachment();

		viewportCopy = GetRender()->TextureCreate2D(512, 512, I_Texture::RGBA);
		viewportCopy->setWrap(I_Texture::CLAMP_TO_EDGE);
		viewportCopy->setFilter(I_Texture::LINEAR);

		viewportCopy_brightPass = GetRender()->TextureCreate2D(512, 512, I_Texture::RGBA);
		viewportCopy_brightPass->setWrap(I_Texture::CLAMP_TO_EDGE);
		viewportCopy_brightPass->setFilter(I_Texture::LINEAR);

		viewportCopy_brightPass_blured = GetRender()->TextureCreate2D(512, 512, I_Texture::RGBA);
		viewportCopy_brightPass_blured->setWrap(I_Texture::CLAMP_TO_EDGE);
		viewportCopy_brightPass_blured->setFilter(I_Texture::LINEAR);

		query = GetRender()->GetOQ();

		sphere = new ObjectMesh("sphere.nggf");

		matViewportMap = NULL;
		matShadowMap = NULL;
		matSpotMap = NULL;

		depthPass = new Material("engine_materials/depth_pass.mat");
		hdr = new Material("engine_materials/hdr.mat");
	}

	/**
	*/
	Scene::~Scene() {
		clear();
		SAFE_DELETE(mUpdateJob);
	}

	/**
	*/
	void Scene::clear() {
		systems.clear();
		objects.clear();
		lights.clear();

		delete terrain;
	}

	/**
	*/
	void Scene::reloadShaders() {
		GetCache()->reloadShaders();
	}

	/**
	*/
	void Scene::setGravity(const Vec3 &_gravity) {
		GetPhysics()->SetGravity(_gravity);
	}

	/**
	*/
	void Scene::setAmbient(const Vec3 &color) {
		ambient = color;
	}

	/**
	*/
	void Scene::setCamera(Camera *camera) {
		delete this->camera;
		this->camera = camera;
	}

	/**
	*/
	void Scene::setTerrain(const String &path, float step, float height, bool physics) {
		terrain = new Terrain(path, step, height, 16);
		terrainLods = Vec3(100, 200, 400);
		if (physics) terrain->setPhysics();
	}

	/**
	*/
	void Scene::setTerrainLods(const Vec3 &lods) {
		terrain->setLods(lods);
	}

	/**
	*/
	void Scene::setTerrainMaterial(const String &path) {
		if (terrain) terrain->setMaterial(path);
	}

	/**
	*/
	void Scene::addLight(Light *light) {
		lights.push_back(light);
	}

	/**
	*/
	void Scene::addObject(Object *object) {
		objects.push_back(object);
	}

	/**
	*/
	void Scene::addParticleSystem(ParticleSystem *system) {
		systems.push_back(system);
	}

	/**
	*/
	void Scene::deleteLight(Light *light) {
		for (int i = 0; i < lights.size(); i++) {
			if (light == lights[i]) {
				lights[i] = NULL;
			}
		}
	}

	/**
	*/
	void Scene::deleteObjectMesh(ObjectMesh *object) {
		for (int i = 0; i < objects.size(); i++) {
			if (object == objects[i]) {
				objects[i] = NULL;
			}
		}
	}

	/**
	*/
	void Scene::deleteParticleSystem(ParticleSystem *system) {
		for (int i = 0; i < systems.size(); i++) {
			if (system == systems[i]) {
				systems[i] = NULL;
			}
		}
	}

	/**
	*/
	void Scene::drawAmbient(bool blended)
	{
		matLightColor = ambient;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				frustum->get();

				matMVP = GetRender()->getMatrix_MVP();
				matLightColor = ambient;
				if (!mtr->setPass("Ambient")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)
				{
					if (!frustum->isInside(terrain->getMin(n), terrain->getMax(n)))
						continue;

					terrain->drawNode(n, camera->getPosition());
				}

				mtr->unsetPass();
			}
		}

		/**
		draw objects
		*/
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];

			GetRender()->push();
			GetRender()->multMatrix(object->getTransform());

			frustum->get();
			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}

			for (int s = 0; s < object->getNumSubsets(); s++) {
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				matMVP = GetRender()->getMatrix_MVP();
				matWorld = object->getTransform();

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
					mtr->setPassBlending("Ambient");
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				//mtr->setAlphaTest();

				if (!mtr->setPass("Ambient")) continue;

				object->drawSubset(s);

				if (mtr->passHasBlending("Ambient") && blended)
					mtr->unsetPassBlending();
				mtr->unsetPass();
			}

			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::drawPoint(LightPoint *light, bool blended) {
		if (!light->isVisible()) return;

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
				frustum->get();

				matMVP = GetRender()->getMatrix_MVP();
#pragma message("TODO:rename param 324,381 lines")
				if (!mtr->setPass("LightOmni")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
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
			Object *object = objects[m];

			GetRender()->push();
			GetRender()->multMatrix(object->getTransform());

			frustum->get();
			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				frustum->get();
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				matMVP = GetRender()->getMatrix_MVP();
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
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::getOmniShadowMap(LightPoint *light) {
		if (!light->getShadows() || !light->castShadows || !cvars->r_shadowtype) {
			return;
		}


		GetRender()->setMatrixMode_Projection();
		GetRender()->push();
		GetRender()->loadMatrix(Mat4::perspective(90, 1, 1, light->radius));

		GetRender()->setMatrixMode_Modelview();
		GetRender()->push();

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;

		for (int f = 0; f < 6; f++) {
			shadowFBO->set();
			shadowFBO->setColorTarget(light->shadowMap, f);
			shadowFBO->clear();

			GetRender()->loadMatrix(Mat4::cube(light->position, f));

			//DRAW OBJECTS
			for (int m = 0; m < objects.size(); m++)
			{
				if (!objects[m]) {
					continue;
				}
				if (objects[m]->getType() == Object::OBJECT) {
					continue;
				}
				Object *object = objects[m];

				GetRender()->push();
				GetRender()->multMatrix(object->getTransform());

				frustum->get();
				if (!frustum->isInside(object->getCenter(), object->getRadius())) {
					GetRender()->pop();
					continue;
				}
				if ((light->position - object->getTransform().getTranslation()).length() >
					light->radius + object->getRadius()) {
					GetRender()->pop();
					continue;
				}

				/**
				draw subsets
				*/
				for (int s = 0; s < object->getNumSubsets(); s++) {
					Material *mtr = object->getMaterial(s);
					if (!mtr) continue;

					frustum->get();
					if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
						continue;
					}

					//set material params
					matMVP = GetRender()->getMatrix_MVP();
					matWorld = object->getTransform();
					depthPass->setPass("DepthPass");

					object->drawSubset(s);

					depthPass->unsetPass();
				}
				GetRender()->pop();
			}
			//			light->getShadowMap()->copy(f);

			shadowFBO->flush();
			shadowFBO->unset();
		}

		GetRender()->setMatrixMode_Projection();
		GetRender()->pop();

		GetRender()->setMatrixMode_Modelview();
		GetRender()->pop();
	}

	/**
	*/
	void Scene::drawSpot(LightSpot *light, bool blended) {
		if (!light->isVisible()) return;

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
		if (terrain && !blended)
		{
			Material *mtr = terrain->getMaterial();
			if (mtr)
			{
				frustum->get();

				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				if (!mtr->setPass("LightSpot")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{

					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
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
		for (int m = 0; m < objects.size(); m++)
		{
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetRender()->push();
			GetRender()->multMatrix(object->getTransform());

			frustum->get();
			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				frustum->get();
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetRender()->getMatrix_MVP();
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
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::getSpotShadowMap(LightSpot *light) {
		if (!light->isVisible() || !light->castShadows || !cvars->r_shadowtype) {
			return;
		}

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;

		GetRender()->setMatrixMode_Projection();
		GetRender()->push();
		GetRender()->loadMatrix(Mat4::perspective(light->fov, 1, 1, light->radius));

		GetRender()->setMatrixMode_Modelview();
		GetRender()->push();
		GetRender()->loadMatrix(Mat4::lookAt(light->position, light->position + light->direction, Vec3(0, 1, 0)));

		shadowFBO->set();
		shadowFBO->setColorTarget(light->shadowMap);
		shadowFBO->clear();
		GetRender()->clearColor(Vec3(1.0, 1.0, 1.0));

		//DRAW OBJECTS
		for (int m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}
			if (objects[m]->getType() == Object::OBJECT) {
				continue;
			}
			Object *object = objects[m];

			GetRender()->push();
			GetRender()->multMatrix(object->getTransform());

			frustum->get();
			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->position - object->getTransform().getTranslation()).length() >
				light->radius + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				Material *mtr = object->getMaterial(s);

				if (!mtr) continue;

				frustum->get();
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				matMVP = GetRender()->getMatrix_MVP();
				matWorld = object->getTransform();
				depthPass->setPass("DepthPass");

				object->drawSubset(s);

				depthPass->unsetPass();
			}
			GetRender()->pop();
		}

		shadowFBO->flush();
		shadowFBO->unset();

		GetRender()->setMatrixMode_Projection();
		GetRender()->pop();

		GetRender()->setMatrixMode_Modelview();
		GetRender()->pop();
	}

	/**
	*/
	void Scene::drawDirect(LightDirect *light, bool blended) {
		//set material params
		matLightDirection = light->direction;
		matViewPosition = camera->getPosition();
		matLightColor = light->color;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->getMaterial();
			if (mtr) {
				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				if (!mtr->setPass("LightDirect")) return;

				frustum->get();

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
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

			GetRender()->push();
			GetRender()->multMatrix(object->getTransform());

			frustum->get();
			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (int s = 0; s < object->getNumSubsets(); s++) {
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->getMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetRender()->getMatrix_MVP();
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
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::checkOmniVisibility(LightPoint *light)
	{
		frustum->get();
		if (!frustum->isInside(light->position, light->radius)) {
			light->setVisible(false);
			return;
		}

		if ((light->position - camera->getPosition()).length() > light->radius) {
			GetRender()->colorMask(false, false, false, false);
			GetRender()->depthMask(false);
			//glColor3f(0, 0, 1);
			query->beginRendering();

			GetRender()->push();
			GetRender()->multMatrix(Mat4::translate(light->position) *
				Mat4::scale(Vec3(light->radius*0.2, light->radius*0.2, light->radius*0.2)));

			sphere->drawSubset(0);

			GetRender()->pop();

			query->endRendering();

			GetRender()->depthMask(true);
			GetRender()->colorMask(true, true, true, true);

			if (query->getResult() < 2) {
				light->setVisible(false);
				return;
			}
		}
		light->setVisible(true);
	}

	/**
	*/
	void Scene::checkSpotVisibility(LightSpot *light)
	{

		frustum->get();
		if (!frustum->isInside(light->position, light->radius)) {
			light->setVisible(false);
			return;
		}

		if ((light->position - camera->getPosition()).length() > light->radius) {
			GetRender()->colorMask(false, false, false, false);
			GetRender()->depthMask(false);

			query->beginRendering();

			GetRender()->push();
			GetRender()->multMatrix(Mat4::translate(light->position) *
				Mat4::scale(Vec3(light->radius*0.2, light->radius*0.2, light->radius*0.2)));

			sphere->drawSubset(0);

			GetRender()->pop();

			query->endRendering();

			GetRender()->depthMask(true);
			GetRender()->colorMask(true, true, true, true);

			if (query->getResult() < 2) {
				light->setVisible(false);
				return;
			}
		}
		light->setVisible(true);
	}

	/**
	*/
	void Scene::update()
	{
#pragma message("BUG,if camera is null,on next frame,after creation is will crash")
		//---------update-camera-----------------------------------
		camera->update();

		//---------draw-scene--------------------------------
		GetRender()->setMatrixMode_Projection();
		GetRender()->loadMatrix(camera->getProjection());

		GetRender()->setMatrixMode_Modelview();
		GetRender()->loadMatrix(camera->getTransform());

		matTime = GetWindow()->getETime();
		if (matTime > 0)
		{
			matViewportMap = viewportCopy;
			matViewportTransform = Mat4::texBias() * GetRender()->getMatrix_MVP();

			drawAmbient(false);

			for (int i = 0; i < lights.size(); i++)
			{
				if (lights[i]->isEnable())
				{
					if (lights[i]->getType() == Light::LIGHT_OMNI) {
						checkOmniVisibility((LightPoint*)lights[i]);
						if ((LightPoint*)lights[i]->isVisible())
							getOmniShadowMap((LightPoint*)lights[i]);
					}
					else if (lights[i]->getType() == Light::LIGHT_SPOT) {
						checkSpotVisibility((LightSpot*)lights[i]);
						if ((LightSpot*)lights[i]->isVisible())
							getSpotShadowMap((LightSpot*)lights[i]);
					}
				}
			}

			GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
			GetRender()->depthMask(false);

			//draw wireframe
			if (GetCvars()->r_wireframe) {
				glColor3f(1, 1, 1);//Nick:TODO:Replace
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Nick:TODO:Replace

				for (int i = 0; i < objects.size(); i++) {
					GetRender()->push();
					GetRender()->multMatrix(objects[i]->getTransform());
					for (int k = 0; k < objects[i]->getNumSubsets(); k++) {
						objects[i]->drawSubset(k);
					}
					GetRender()->pop();
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
				if (lights[i]->isVisible())
				{
					if (lights[i]->getType() == Light::LIGHT_OMNI) {
						drawPoint((LightPoint*)lights[i], false);
					}
					else if (lights[i]->getType() == Light::LIGHT_SPOT) {
						drawSpot((LightSpot*)lights[i], false);
					}
					else if (lights[i]->getType() == Light::LIGHT_DIRECT) {
						drawDirect((LightDirect*)lights[i], false);
					}
				}
			}

			GetRender()->depthMask(true);
			GetRender()->disableBlending();

			drawAmbient(true);

			//draw particle systems
			for (int k = 0; k < systems.size(); k++) {
				if (systems[k]) systems[k]->draw();
			}

			//---------draw-scene-into-viewport-copy-------------------------------
			viewportFBO->set();
			viewportFBO->setColorTarget(viewportCopy);
			viewportFBO->clear();

			//---------set-camera--------------------------------
			GetRender()->setMatrixMode_Projection();
			GetRender()->loadMatrix(camera->getProjection());

			GetRender()->setMatrixMode_Modelview();
			GetRender()->loadMatrix(camera->getTransform());

			drawAmbient(false);

			GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
			GetRender()->depthMask(false);
		}
	}

	/**
	*/
	void Scene::updateSound()
	{
		GetAudio()->setListener(camera->getPosition(), camera->getDirection());
	}

	/**
	*/
	void Scene::render()
	{
		drawAmbient(true);

		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->isVisible())
			{
				if (lights[i]->getType() == Light::LIGHT_OMNI) {
					drawPoint((LightPoint*)lights[i], true);
				}
				else if (lights[i]->getType() == Light::LIGHT_SPOT) {
					drawSpot((LightSpot*)lights[i], true);
				}
				else if (lights[i]->getType() == Light::LIGHT_DIRECT) {
					drawDirect((LightDirect*)lights[i], true);
				}
			}
		}

		GetRender()->depthMask(true);
		GetRender()->disableBlending();

		viewportFBO->flush();
		viewportFBO->unset();

		matMVP = GetRender()->getMatrix_MVP();
#pragma message("Тормозит,все из-за HDR")
		if (GetCvars()->r_hdr) {
			//---------bright-pass--------------------------------
			viewportFBO->set();
			viewportFBO->clear();

			GetRender()->enable2d(true);

			hdr->setPass("BrightPass");
			matViewportMap = viewportCopy;
			GetRender()->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetRender()->enable3d();

			viewportCopy_brightPass->copy();
			viewportFBO->clear();

			GetRender()->enable2d(true);

			hdr->setPass("BlurPass");
			matViewportMap = viewportCopy_brightPass;
			GetRender()->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetRender()->enable3d();

			viewportCopy_brightPass_blured->copy();
			viewportFBO->unset();

			//---------draw-bloom-------------------------------
			GetRender()->enable2d(true);
			GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);

			hdr->setPass("BlurPass");
			matViewportMap = viewportCopy_brightPass_blured;
			GetRender()->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
			hdr->unsetPass();

			GetRender()->disableBlending();
			GetRender()->enable3d();
		}
	}

	/**
	*/
	void Scene::runUpdate() {
		if (mUpdateJob) {
			update_id = GetEngine()->threads->runJobs(mUpdateJob, sizeof(SceneUpdateJob), 1);
		}
	}

	/**
	*/
	void Scene::waitUpdate(){
		if (mUpdateJob) {
			GetEngine()->threads->waitJobs(update_id);
		}
	}

	/**
	*/
	bool Scene::_cameraCheckValid()
	{
		if (!camera)
		{
			Warning("Current camera is not valid.Loosing ptr?.Activated Fixed camera");
			camera = new CameraFixed();
			return false;
		}
		return true;
	}
}