#include "EnginePrivate.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
//***************************************************************************
#include "PxPhysicsAPI.h"
#include "EngineMathToPhysx.inl"
//***************************************************************************

namespace NGTech {
	using namespace physx;

	PhysBody *PhysBody::createBox(const Vec3 &size, float _mass){ 

		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxMaterial* cubeMaterial = GetEngine()->physSystem->mPhysics->createMaterial(0.5f, 0.5f, 0.1f);

		PxReal cubeDensity = 2.0f;//плотность
		PxBoxGeometry cubeGeometry(size.x, size.y, size.z);
		body->mActor = PxCreateDynamic(*GetEngine()->physSystem->mPhysics, PxTransform(PxVec3(0, 0, 0)), cubeGeometry, *cubeMaterial, cubeDensity);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}
		
		if (_mass > 0)
			body->mActor->setMass(body->mass);
		body->mActor->setLinearDamping(1.0f);
		body->mActor->setAngularDamping(1.0f);
		GetEngine()->physSystem->mScene->addActor(*body->mActor);

		if (body->mActor)
			body->mActor->setGlobalPose(physx::PxTransform(physx::PxVec3(0,100,0)));
		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::createSphere(float radius, float _mass) {
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxMaterial* cubeMaterial = GetEngine()->physSystem->mPhysics->createMaterial(0.5f, 0.5f, 0.1f);

		PxReal cubeDensity = 2.0f;//плотность
		PxSphereGeometry sphereGeometry(radius);
		body->mActor = PxCreateDynamic(*GetEngine()->physSystem->mPhysics, PxTransform(PxVec3(0, 0, 0)), sphereGeometry, *cubeMaterial, cubeDensity);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (_mass > 0)
			body->mActor->setMass(body->mass);
		body->mActor->setLinearDamping(1.0f);
		body->mActor->setAngularDamping(1.0f);
		GetEngine()->physSystem->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::createCylinder(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateCylinder(GetEngine()->physSystem->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);

		//NewtonBodySetUserData(body->nBody, body);
		//NewtonBodySetAutoSleep(body->nBody, 0);

		//if (mass > 0) {
		//	NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
		//	NewtonBodySetCentreOfMass(body->nBody, origin);
		//	NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		//}

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody *PhysBody::createCone(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateCone(GetEngine()->physSystem->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);

		//NewtonBodySetUserData(body->nBody, body);
		//NewtonBodySetAutoSleep(body->nBody, 0);

		//if (mass > 0) {
		//	NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
		//	NewtonBodySetCentreOfMass(body->nBody, origin);
		//	NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		//}

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody *PhysBody::createCapsule(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateCapsule(GetEngine()->physSystem->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);

		//NewtonBodySetUserData(body->nBody, body);
		//NewtonBodySetAutoSleep(body->nBody, 0);

		//if (mass > 0) {
		//	NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
		//	NewtonBodySetCentreOfMass(body->nBody, origin);
		//	NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		//}

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody *PhysBody::createChampferCylinder(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateChamferCylinder(GetEngine()->physSystem->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);

		//NewtonBodySetUserData(body->nBody, body);
		//NewtonBodySetAutoSleep(body->nBody, 0);

		//if (mass > 0) {
		//	NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
		//	NewtonBodySetCentreOfMass(body->nBody, origin);
		//	NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		//}

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody *PhysBody::createConvexHull(Vec3 *pos, const int numPos, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateConvexHull(GetEngine()->physSystem->nWorld, numPos, &pos[0].x, 3 * sizeof(float) , 0, NULL, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);

		//NewtonBodySetUserData(body->nBody, body);
		//NewtonBodySetAutoSleep(body->nBody, 0);

		//if (mass > 0) {
		//	NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
		//	NewtonBodySetCentreOfMass(body->nBody, origin);
		//	NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		//}

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody *PhysBody::createStaticMesh(Vec3 *pos, const int numPos, bool optimize) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		//NewtonCollision const*collision = NewtonCreateTreeCollision(GetEngine()->physSystem->nWorld, 0);
		//NewtonTreeCollisionBeginBuild(collision);
		//for (int i = 0; i < numPos / 3; i++) {
		//	Vec3 v[3];
		//	v[0] = pos[i * 3 + 0];
		//	v[1] = pos[i * 3 + 1];
		//	v[2] = pos[i * 3 + 2];

		//	NewtonTreeCollisionAddFace(collision, 3, &v[0].x, 3 * sizeof(float) , i+1);
		//}
		//NewtonTreeCollisionEndBuild(collision, 1);
		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, Mat4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1));//Nick:Сомневаюсь,верно ли?
		//NewtonDestroyCollision(collision);
		//NewtonBodySetMassMatrix(body->nBody, 1.0f, 1.0f, 1.0f, 1.0f);
		//NewtonBodySetUserData(body->nBody, body);

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	PhysBody::~PhysBody() {/*
		NewtonDestroyBody(GetEngine()->physSystem->nWorld, nBody);*/
		SAFE_DELETE(impactSrc);
	}

	void PhysBody::setTransform(const Mat4 &transform) {
		if (mActor)
			mActor->setGlobalPose(EngineMathToPhysX(transform));
	}

	Mat4 PhysBody::getTransform() {
		auto pos = mActor->getGlobalPose();
		return PhysXToEngineMath(pos);
	}

	void PhysBody::addTorque(const Vec3 &torque) {
		this->torque += torque;
		this->force += torque * getMass();
	}

	Vec3 PhysBody::getTorque() {
		return torque;
	}

	void PhysBody::addForce(const Vec3 &force) {
	/*	this->torque += force / getMass();
		this->force += force;*/
	}

	Vec3 PhysBody::getForce() {
		return force;
	}

	void PhysBody::addVelocity(const Vec3 &velocity) {
	}

	void PhysBody::setLinearVelocity(const Vec3 &velocity) {
		if (mActor)
			mActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::getVelocity() {
		Vec3 velocity(0, 0, 0);
		return velocity;
	}

	float PhysBody::getMass() {
		if (mActor)
			return mActor->getMass();
		else return 0.0f;
	}

	void PhysBody::setMass(float mass) {
		if (mActor)
			mActor->setMass(mass);
	}
}