cmake_minimum_required(VERSION 2.8.4)
project(Engine)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fexceptions -w -fpermissive")
set(LIBRARIES_FROM_REFERENCES "")

file(GLOB SOURCE
    "inc/*.h"
    "src/*.cpp"
    "inc/*.inl"
)

#add_library(Engine SHARED ${SOURCE})
add_library(Engine ${SOURCE})

MACRO(ADD_PHYSX_LIBRARY LIBNAME)
find_library( ${LIBNAME}_LIBRARY
NAMES
${LIBNAME}${PHYSX_LIBRARY_TYPE}
${LIBNAME}${PHYSX_LIBRARY_TYPE}_x64
PATHS ../../Externals/physx/Lib/linux64
)
set(PHYSX_LIBRARY ${PHYSX_LIBRARY} ${${LIBNAME}_LIBRARY})
ENDMACRO()

ADD_PHYSX_LIBRARY( PvdRuntime )
ADD_PHYSX_LIBRARY( SimulationController )
ADD_PHYSX_LIBRARY( SceneQuery )
ADD_PHYSX_LIBRARY( LowLevel )
ADD_PHYSX_LIBRARY( LowLevelCloth )
ADD_PHYSX_LIBRARY( PhysX3Vehicle )
ADD_PHYSX_LIBRARY( PhysX3Cooking )
ADD_PHYSX_LIBRARY( PhysX3Extensions )
ADD_PHYSX_LIBRARY( PhysX3CharacterKinematic )
ADD_PHYSX_LIBRARY( PhysXProfileSDK )
ADD_PHYSX_LIBRARY( PhysXVisualDebuggerSDK )
#ADD_PHYSX_LIBRARY( PhysX3CHECKED )

IF(UNIX AND NOT MAC)

target_link_libraries(Engine
-Wl,--start-group
"${LIBRARIES_FROM_REFERENCES}" Common Core OGLDRV Platform Externals

GL GLU X11 Xxf86vm rt pthread
-Wl,--end-group
)
ELSE(UNIX AND NOT MAC)

target_link_libraries(Engine
"${LIBRARIES_FROM_REFERENCES}" Common Core OGLDRV Platform Externals
)
ENDIF(UNIX AND NOT MAC)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")

IF(UNIX)
add_definitions(-D__LINUX__)
ENDIF(UNIX)

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
	add_definitions(-DENGINE_EXPORTS)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	 message(SEND_ERROR "Nick:Write on gcc")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
endif()


set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
