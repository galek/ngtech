cmake_minimum_required(VERSION 2.8.4)
project(Common)
set(LIBRARIES_FROM_REFERENCES "")


file(GLOB SOURCE
    "inc/*.h"
    "inc/crashreport/common/*.h"
    "inc/crashreport/common/windows/*.h"
    "inc/crashreport/processor/*.h"
    "inc/crashreport/windows/*.h"
    "inc/crashreport/windows/common/*.h"
    "inc/crashreport/windows/crash_generation/*.h"
    "inc/crashreport/windows/handler/*.h"
    "inc/glfw/*.h"
    "src/*.cpp"
    "src/glfw/*.cpp"
    "src/glfw/*.c"
#   "src/glfw/*.m"
    "src/crashreport/*.cpp"
    "src/crashreport/common/windows/*.cc"
    "src/crashreport/windows/crash_generation/*.cc"
    "src/crashreport/windows/handler/*.cc"
)

add_library(Platform ${SOURCE})
#target_link_libraries(Platform "${LIBRARIES_FROM_REFERENCES}")

#TODO:Сделать поддержку OPENGLES
IF(UNIX)
add_definitions(-D_GLFW_USE_OPENGL -D_GLFW_GLX -D_GLFW_X11)
ELSE()
add_definitions(-D_GLFW_USE_OPENGL -D_GLFW_WGL -D_GLFW_WIN32)
ENDIF(UNIX)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")