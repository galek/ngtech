// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.

#ifndef PX_VEHICLE_METADATAOBJECTS_H
#define PX_VEHICLE_METADATAOBJECTS_H
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "PxMetaDataObjects.h"
#include "PxExtensionMetaDataObjects.h"

/** \addtogroup physics
@{
*/

namespace physx
{

struct PxVehiclePropertyInfoName
{
	enum Enum
	{
		Unnamed = PxExtensionsPropertyInfoName::LastPxPropertyInfoName,
#include "PxVehicleAutoGeneratedMetaDataObjectNames.h"
		LastPxPropertyInfoName
	};
};
		
#define DEFINE_PROPERTY_TO_VALUE_STRUCT_MAP( type, prop, valueStruct )																	\
	template<> struct PxPropertyToValueStructMemberMap< PxVehiclePropertyInfoName::type##_##prop >									\
	{																																	\
		PxU32 Offset;																													\
		PxPropertyToValueStructMemberMap< PxVehiclePropertyInfoName::type##_##prop >() : Offset( offsetof( valueStruct, prop ) ) {}	\
		template<typename TOperator> void visitProp( TOperator inOperator, valueStruct& inStruct ) { inOperator( inStruct.prop );	}	\
	};

struct MFrictionVsSlipGraphProperty : public PxExtendedDualIndexedPropertyInfo<PxVehiclePropertyInfoName::PxVehicleTireData_MFrictionVsSlipGraph
																			, PxVehicleTireData
																			, PxU32
																			, PxU32
																			, PxReal> 
{
	PX_PHYSX_CORE_API MFrictionVsSlipGraphProperty();
};

struct MTorqueCurveProperty : public PxExtendedIndexedPropertyInfo<PxVehiclePropertyInfoName::PxVehicleEngineData_MTorqueCurve
																			, PxVehicleEngineData
																			, PxU32
																			, PxReal>
{
	PX_PHYSX_CORE_API MTorqueCurveProperty();
};

#include "PxVehicleAutoGeneratedMetaDataObjects.h"

#undef DEFINE_PROPERTY_TO_VALUE_STRUCT_MAP

}

/** @} */
#endif
