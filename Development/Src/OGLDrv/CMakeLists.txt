cmake_minimum_required(VERSION 2.8.4)
project(OGLDRV)
set(LIBRARIES_FROM_REFERENCES "")


file(GLOB SOURCE
    "inc/*.h"
    "src/*.cpp"
    "src/*.cc"
)

add_library(OGLDRV ${SOURCE})
#add_executable(OGLDRV ${SOURCE})
target_link_libraries(OGLDRV "${LIBRARIES_FROM_REFERENCES}" Common Core Platform)

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
  #TODO: ��� ������ ���� ������
	add_definitions(-DRENDER_EXPORTS)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	 message(SEND_ERROR "Nick:Write on gcc")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")