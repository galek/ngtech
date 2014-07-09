/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
//***************************************************************************

namespace NGTech {
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with box collider
	//Params:  size - size of the box, mass - PhysBody mass, isStatic - is PhysBody static
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
	PhysBody *PhysBody::createBox(const Vec3 &size, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

	/*	NewtonCollision *collision = NewtonCreateBox(GetEngine()->physSystem->nWorld, size.x, size.y, size.z, 0, 0);

		Vec3 inertia, origin;
		NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		float Ixx = mass * inertia.x;
		float Iyy = mass * inertia.y;
		float Izz = mass * inertia.z;

		body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);
		NewtonDestroyCollision(collision);

		NewtonBodySetUserData(body->nBody, body);
		NewtonBodySetAutoSleep(body->nBody, 0);

		if (mass > 0) {
			NewtonBodySetMassMatrix(body->nBody, mass, Ixx, Iyy, Izz);
			NewtonBodySetCentreOfMass(body->nBody, origin);
			NewtonBodySetForceAndTorqueCallback(body->nBody, applyForce_Callback);
		}
		*/
		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with sphere collider
	//Params:  size - radius of the sphere, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
	PhysBody *PhysBody::createSphere(float radius, float mass) {
#if 0
		PhysBody *body = new PhysBody();

		body->force = Vec3(0, 0, 0);
		body->torque = Vec3(0, 0, 0);
		body->impulse = Vec3(0, 0, 0);
		body->velocity = Vec3(0, 0, 0);

		body->mass = mass;

		//NewtonCollision *collision = NewtonCreateSphere(GetEngine()->physSystem->nWorld, radius, 0, 0);

		//Vec3 inertia, origin;
		//NewtonConvexCollisionCalculateInertialMatrix(collision, inertia, origin);
		//float Ixx = mass * inertia.x;
		//float Iyy = mass * inertia.y;
		//float Izz = mass * inertia.z;

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with cylinder collider
	//Params:  radius - radius of the cylinder, height - cylinder height, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with cone collider
	//Params:  radius - radius of the cone, height - cone height, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with capsule collider
	//Params:  radius - radius of the capsule, height - capsule height, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with champfer cylinder collider
	//Params:  radius - radius of the cylinder, height - cylinder height, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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
	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with convex hull collider
	//Params:  pos - array of the hull points, numPos - number of hull points, mass - PhysBody mass
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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

		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, origin);//Nick:����������,����� ��?
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

	//---------------------------------------------------------------------------
	//Desc:    creates PhysBody with object collider
	//Params:  pos - array of vertices, numPos - number of vertices
	//Returns: pointer to the new PhysBody
	//---------------------------------------------------------------------------
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
		//body->nBody = NewtonCreateDynamicBody(GetEngine()->physSystem->nWorld, collision, Mat4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1));//Nick:����������,����� ��?
		//NewtonDestroyCollision(collision);
		//NewtonBodySetMassMatrix(body->nBody, 1.0f, 1.0f, 1.0f, 1.0f);
		//NewtonBodySetUserData(body->nBody, body);

		body->impactSrc = NULL;

		return body;
#else
		return NULL;
#endif
	}

	//---------------------------------------------------------------------------
	//Desc:    PhysBody destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	PhysBody::~PhysBody() {/*
		NewtonDestroyBody(GetEngine()->physSystem->nWorld, nBody);*/
		SAFE_DELETE(impactSrc);
	}

	//***************************************************************************
	//Set and get transform
	//***************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    sets PhysBody transform
	//Params:  trans - transform matrix
	//Returns: -
	//---------------------------------------------------------------------------
	void PhysBody::setTransform(const Mat4 &transform) {
	}

	//---------------------------------------------------------------------------
	//Desc:    gets PhysBody transform
	//Params:  -
	//Returns: PhysBody transform matrix
	//---------------------------------------------------------------------------
	Mat4 PhysBody::getTransform() {
		Mat4 out;
		out.identity();
		return out;
	}

	//***************************************************************************
	//Set and get velocity
	//***************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    adds torque to PhysBody
	//Params:  torque - torque vector
	//Returns: -
	//---------------------------------------------------------------------------
	void PhysBody::addTorque(const Vec3 &torque) {
		this->torque += torque;
		this->force += torque * getMass();
	}

	Vec3 PhysBody::getTorque() {
		return torque;
	}

	void PhysBody::addForce(const Vec3 &force) {
		this->torque += force / getMass();
		this->force += force;
	}

	Vec3 PhysBody::getForce() {
		return force;
	}

	void PhysBody::addVelocity(const Vec3 &velocity) {
	}

	void PhysBody::setVelocity(const Vec3 &velocity) {
	}

	Vec3 PhysBody::getVelocity() {
		Vec3 velocity(0, 0, 0);
		return velocity;
	}
	
	float PhysBody::getMass() {
		float Ixx, Iyy, Izz;
		float mass;

		return 0;//mass;
	}

	void PhysBody::setMass(float mass) {
	}
}