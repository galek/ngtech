//
//	resourceOverride.h
//
//
//	This header file contains product specific resource override code.
//	It remaps max resource ID to derivative product alternatives.
//
//

#ifndef _RESOURCEOVERRIDE_H
#define _RESOURCEOVERRIDE_H

#include "buildver.h"

#ifdef GAME_VER

	// dialogs
	#undef	IDD_EXTRUDEPARAM
	#define IDD_EXTRUDEPARAM		IDD_EXTRUDEPARAM_GMAX

	#undef	IDD_SURFREVPARAM
	#define IDD_SURFREVPARAM		IDD_SURFREVPARAM_GMAX

	#undef	IDD_TESSPARAM
	#define IDD_TESSPARAM			IDD_TESSPARAM_GMAX

	#undef  IDD_OPTIMIZEPARAM
	#define IDD_OPTIMIZEPARAM		IDD_OPTIMIZEPARAM_GMAX

#endif	// GAME_VER

#ifdef WEBVERSION

	// dialogs
	#undef	IDD_EXTRUDEPARAM
	#define IDD_EXTRUDEPARAM		IDD_EXTRUDEPARAM_PLASMA

	#undef	IDD_SURFREVPARAM
	#define IDD_SURFREVPARAM		IDD_SURFREVPARAM_PLASMA

	// strings
	#undef	IDS_RB_UVWMAP_CLASS
	#define IDS_RB_UVWMAP_CLASS		IDS_RB_UVWMAP_CLASS_PLASMA

	#undef	IDS_RB_UVWMAPPING
	#define IDS_RB_UVWMAPPING		IDS_RB_UVWMAPPING_PLASMA

#endif	// WEBVERSION

#ifdef RENDER_VER

	// dialogs
	#undef	IDD_UVWXFORMPARAM
	#define IDD_UVWXFORMPARAM		IDD_UVWXFORMPARAM_VIZR

	#undef	IDD_SMOOTHPARAM
	#define IDD_SMOOTHPARAM			IDD_SMOOTHPARAM_VIZR

	#undef	IDD_MESH_SELECT
	#define IDD_MESH_SELECT			IDD_MESH_SELECT_VIZR

	#undef	IDD_EM_GEOM
	#define IDD_EM_GEOM			IDD_EM_GEOM_VIZR

	#undef	IDD_EM_SELECT
	#define IDD_EM_SELECT			IDD_EM_SELECT_VIZR

	#undef	IDD_EM_SURF_FACE
	#define IDD_EM_SURF_FACE		IDD_EM_SURF_FACE_VIZR

	#undef	IDD_UVWMAPPARAM
	#define IDD_UVWMAPPARAM			IDD_UVWMAPPARAM_VIZR

	// strings
	#undef	IDS_RB_UVWXFORMMOD
	#define IDS_RB_UVWXFORMMOD		IDS_SS_UVWXFORMMOD_VIZR

#endif

#endif	// _RESOURCEOVERRIDE_H
