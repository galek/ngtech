#include "EnginePrivate.h"
#include "Engine.h"
//**************************************
#include "Effect.h"
#include "Frustum.h"
#include "Config.h"
#include "Scene.h"
#include "WindowSystem.h"
#include <math.h>
//**************************************
#include "../OGLDrv/inc/GLExtensions.h"//TODO

namespace NGTech {

	EffectParticleSystem::EffectParticleSystem(const String &path, int numParticles) {
		texture = GetRender()->TextureCreate2D(path);

		particleList = GetRender()->GetDL();
		particleList->beginBuild();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();
		particleList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);

		lifeTime = 5000;
		this->numParticles = numParticles;
		particles = new Particle[numParticles];

		for (int i = 0; i < numParticles; i++) {
			particles[i].lifeTime = 0;
			particles[i].velocity = velocity = Vec3(0, 10, 0);
			particles[i].position = Vec3(0, 0, 0);
		};
	};

	EffectParticleSystem::~EffectParticleSystem() {
	}

	void EffectParticleSystem::draw() {
		texture->set(0);
		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		for (int i = 0; i < numParticles; i++) {

			if (lifeTime < particles[i].lifeTime || particles[i].lifeTime == 0) {
				particles[i].position = position;
				particles[i].velocity = Vec3(velocity.x + sinf(rand()) * velocity.length(),
					velocity.y + cosf(rand()) * velocity.length(),
					velocity.z + sinf(rand()) * velocity.length())
					+ force * GetWindow()->getDTime() * 0.001;
				particles[i].lifeTime = i / numParticles *lifeTime;
			};

			particles[i].position += particles[i].velocity * GetWindow()->getDTime() * 0.001;

			GetRender()->push();
			GetRender()->translate(particles[i].position);
			GetRender()->scale(Vec3(5, 5, 5));

			GetRender()->rotate(GetScene()->GetActiveCamera()->getAngle(0) - 180, Vec3(0, 1, 0));
			GetRender()->rotate(GetScene()->GetActiveCamera()->getAngle(1), Vec3(1, 0, 0));

			particleList->call();

			GetRender()->pop();

			particles[i].lifeTime += GetWindow()->getDTime();
		};
		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);
		GetRender()->setColor(Vec4(1, 1, 1, 1));
	}

	EffectFlare::EffectFlare(const String &path) {
		texture = GetRender()->TextureCreate2D(path);

		flareList = GetRender()->GetDL();
		flareList->beginBuild();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();
		flareList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);
		radius = 5.0;
	};

	EffectFlare::~EffectFlare() {
	}

	void EffectFlare::draw() {
		GetRender()->push();
		GetRender()->translate(position);
		GetRender()->scale(Vec3(radius, radius, radius));

		GetRender()->rotate(GetScene()->GetActiveCamera()->getAngle(0) - 180, Vec3(0, 1, 0));
		GetRender()->rotate(GetScene()->GetActiveCamera()->getAngle(1), Vec3(1, 0, 0));

		texture->set(0);
		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		flareList->call();

		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);

		GetRender()->pop();
	}
}