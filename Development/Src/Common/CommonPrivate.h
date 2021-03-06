#pragma once

//STL and types
#include "Types.h"
#include <assert.h>
#include <thread>
#include <mutex>

// disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
#pragma warning (disable : 4275)

#include "IWindow.h"
#include "IRender.h"
#include "IGUI.h"
#include "IILImage.h"
#include "IGame.h"