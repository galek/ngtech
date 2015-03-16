/*
NOTES:
*neeeded refactoring
*/
//**************************************
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
#include "AudioUpdateJob.h"
//**************************************
#include <iostream>
#include <algorithm>
#include <vector>
//**************************************

namespace NGTech {

	/**
	*/
	Scene::Scene(CVARManager*_cvars)
		: cvars(_cvars),
		mUpdateJob(nullptr),
		terrain(nullptr),
		depthPass(nullptr),
		hdr(nullptr),
		query(nullptr),
		sphere(nullptr),
		viewportFBO(nullptr),
		shadowFBO(nullptr),
		viewportCopy(nullptr),
		hdrViewportCopy(nullptr),
		viewportCopy_brightPass_blured(nullptr),
		matViewportMap(nullptr),
		matShadowMap(nullptr),
		matSpotMap(nullptr),
		currentLight(nullptr),
		needStats(false),
		paused(false),
		currentCamera(new CameraFixed()),
		frustum(new Frustum())
	{
	}

	/**
	*/
	void Scene::initialise()
	{
		Log::writeHeader("-- Scene --");
		Debug("Scene::initialise");
		mUpdateJob = new AudioUpdateJob();

		viewportFBO = GetRender()->CreateFBO(cvars->r_width, cvars->r_height);
		viewportFBO->createDepthAttachment();

		int size = GetCvars()->r_shadowsize;
		shadowFBO = GetRender()->CreateFBO(size, size);
		shadowFBO->createDepthAttachment();

		viewportCopy = GetRender()->TextureCreate2D(cvars->r_width, cvars->r_height, I_Texture::RGBA);
		viewportCopy->setWrap(I_Texture::CLAMP_TO_EDGE);
		viewportCopy->setFilter(I_Texture::LINEAR);

		hdrViewportCopy = GetRender()->TextureCreate2D(128, 128, I_Texture::RGBA);
		hdrViewportCopy->setWrap(I_Texture::CLAMP_TO_EDGE);
		hdrViewportCopy->setFilter(I_Texture::LINEAR);
		hdrViewportCopy->setAniso(I_Texture::ANISO_X0);

		viewportCopy_brightPass_blured = GetRender()->TextureCreate2D(512, 512, I_Texture::RGBA);
		viewportCopy_brightPass_blured->setWrap(I_Texture::CLAMP_TO_EDGE);
		viewportCopy_brightPass_blured->setFilter(I_Texture::LINEAR);

		query = GetRender()->GetOQ();

		sphere = new ObjectMesh("sphere.nggf");

		depthPass = new Material("materials/engine_materials/depth_pass.mat");
		hdr = new Material("materials/engine_materials/HDR.mat");
	}

	/**
	*/
	Scene::~Scene() {
		clear();
		SAFE_DELETE(hdr);
		SAFE_DELETE(sphere);
		SAFE_DELETE(shadowFBO);
		SAFE_DELETE(viewportFBO);
		SAFE_DELETE(currentCamera);
		SAFE_DELETE(query);
		SAFE_DELETE(viewportCopy);
		SAFE_DELETE(hdrViewportCopy);
		SAFE_DELETE(mUpdateJob);
	}

	/**
	*/
	void Scene::clear() {
		systems.clear();
		objects.clear();
		visibleLights.clear();
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
		auto older = this->currentCamera;
		this->currentCamera = camera;
		SAFE_DELETE(older);
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
		auto it = std::find(lights.begin(), lights.end(), light);
		if (it != lights.end())
		{
			lights.erase(it);
		}
	}

	/**
	*/
	void Scene::deleteObjectMesh(ObjectMesh *object) {
		auto it = std::find(objects.begin(), objects.end(), object);
		if (it != objects.end())
		{
			objects.erase(it);
		}
	}

	/**
	*/
	void Scene::deleteParticleSystem(ParticleSystem *system) {
		auto it = std::find(systems.begin(), systems.end(), system);
		if (it != systems.end())
		{
			systems.erase(it);
		}
	}

	/**
	*/
	void Scene::drawAmbient(bool blended) {
		matLightColor = ambient;//was deleted:если удалить то будут некорректно затенение
		/*
		draw terrain
		*/
		if (terrain && !blended) {
			Material *mtr = terrain->GetMaterial();

			if (mtr && mtr->setPass("Ambient")) {
				matMVP = GetRender()->getMatrix_MVP();
				matLightColor = ambient;//Тоже заменялось

				for (int n = 0; n < terrain->getNumNodes(); n++) {
					if (!frustum->isInside(terrain->getMin(n), terrain->getMax(n)))
						continue;

					terrain->drawNode(n, currentCamera->getPosition());
				}

				mtr->unsetPass();
			}
		}

		/**
		draw objects
		*/
		for (size_t m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			for (size_t s = 0; s < object->GetNumSubsets(); s++)
			{
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->GetMaterial(s);
				if (!mtr) continue;

				matMVP = GetRender()->getMatrix_MVP();//was deleted
				matWorld = objTrans;

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
					mtr->setPassBlending("Ambient");
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				mtr->setPassAlphaTest();

				if (!mtr->setPass("Ambient")) continue;

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();
				if (mtr->passHasBlending("Ambient") && blended){
					mtr->unsetPassBlending();
				}
				mtr->unsetPass();
			}
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::drawPoint(LightPoint *light, bool blended)
	{
		currentLight = light;
		if (!light->isVisible() || !light->isEnable()) return;

		if (!currentCamera->GetFrustum()->isInside(BSphere(light->getPosition(), light->getRadius())))
			return;

		matLightIRadius = light->getIRadius();//was deleted
		matLightPosition = light->position;//was deleted
		matViewPosition = currentCamera->getPosition();//was deleted
		matLightColor = light->color;//was deleted
		matShadowMap = light->shadowMap;//was deleted

		/*
		Draw terrain
		*/
		if (terrain && !blended) {
			Material *mtr = terrain->GetMaterial();
			if (mtr && mtr->setPass("LightOmni")) {
				matMVP = GetRender()->getMatrix_MVP();

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}
					if ((light->getPosition() - terrain->getCenter(n)).length() >
						light->getRadius() + terrain->getRadius(n)) {
						continue;
					}

					terrain->drawNode(n, currentCamera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		/*
		draw objects
		*/
		for (size_t m = 0; m < objects.size(); m++)
		{
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->position - objTrans.getTranslation()).length() >
				light->radius + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			/*
			draw subsets
			*/
			for (size_t s = 0; s < object->GetNumSubsets(); s++) {
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s)))
					continue;

				Material *mtr = object->GetMaterial(s);
				if (!mtr) continue;
				matMVP = GetRender()->getMatrix_MVP();//was deleted
				matWorld = objTrans;//was deleted
				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				if (!mtr->setPass("LightOmni")) continue;

				mtr->setPassAlphaTest();

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();

				mtr->unsetPass();
			}
			GetRender()->pop();
		}
	}

	/**
	*/
	bool Scene::_LightInVisibleList(Light* _light)
	{
		return ((std::find(visibleLights.begin(), visibleLights.end(), _light) != visibleLights.end()));
	}

	/**
	*/
	bool Scene::_CheckLightOnShadows(Light* _light)
	{
		return _LightInVisibleList(_light) || !_light->isCastShadows() || !_light->isVisible() || cvars->r_shadowtype == 0;
	}

	/**
	*/
	void Scene::_PointShadowMap(LightPoint *light) {
		if (!_CheckLightOnShadows(light))
			return;

		//set matrices
		GetRender()->setMatrixMode_Projection();
		GetRender()->push();
		GetRender()->loadMatrix(Mat4::perspective(90, 1, 1, light->radius));

		GetRender()->setMatrixMode_Modelview();
		GetRender()->push();

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;


		for (size_t f = 0; f < 6; f++)
		{
			shadowFBO->set();

			if (needStats)
				GetDebug()->SetRenderChangesOfFrameBufferrPerFrame(GetDebug()->GetRenderChangesOfFrameBufferrPerFrame() + 1);

			shadowFBO->setColorTarget(light->shadowMap, f);

			shadowFBO->clear();

			GetRender()->loadMatrix(Mat4::cube(light->getPosition(), f));

			_RenderScene(light);

			shadowFBO->unset();
		}

		GetRender()->setMatrixMode_Projection();
		GetRender()->pop();

		GetRender()->setMatrixMode_Modelview();
		GetRender()->pop();
	}

	/**
	*/
	void Scene::drawSpot(LightSpot *light, bool blended)
	{
		currentLight = light;
		if (!light->isVisible() || !light->isEnable()) {
			return;
		}

		light->projTransform = Mat4::texBias() *
			Mat4::perspective(light->getFOV(), 1, 1, light->getRadius()) *
			Mat4::lookAt(light->getPosition(), light->getPosition() + light->getDirection(), Vec3(0, 1, 0));

		//set material params
		matLightIRadius = light->getIRadius();//was deleted
		matLightPosition = light->position;//was deleted
		matLightDirection = light->direction;//was deleted
		matViewPosition = currentCamera->getPosition();//was deleted
		matLightColor = light->color;//was deleted
		matShadowMap = light->shadowMap;//was deleted
		matSpotMap = light->projMap;//was deleted
		matSpotTransform = light->projTransform;//was deleted

		/*
		draw terrain
		*/
		if (terrain && !blended) {
			Material *mtr = terrain->GetMaterial();

			if (mtr && mtr->setPass("LightSpot")) {
				//set material params
				matMVP = GetRender()->getMatrix_MVP();

				for (int n = 0; n < terrain->getNumNodes(); n++)
				{

					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}
					if ((light->getPosition() - terrain->getCenter(n)).length() >
						light->getRadius() + terrain->getRadius(n)) {
						continue;
					}

					terrain->drawNode(n, currentCamera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		/*
		draw objects
		*/
		for (size_t m = 0; m < objects.size(); m++)
		{
			if (!objects[m])
			{
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->getPosition() - objTrans.getTranslation()).length() >
				light->getRadius() + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			/*
			draw subsets
			*/
			for (size_t s = 0; s < object->GetNumSubsets(); s++)
			{
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->GetMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				matWorld = objTrans;
				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
					mtr->setPassBlending("Ambient");
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}
				if (!mtr->setPass("LightSpot")) continue;
				mtr->setPassAlphaTest();

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();

				mtr->unsetPass();
			}
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::getSpotShadowMap(LightSpot *light) {
		if (!light->isCastShadows() || cvars->r_shadowtype == 0) {
			return;
		}

		matLightIRadius = light->getIRadius();
		matLightPosition = light->position;

		GetRender()->setMatrixMode_Projection();
		GetRender()->push();//was deleted
		GetRender()->loadMatrix(Mat4::perspective(light->getFOV(), 1, 1, light->getRadius()));

		GetRender()->setMatrixMode_Modelview();
		GetRender()->push();//was deleted
		GetRender()->loadMatrix(Mat4::lookAt(light->getPosition(), light->getPosition() + light->direction, Vec3(0, 1, 0)));

		shadowFBO->set();
		if (needStats)
			GetDebug()->SetRenderChangesOfFrameBufferrPerFrame(GetDebug()->GetRenderChangesOfFrameBufferrPerFrame() + 1);

		shadowFBO->setColorTarget(light->shadowMap);
		shadowFBO->clear();

		//DRAW OBJECTS
		for (size_t m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}
			if ((light->getPosition() - objTrans.getTranslation()).length() >
				light->getRadius() + object->getRadius()) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (size_t s = 0; s < object->GetNumSubsets(); s++) {
				Material *mtr = object->GetMaterial(s);

				if (!mtr) continue;

				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				matMVP = GetRender()->getMatrix_MVP();
				matWorld = objTrans;
				depthPass->setPass("DepthPass");

				mtr->setPassAlphaTest();

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();

				depthPass->unsetPass();
			}
			GetRender()->pop();
		}

		shadowFBO->unset();

		GetRender()->setMatrixMode_Projection();
		GetRender()->pop();

		GetRender()->setMatrixMode_Modelview();
		GetRender()->pop();
	}

	/**
	*/
	void Scene::drawDirect(LightDirect *light, bool blended)
	{
		currentLight = light;
		if (!light->isVisible() || !light->isEnable()) return;

		//set material params
		matLightDirection = light->direction;
		matViewPosition = currentCamera->getPosition();
		matLightColor = light->color;

		//DRAW TERRAIN
		if (terrain && !blended) {
			Material *mtr = terrain->GetMaterial();
			if (mtr) {
				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				if (!mtr->setPass("LightDirect")) return;

				for (int n = 0; n < terrain->getNumNodes(); n++)	{
					if (!frustum->isInside(terrain->getCenter(n), terrain->getRadius(n))) {
						continue;
					}

					terrain->drawNode(n, currentCamera->getPosition());
				}
				mtr->unsetPass();
			}
		}

		//DRAW OBJECTS
		for (size_t m = 0; m < objects.size(); m++) {
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			if (!frustum->isInside(object->getCenter(), object->getRadius())) {
				GetRender()->pop();
				continue;
			}

			//DRAW OBJECT SUBSETS
			for (size_t s = 0; s < object->GetNumSubsets(); s++) {
				if (!frustum->isInside(object->getCenter(s), object->getRadius(s))) {
					continue;
				}

				Material *mtr = object->GetMaterial(s);
				if (!mtr) continue;

				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				matWorld = objTrans;

				if (blended) {
					if (!mtr->passHasBlending("Ambient")) continue;
				}
				else {
					if (mtr->passHasBlending("Ambient")) continue;
				}

				if (!mtr->setPass("LightDirect")) continue;

				mtr->setPassAlphaTest();

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();
				mtr->unsetPass();
			}
			GetRender()->pop();
		}
	}

	/**
	*/
	void Scene::checkPointVisibility(LightPoint *light)
	{
		//frustum visibility
		if (!currentCamera->GetFrustum()->isInside(BSphere(light->getPosition(), light->getRadius())))
			return;

		//occlusion culling
		if ((light->getPosition() - currentCamera->getPosition()).length() > light->getRadius())
		{
			GetRender()->colorMask(false, false, false, false);
			GetRender()->depthMask(false);

			query->beginRendering();

			GetRender()->push();
			GetRender()->multMatrix(Mat4::translate(light->getPosition()) *
				Mat4::scale(Vec3(light->getRadius(), light->getRadius(), light->getRadius())));

			sphere->DrawSubset(0, this);

			GetRender()->pop();
			query->endRendering();

			GetRender()->depthMask(true);
			GetRender()->colorMask(true, true, true, true);

			if (query->getResult() < 2)
				return;
		}

		visibleLights.push_back(light);
	}

	/**
	*/
	void Scene::checkSpotVisibility(LightSpot *light)
	{
		//is incorrect
		/*if (!frustum->isInside(light->position, light->radius)) {
			light->setVisible(false);
			return;
			}*/

		//if ((light->getPosition() - currentCamera->getPosition()).length() > light->radius) {
		//	GetRender()->colorMask(false, false, false, false);
		//	GetRender()->depthMask(false);

		//	query->beginRendering();

		//	GetRender()->push();
		//	GetRender()->multMatrix(Mat4::translate(light->getPosition()) *
		//		Mat4::scale(Vec3(light->radius*0.2, light->radius*0.2, light->radius*0.2)));

		//	sphere->DrawSubset(0);

		//	GetRender()->pop();

		//	query->endRendering();

		//	GetRender()->depthMask(true);
		//	GetRender()->colorMask(true, true, true, true);

		//	if (query->getResult() < 2) {
		//		light->setVisible(false);
		//		return;
		//	}
		//}
	}

	/**
	*/
	void Scene::update(bool _paused)
	{
		if (paused != _paused)
			paused = _paused;

		if (!currentCamera)
			return;
		//---------update-camera-----------------------------------
		currentCamera->update();

		_DrawParticles();

		//---------draw-scene--------------------------------
		GetRender()->setMatrixMode_Projection();
		currentCamera->RecalculateProjection();
		GetRender()->loadMatrix(currentCamera->GetProjection());

		GetRender()->setMatrixMode_Modelview();
		GetRender()->loadMatrix(currentCamera->getTransform());

		matTime = GetWindow()->getETime();
		if (matTime <= 0)
			return;
		matViewportMap = viewportCopy;
		matViewportTransform = Mat4::texBias() * GetRender()->getMatrix_MVP();

		//draw wireframe
		if (GetCvars()->r_wireframe)
			GetRender()->EnableWireframeMode();

		drawAmbient(false);

		_RenderShadows();
		//lighting
		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
		GetRender()->depthMask(false);


		//draw lighting
		_RenderVisibleLights(false);


		GetRender()->depthMask(true);
		GetRender()->disableBlending();

		drawAmbient(true);

		_DrawParticles();

		//---------draw-scene-into-viewport-copy-------------------------------
		viewportFBO->set();
		viewportFBO->setColorTarget(viewportCopy);
		viewportFBO->clear();

		//---------set-camera--------------------------------
		GetRender()->setMatrixMode_Projection();
		currentCamera->RecalculateProjection();
		GetRender()->loadMatrix(currentCamera->GetProjection());

		GetRender()->setMatrixMode_Modelview();
		GetRender()->loadMatrix(currentCamera->getTransform());

		drawAmbient(false);

		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
		GetRender()->depthMask(false);

		_RenderAnimation();
	}

	/**
	*/
	void Scene::_RenderAnimation()
	{
		float mFPS = GetEngine()->GetLastFPS();

		if (mFPS <= 0.f) mFPS = 1.f;
		if (mFPS >= 60.f) mFPS = 60.f;

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getType() == Object::OBJECT_SKELETEAL_MESH)
			{
				ObjectSkinnedMesh*mesh = (ObjectSkinnedMesh*)objects[i];
				mesh->setAnimationFrame(matTime / GetWindow()->divider()*mFPS);
			}
		}
	}

	/**
	*/
	void Scene::updateSound()
	{
		GetAudio()->setListener(currentCamera->getPosition(), currentCamera->getDirection());
	}

	/**
	*/
	void Scene::render()
	{
		drawAmbient(true);

		_RenderVisibleLights(true);

		GetRender()->depthMask(true);
		GetRender()->disableBlending();

		_DrawParticles();

		viewportFBO->unset();

		matMVP = GetRender()->getMatrix_MVP();
		if (GetCvars()->r_hdr)
		{
			viewportFBO->set();
			viewportFBO->clear();

			GetRender()->enable2d(true);
			{
				viewportCopy->Set();
				GetRender()->drawRect(0, 0, 1, 1, 0, cvars->r_width, cvars->r_height, 0);
				viewportCopy->UnSet();

				//hdrViewportCopy->copy();//Тормозит
				viewportFBO->clear();

				//BrightPass
				hdr->setPass("BrightPass");
				matViewportMap = hdrViewportCopy;
				GetRender()->drawRect(0, 0, 1, 1, 0, 1/*cvars->r_width*/, 1/*cvars->r_height*/, 0);
				hdr->unsetPass();

				//hdrViewportCopy->copy();//Тормозит
				viewportFBO->clear();

				//BlurV
				hdr->setPass("BlurPass");
				matViewportMap = hdrViewportCopy;
				GetRender()->drawRect(0, 0, 1, 1, 0, /*cvars->r_width*/1, /*cvars->r_height*/1, 0);
				hdr->unsetPass();

				//hdrViewportCopy->copy();//Тормозит
			}
			GetRender()->enable3d();

			viewportFBO->unset();

			//BlurH
			GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
			GetRender()->enable2d(true);
			{
				hdr->setPass("BlurPass");
				matViewportMap = hdrViewportCopy;
				GetRender()->drawRect(0, 0, 1, 1, 0, /*cvars->r_width*/1, /*cvars->r_height*/1, 0);
				hdr->unsetPass();
			}
			GetRender()->disableBlending();
			GetRender()->enable3d();
		}
		//draw wireframe
		if (GetCvars()->r_wireframe)
			GetRender()->DisableWireframeMode();
		needStats = false;

		visibleLights.clear();
	}

	/**
	*/
	void Scene::runUpdate() {
		if (mUpdateJob) {
			update_id = GetEngine()->threads->runJobs(mUpdateJob, sizeof(AudioUpdateJob), 1);
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
		if (!currentCamera)
		{
			Warning("Current camera is not valid.Loosing ptr?.Activated Fixed camera");
			currentCamera = new CameraFixed();
			return false;
		}
		return true;
	}

	void Scene::testInteserction()
	{
#if 0
		Vec3 ray_origin;
		Vec3 ray_direction;
		ScreenPosToWorldRay(
			1024 / 2, 768 / 2,
			1024, 768,
			ViewMatrix,
			ProjectionMatrix,
			ray_origin,
			ray_direction
			);

		//ray_direction = ray_direction*20.0f;

		//Nick message = "background";

		// Test each each Oriented Bounding Box (OBB).
		// A physics engine can be much smarter than this, 
		// because it already has some spatial partitionning structure, 
		// like Binary Space Partitionning Tree (BSP-Tree),
		// Bounding Volume Hierarchy (BVH) or other.
		for (int i = 0; i < 100; i++){

			float intersection_distance; // Output of TestRayOBBIntersection()
			Vec3 aabb_min(-1.0f, -1.0f, -1.0f);
			Vec3 aabb_max(1.0f, 1.0f, 1.0f);

			// The ModelMatrix transforms :
			// - the mesh to its desired position and orientation
			// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
			Mat4 RotationMatrix = glm::toMat4(orientations[i]);
			Mat4 TranslationMatrix = Mat4::translate(Mat4(), positions[i]);
			Mat4 ModelMatrix = TranslationMatrix * RotationMatrix;


			if (TestRayOBBIntersection(
				ray_origin,
				ray_direction,
				aabb_min,
				aabb_max,
				ModelMatrix,
				intersection_distance)
				){
				/*std::ostringstream oss;
				oss << "mesh " << i;
				message = oss.str();*/
				break;
			}
		}
#endif
	}

	/**
	*/
	void Scene::_RenderLight(Light* _light, bool _blend)
	{
		switch (_light->getType())
		{
		case Light::LIGHT_OMNI:
			drawPoint((LightPoint*)_light, _blend);
			break;

		case Light::LIGHT_SPOT:
			drawSpot((LightSpot*)_light, _blend);
			break;

		case Light::LIGHT_DIRECT:
			drawDirect((LightDirect*)_light, _blend);
			break;
		}
	}

	/**
	*/
	void Scene::_RenderVisibleLights(bool _v)
	{
		for (size_t i = 0; i < visibleLights.size(); i++)
		{
			_RenderLight(lights[i], _v);
		}
	}

	/**
	*/
	void Scene::_RenderShadows()
	{
		Light* tempLight = nullptr;

		for (size_t i = 0; i < lights.size(); i++)
		{
			tempLight = lights[i];

			switch (tempLight->getType())
			{
			case Light::LIGHT_OMNI:
				checkPointVisibility((LightPoint*)tempLight);
				_PointShadowMap((LightPoint*)tempLight);
				break;

			case Light::LIGHT_SPOT:
				checkSpotVisibility((LightSpot*)tempLight);
				getSpotShadowMap((LightSpot*)tempLight);
				break;

			case Light::LIGHT_DIRECT:
				break;
			}
		}
	}

	/**
	*/
	void Scene::_DrawParticles()
	{
		if (!paused)
		{
			//draw particle systems
			for (size_t k = 0; k < systems.size(); k++) {
				if (systems[k]) systems[k]->draw();
			}
		}
	}

	/**
	*/
	void Scene::_RenderScene(Light* light)
	{
		/*
		draw objects
		*/
		for (size_t m = 0; m < objects.size(); m++)
		{
			if (!objects[m]) {
				continue;
			}

			Object *object = objects[m];
			Mat4 objTrans = object->GetTransform();

			GetRender()->push();
			GetRender()->multMatrix(objTrans);

			if ((!frustum->isInside(object->getCenter(), object->getRadius()))
				|| (((light->getPosition() - objTrans.getTranslation()).length() >
				light->getRadius() + object->getRadius())))
			{
				GetRender()->pop();
				continue;
			}

			/**
			draw subsets
			*/
			for (size_t s = 0; s < object->GetNumSubsets(); s++) {
				Material *mtr = object->GetMaterial(s);
				if (!mtr) continue;

				if (!frustum->isInside(object->getCenter(s), object->getRadius(s)))
					continue;


				//set material params
				matMVP = GetRender()->getMatrix_MVP();
				matWorld = objTrans;
				depthPass->setPass("DepthPass");

				mtr->setPassAlphaTest();

				object->DrawSubset(s, this);

				mtr->unsetPassAlphaTest();

				depthPass->unsetPass();
			}
			GetRender()->pop();
		}
	}
}