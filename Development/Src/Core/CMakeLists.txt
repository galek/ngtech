cmake_minimum_required(VERSION 2.8.4)
project(Core)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fexceptions -w -fpermissive")
set(LIBRARIES_FROM_REFERENCES "")

file(GLOB SOURCE
    "inc/*.h"
    "inc/*.cpp"
    "inc/FileSystemInfo/*.h"
    "inc/License/*.h"
    "inc/lua/*.h"
    "inc/lua/*.hpp"
    "inc/lua/detail/*.hpp"
    "inc/lua/detail/conversion_policies/*.hpp"
    "src/*.h"
    "src/*.cpp"
    "src/License/*.cpp"
    "src/lua/*.cpp"
    "src/lua/*.c"
)

#add_library(Core SHARED ${SOURCE})
add_library(Core ${SOURCE})

IF(UNIX AND NOT MAC)
target_link_libraries(Core "${LIBRARIES_FROM_REFERENCES}" Common Externals dl rt pthread )
ELSE(UNIX AND NOT MAC)
target_link_libraries(Core "${LIBRARIES_FROM_REFERENCES}" Common Externals )
ENDIF(UNIX AND NOT MAC)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")

IF(UNIX)
add_definitions(-D__LINUX__)
ENDIF(UNIX)

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
	add_definitions(-DCORE_EXPORTS)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	 message(SEND_ERROR "Nick:Write on gcc")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
endif()


set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
