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

	PxConvexMesh* createConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics& physics, PxCooking& cooking)
	{
		// Create descriptor for convex mesh
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = numVerts;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = verts;
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eINFLATE_CONVEX;

		PxConvexMesh* convexMesh = NULL;
		PxDefaultMemoryOutputStream buf;
		if (cooking.cookConvexMesh(convexDesc, buf))
		{
			PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
			convexMesh = physics.createConvexMesh(id);
		}

		return convexMesh;
	}

	PxConvexMesh* createCylinderConvexMesh(const PxF32 width, const PxF32 radius, const PxU32 numCirclePoints, PxPhysics& physics, PxCooking& cooking)
	{
#define  MAX_NUM_VERTS_IN_CIRCLE 100
		PX_ASSERT(numCirclePoints < MAX_NUM_VERTS_IN_CIRCLE);
		PxVec3 verts[2 * MAX_NUM_VERTS_IN_CIRCLE];
		PxU32 numVerts = 2 * numCirclePoints;
		const PxF32 dtheta = 2 * PxPi / (1.0f*numCirclePoints);
		for (PxU32 i = 0; i < MAX_NUM_VERTS_IN_CIRCLE; i++)
		{
			const PxF32 theta = dtheta*i;
			const PxF32 cosTheta = radius*PxCos(theta);
			const PxF32 sinTheta = radius*PxSin(theta);
			verts[2 * i + 0] = PxVec3(-0.5f*width, cosTheta, sinTheta);
			verts[2 * i + 1] = PxVec3(+0.5f*width, cosTheta, sinTheta);
		}

		return createConvexMesh(verts, numVerts, physics, cooking);
	}

	void SetupDefaultRigidDynamic(PxRigidDynamic& body, bool kinematic)
	{
		body.setActorFlag(PxActorFlag::eVISUALIZATION, true);
		body.setAngularDamping(0.5f);
		body.setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
	}
}