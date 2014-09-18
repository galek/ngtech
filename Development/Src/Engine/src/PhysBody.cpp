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
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
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

		SetupDefaultRigidDynamic(*(PxRigidDynamic*)body->mActor);

		if (body->mass > 0)
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
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
			((PxRigidBody*)body->mActor)->setMass(body->mass);
		PxRigidBodyExt::updateMassAndInertia(*((PxRigidBody*)body->mActor), 1.0f);
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

	PhysBody *PhysBody::CreateConvexHull(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices, float _mass) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->impactSrc = NULL;
		body->mActor = NULL;
		body->mShape = NULL;
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

		body->impactSrc = NULL;

		return body;
	}

	PhysBody *PhysBody::CreateStaticMesh(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices) {
		PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->impactSrc = NULL;
		body->mActor = NULL;
		body->mShape = NULL;

		PxTriangleMeshDesc description;

		description.points.count = _numVert;
		description.points.stride = sizeof(Model::Vertex);
		description.points.data = ((Model::Vertex*)_vertices);

		description.triangles.count = _numFaces / 3;
		description.triangles.stride = sizeof(unsigned int) * 3;
		description.triangles.data = _indices;


		Warning(" nbVertices %i", description.points.count);
		Warning(" nbTriangles %i", description.triangles.count);

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

	PhysBody *PhysBody::CreateCloth(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices) {
		/*PhysBody *body = new PhysBody();

		body->mLvelocity = Vec3(0, 0, 0);
		body->mAvelocity = Vec3(0, 0, 0);

		body->impactSrc = NULL;
		body->mActor = NULL;
		body->mShape = NULL;

		PxClothParticle vertices[] = {
			PxClothParticle(PxVec3(0.0f, 0.0f, 0.0f), 0.0f),
			PxClothParticle(PxVec3(0.0f, 1.0f, 0.0f), 1.0f),
			PxClothParticle(PxVec3(1.0f, 0.0f, 0.0f), 1.0f),
			PxClothParticle(PxVec3(1.0f, 1.0f, 0.0f), 1.0f)
		};
		PxU32 primitives[] = { 0, 1, 3, 2 };

		PxClothMeshDesc  description;

		description.points.data = vertices;
		description.points.count = 4;
		description.points.stride = sizeof(PxClothParticle);

		description.invMasses.data = &vertices->invWeight;
		description.invMasses.count = 4;
		description.invMasses.stride = sizeof(PxClothParticle);

		description.quads.data = primitives;
		description.quads.count = 1;
		description.quads.stride = sizeof(PxU32) * 4;

		Vec3 engrav = GetPhysics()->GetGravity();
		PxVec3 grav = { engrav.x, engrav.y, engrav.z };
		PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics()->GetPxPhysics(), description, grav);


	
		auto mCloth = GetPhysics()->GetPxPhysics()->createCloth(EngineMathToPhysX(_trans), *fabric, vertices, PxClothFlags());
		GetPhysics()->mScene->addActor(*mCloth);

		return body;*/
		return NULL;
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
		if (mActor){
			auto pos = mActor->getGlobalPose();
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