#include "EnginePrivate.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Model.h"
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
			((PxRigidBody*)body->mActor)->setMass(body->mass);

		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);

		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

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
			((PxRigidBody*)body->mActor)->setMass(body->mass);

		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);

		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

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

		SetupDefaultRigidDynamic(*(PxRigidDynamic*)body->mActor);

		if (body->mass > 0)
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		return body;
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
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		return body;
	}

	PhysBody *PhysBody::CreateConvexHull(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices, float _mass) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mActor = NULL;
		body->mass = _mass;

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = _numVert;
		convexDesc.points.stride = sizeof(Model::Vertex);
		convexDesc.points.data = ((Model::Vertex*)_vertices);
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		convexDesc.vertexLimit = 256;

		PxDefaultMemoryOutputStream  buffer;
		bool status = GetPhysics()->GetPxCooking()->cookConvexMesh(convexDesc, buffer);
		if (!status){
			Warning("[Physics]Cooking failed");
			return NULL;
		}
		PxDefaultMemoryInputData input(buffer.getData(), buffer.getSize());

		PxConvexMesh* triangleMesh = GetPhysics()->GetPxPhysics()->createConvexMesh(input);


		// Initialize Convex Actor
		body->mActor = PxCreateDynamic(*GetPhysics()->mPhysics, EngineMathToPhysX(_trans), PxConvexMeshGeometry(triangleMesh), *GetPhysics()->mMaterial, 1.0f);
		if (!body->mActor){
			Warning("create actor failed!");
			return NULL;
		}

		if (body->mass > 0)
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
		body->SetAngularDamping(1.0f);
		body->SetLinearDamping(1.0f);
		GetPhysics()->mScene->addActor(*body->mActor);

		return body;
	}

	PhysBody *PhysBody::CreateStaticMesh(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->mActor = NULL;

		PxTriangleMeshDesc description;

		description.points.count = _numVert;
		description.points.stride = sizeof(Model::Vertex);
		description.points.data = ((Model::Vertex*)_vertices);

		description.triangles.count = _numFaces / 3;
		description.triangles.stride = sizeof(unsigned int) * 3;
		description.triangles.data = _indices;

		if (!GetPhysics()->GetPxCooking()->validateTriangleMesh(description)){
			Debug("[Physics]Is not valid triangle mesh");
		}

		PxDefaultMemoryOutputStream  buffer;
		bool status = GetPhysics()->GetPxCooking()->cookTriangleMesh(description, buffer);
		if (!status){
			Warning("[Physics]Cooking failed");
			return NULL;
		}
		PxDefaultMemoryInputData input(buffer.getData(), buffer.getSize());

		PxTriangleMesh* triangleMesh = GetPhysics()->GetPxPhysics()->createTriangleMesh(input);

		body->mActor = GetPhysics()->GetPxPhysics()->createRigidStatic(EngineMathToPhysX(_trans));
		body->mShape = body->mActor->createShape(PxTriangleMeshGeometry(triangleMesh), *GetPhysics()->mMaterial);
		GetPhysics()->mScene->addActor(*body->mActor);

		return body;
	}


	PhysBody::PhysBody()
		:impactSrc(nullptr),
		mShape(nullptr),
		mActor(nullptr),
		mass(0.0f)
	{}

	PhysBody::PhysBody(const PhysBody&_copy)
		: impactSrc(_copy.impactSrc),
		mShape(_copy.mShape), 
		mActor(_copy.mActor),
		mass(_copy.mass)
	{
	}

	PhysBody::~PhysBody() {
		Warning(__FUNCTION__);
		GetPhysics()->mScene->removeActor(*mActor);
		SAFE_DELETE(impactSrc);
	}

	void PhysBody::SetTransform(const Mat4 &_transform) {
		if (mActor)
		{
			GetPhysics()->LockWrite();
			mActor->setGlobalPose(EngineMathToPhysX((NGTech::Mat4 *)&_transform));
			GetPhysics()->UnLockWrite();
		}
		else
			Debug("PhysBody::setTransform:-mActor is not exist");
	}

	Mat4 PhysBody::GetTransform() {
		if (mActor){
			GetPhysics()->LockWrite();
			auto pos = mActor->getGlobalPose();
			GetPhysics()->UnLockWrite();
			return PhysXToEngineMath(pos);
		}
		else
		{
			Mat4 t;
			return t.identity();
		}
	}

	void PhysBody::AddTorque(const Vec3 &_torque) {
		if (mActor)
			((PxRigidBody*)mActor)->addTorque(PxVec3(_torque.x, _torque.y, _torque.z));
	}


	void PhysBody::AddForce(const Vec3 &force) {
		if (mActor)
			((PxRigidBody*)mActor)->addForce(PxVec3(force.x, force.y, force.z));
	}


	void PhysBody::SetLinearVelocity(const Vec3 &velocity) {
		mLvelocity = velocity;
		if (mActor)
			((PxRigidBody*)mActor)->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::GetLinearVelocity() {
		if (mActor){
			Vec3 velocity;
			auto mPxVec = ((PxRigidBody*)mActor)->getLinearVelocity();
			velocity = { mPxVec.x, mPxVec.y, mPxVec.z };
			return velocity;
		}
		return mLvelocity;
	}

	void PhysBody::SetAngularVelocity(const Vec3 &velocity) {
		mAvelocity = velocity;
		if (mActor)
			((PxRigidBody*)mActor)->setAngularVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	Vec3 PhysBody::GetAngularVelocity() {
		if (mActor){
			Vec3 velocity;
			auto mPxVec = ((PxRigidBody*)mActor)->getAngularVelocity();
			velocity = { mPxVec.x, mPxVec.y, mPxVec.z };
		}
		return mAvelocity;
	}

	float PhysBody::GetMass() {
		if (mActor)
			return ((PxRigidBody*)mActor)->getMass();
		else return 0.0f;
	}

	void PhysBody::SetMass(float mass) {
		if (mActor)
			((PxRigidBody*)mActor)->setMass(mass);
	}

	void PhysBody::SetLinearDamping(float _v){
		if (mActor) {
			PxRigidDynamic* mNActor = (PxRigidDynamic*)mActor;
			mNActor->setLinearDamping(_v);
		}
	}

	void PhysBody::SetAngularDamping(float _v){
		if (mActor) {
			PxRigidDynamic* mNActor = (PxRigidDynamic*)mActor;
			mNActor->setAngularDamping(_v);
		}
	}

	void PhysBody::SetMassSpaceInertiaTensor(const Vec3&_vec){
		if (mActor)
			((PxRigidBody*)mActor)->setMassSpaceInertiaTensor(PxVec3(_vec.x, _vec.y, _vec.z));
	}
}