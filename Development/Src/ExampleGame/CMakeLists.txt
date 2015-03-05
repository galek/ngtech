cmake_minimum_required(VERSION 2.8.4)
project(ExampleGame)
set(LIBRARIES_FROM_REFERENCES "")


file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_library(ExampleGame ${SOURCE})
target_link_libraries(ExampleGame "${LIBRARIES_FROM_REFERENCES}")

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
  #TODO: Тут сделай тоже макрос
	add_definitions(-DGAME_EXPORTS)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	 message(SEND_ERROR "Nick:Write on gcc")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")