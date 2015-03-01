#pragma once

//STL and types
#include "Types.h"
#include <assert.h>
//***************************************************************************
#include "../Platform/inc/PlatformPrivate.h"
//***************************************************************************

// disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
#pragma warning (disable : 4275)

#include "StringHelper.h"
#include "MathLib.h"
#include "IWindow.h"
#include "IRender.h"
#include "IGUI.h"
#include "I_MeshFormat.h"
#include "I_MeshFormatSkinned.h"
#include "IILImage.h"
#include "IGame.h"
#include "Utils.h"
#include "DebugLayer.h"