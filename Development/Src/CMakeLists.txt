cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fexceptions -w -fpermissive")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/nick/develop/Development/BinOut")

project(Launcher)

#Externals Inlcudes
include_directories(../Externals/freetype/include)
include_directories(../Externals/freetype)
include_directories(../Externals/MyGUI/Common)
include_directories(../Externals/MyGUI/Platforms/OpenGL/OpenGLPlatform/include)
include_directories(../Externals/MyGUI/MyGUIEngine/include)
include_directories(../Externals/MyGUI/MyGUIEngine/include)
include_directories(../Externals/PhysX/Include)
include_directories(../Externals/PhysX/source/Common/src)
include_directories(../Externals/PhysX/source/foundation/include)
include_directories(../Externals/PhysX/source/GeomUtils/headers)
include_directories(../Externals/PhysX/source/PhysXCharacterKinematic/src)
include_directories(../Externals/PhysX/source/PhysXExtensions/src)
include_directories(../Externals/PhysX/source/PhysXMetaData/core/include)
include_directories(../Externals/PhysX/source/PhysXMetaData/extensions/include)
include_directories(../Externals/PhysX/source/PhysXProfileSDK)
include_directories(../Externals/PhysX/source/PhysXVehicle/src)
include_directories(../Externals/PhysX/source/PhysXVehicle/src/PhysXMetaData/include)
include_directories(../Externals/PhysX/source/PvdRuntime/src)

#Source Includes
include_directories(inc)
include_directories(Common)
include_directories(GUI)
include_directories(Platform/inc)
include_directories(Platform/inc/glfw)
include_directories(Platform/inc/crashreport)
include_directories(Platform/inc/crashreport/common)
include_directories(Platform/inc/crashreport/common/windows)
include_directories(Platform/inc/crashreport/processor)
include_directories(Platform/inc/crashreport/windows)
include_directories(Platform/inc/crashreport/windows/common)
include_directories(Platform/inc/crashreport/windows/crash_generation)
include_directories(Platform/inc/crashreport/windows/handler)
include_directories(Core/inc)
include_directories(Core/inc/FileSystemInfo)
include_directories(Core/inc/License)
include_directories(Core/inc/Lua)
include_directories(Engine/inc)
include_directories(OGLDrv/inc)
include_directories(ExampleGame)
include_directories(Launcher)


IF(UNIX)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)


find_package(Threads)

find_package(X11 REQUIRED)

if(NOT X11_FOUND)
message(FATAL_ERROR "Failed to find X11")
endif(NOT X11_FOUND)




#if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fexceptions -w -fpermissive")
#set(CMAKE_EXE_LINKER_FLAGS "-lpthread")

#endif()

add_definitions(-D__LINUX__)
ENDIF(UNIX)


find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
message(FATAL_ERROR "Failed to find opengl")
endif(NOT OPENGL_FOUND)



add_subdirectory(../Externals "${CMAKE_CURRENT_BINARY_DIR}/Externals")
add_subdirectory(Platform)
add_subdirectory(Common)
add_subdirectory(Core)
add_subdirectory(OGLDRV)
add_subdirectory(Engine)
add_subdirectory(ExampleGame)
add_subdirectory(Launcher)


set(SOURCE_FILES ${SOURCE})