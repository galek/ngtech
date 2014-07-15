#include "EnginePrivate.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
//***************************************************************************
#include "PxPhysicsAPI.h"
//***************************************************************************
#include "EngineMathToPhysx.inl"
#include "PhysXUtils.h"
//***************************************************************************

namespace NGTech {
	using namespace physx;

	PhysBody *PhysBody::CreateBox(const Vec3 &size, Mat4 *_trans, float _mass){

		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxBoxGeometry(size.x / 2, size.y / 2, size.z / 2), *GetPhysics()->mMaterial, 1.0f);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (_mass > 0)
			body->mActor->setMass(body->mass);

		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));

		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::CreateSphere(float radius, Mat4 *_trans, float _mass) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		PxSphereGeometry sphereGeometry(radius);
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), sphereGeometry, *GetPhysics()->mMaterial, 1.0f);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (_mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
	}
	//NOTE:Вроде подобрал число,что бы в дебагере было похоже на круг у основания цилиндра
	PhysBody *PhysBody::CreateCylinder(float radius, float width, Mat4 *_trans, float mass) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mass = mass;
		PxConvexMesh* mesh = createCylinderConvexMesh(width, radius, 60, *GetPhysics()->mPhysics, *GetPhysics()->mCooking);

		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxConvexMeshGeometry(mesh), *GetPhysics()->mMaterial, 1.0f);

		SetupDefaultRigidDynamic(*body->mActor);

		if (body->mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		body->impactSrc = NULL;

		return body;
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

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
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

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mass = _mass;

		// Initialize Cube Actor
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxCapsuleGeometry(radius, height / 2), *GetPhysics()->mMaterial, 1.0f);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (body->mass > 0)
			body->mActor->setMass(body->mass);
		body->SetMassSpaceInertiaTensor(Vec3(1.0f, 1.0f, 1.0f));
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
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

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
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

		//body->nBody = NewtonCreateDynamicBody(GetPhysics()->nWorld, collision, origin);//Nick:Сомневаюсь,верно ли?
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
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->impactSrc = NULL;

		return body;
	}

	PhysBody::~PhysBody() {
		GetPhysics()->mScene->removeActor(*mActor);
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

	void PhysBody::AddTorque(const Vec3 &_torque) {
		if (mActor)
			mActor->addTorque(PxVec3(_torque.x, _torque.y, _torque.z));
	}


	void PhysBody::AddForce(const Vec3 &force) {
		if (mActor)
			mActor->addForce(PxVec3(force.x, force.y, force.z));
	}
	

	void PhysBody::SetLinearVelocity(const Vec3 &velocity) {
		mLvelocity = velocity;
		if (mActor)
			mActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::GetLinearVelocity() {
		if (mActor){
			Vec3 velocity;
			auto mPxVec = mActor->getLinearVelocity();
			velocity = { mPxVec.x, mPxVec.y, mPxVec.z };
			return velocity;
		}
		return mLvelocity;
	}

	void PhysBody::SetAngularVelocity(const Vec3 &velocity) {
		mAvelocity = velocity;
		if (mActor)
			mActor->setAngularVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::GetAngularVelocity() {
		if (mActor){
			Vec3 velocity;
			auto mPxVec = mActor->getAngularVelocity();
			velocity = { mPxVec.x, mPxVec.y, mPxVec.z };
		}
		return mAvelocity;
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