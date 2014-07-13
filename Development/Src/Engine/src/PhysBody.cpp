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

	PhysBody *PhysBody::CreateBox(const Vec3 &size, Mat4 *_trans, float _mass){

		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxReal cubeDensity = 2.0f;//���������
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxBoxGeometry(size.x, size.y, size.z), *GetPhysics()->mMaterial, cubeDensity);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}
		
		if (_mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(0.5f);
		body->SetLinearDamping(0.5f);
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::CreateSphere(float radius,Mat4 *_trans, float _mass) {
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxReal cubeDensity = 2.0f;//���������
		PxSphereGeometry sphereGeometry(radius);
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), sphereGeometry, *GetPhysics()->mMaterial, cubeDensity);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (body->mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(0.5f);
		body->SetLinearDamping(0.5f);	
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::CreateCylinder(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateCylinder(GetPhysics()->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:����������,����� ��?
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

	PhysBody *PhysBody::CreateCone(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateCone(GetPhysics()->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:����������,����� ��?
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

	PhysBody *PhysBody::CreateCapsule(float radius, float height, Mat4 *_trans, float _mass) {
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxReal cubeDensity = 2.0f;//���������
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxCapsuleGeometry(radius, height / 2), *GetPhysics()->mMaterial, cubeDensity);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (body->mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(0.5f);
		body->SetLinearDamping(0.5f);	
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::CreateChampferCylinder(float radius, float height, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateChamferCylinder(GetPhysics()->nWorld, radius, height, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:����������,����� ��?
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

	PhysBody *PhysBody::CreateConvexHull(Vec3 *pos, const int numPos, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateConvexHull(GetPhysics()->nWorld, numPos, &pos[0].x, 3 * sizeof(float) , 0, NULL, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:����������,����� ��?
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

	PhysBody *PhysBody::CreateStaticMesh(Vec3 *pos, const int numPos, bool optimize) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		//NewtonCollision const*collision = NewtonCreateTreeCollision(GetPhysics()->nWorld, 0);
		//NewtonTreeCollisionBeginBuild(collision);
		//for (int i = 0; i < numPos / 3; i++) {
		//	Vec3 v[3];
		//	v[0] = pos[i * 3 + 0];
		//	v[1] = pos[i * 3 + 1];
		//	v[2] = pos[i * 3 + 2];

		//	NewtonTreeCollisionAddFace(collision, 3, &v[0].x, 3 * sizeof(float) , i+1);
		//}
		//NewtonTreeCollisionEndBuild(collision, 1);
		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, Mat4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1));//Nick:����������,����� ��?
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
		NewtonDestroyBody(GetPhysics()->nWorld, nBody);*/
		SAFE_DELETE(impactSrc);
	}

	void PhysBody::SetTransform(const Mat4 &_transform) {
		if (mActor)
			mActor->setGlobalPose(EngineMathToPhysX((NGTech::Mat4 *)&_transform));
		else
			Debug("PhysBody::setTransform:-mActor is not exist");
	}

	Mat4 PhysBody::GetTransform() {
		auto pos = mActor->getGlobalPose();
		return PhysXToEngineMath(pos);
	}

	void PhysBody::AddTorque(const Vec3 &torque) {
		this->torque += torque;
		this->force += torque * GetMass();
	}

	Vec3 PhysBody::GetTorque() {
		return torque;
	}

	void PhysBody::AddForce(const Vec3 &force) {
	/*	this->torque += force / GetMass();
		this->force += force;*/
	}

	Vec3 PhysBody::GetForce() {
		return force;
	}

	void PhysBody::AddVelocity(const Vec3 &velocity) {
	}

	void PhysBody::SetLinearVelocity(const Vec3 &velocity) {
		if (mActor)
			mActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::GetVelocity() {
		Vec3 velocity(0, 0, 0);
		return velocity;
	}

	float PhysBody::GetMass() {
		if (mActor)
			return mActor->getMass();
		else return 0.0f;
	}

	void PhysBody::SetMass(float mass) {
		if (mActor)
			mActor->setMass(mass);
	}

	void PhysBody::SetLinearDamping(float _v){
		if (mActor)
			mActor->setLinearDamping(_v);
	}

	void PhysBody::SetAngularDamping(float _v){
		if (mActor)
			mActor->setAngularDamping(_v);
	}

	void PhysBody::SetMassSpaceInertiaTensor(const Vec3&_vec){
		if (mActor)
			mActor->setMassSpaceInertiaTensor(PxVec3(_vec.x, _vec.y, _vec.z));
	}
}