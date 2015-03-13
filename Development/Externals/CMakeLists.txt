cmake_minimum_required(VERSION 2.8.4)
project(Externals)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fexceptions -w -fpermissive")
set(LIBRARIES_FROM_REFERENCES "")

include_directories(openal)
include_directories(openal/alc)
include_directories(openal/include)
include_directories(openal/OpenAL32/Include)

#LIBOGG LIBVORBIS
file(GLOB LIBOGGVORBIS
    "libogg/src/*.c"
    "libvorbis/lib/*.c"
    "libvorbis/lib/*.h"
    "libvorbis/lib/modes/*.h"
)

#RESIL DEPENDS
file(GLOB RESIL
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/base/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/bmp/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/jp2/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/jpc/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/jpg/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/mif/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/pgx/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/pnm/*.c"
    "ResIL/Input\ Libs/jasper-1.900.1/src/libjasper/ras/*.c"
    "ResIL/Input\ Libs/jpegsr9a/*.c"
    "ResIL/Input\ Libs/lcms2-2.5/src/*.c"
    "ResIL/Input\ Libs/libmng-2.0.2/*.c"
    "ResIL/Input\ Libs/libpng169/*.c"
    "ResIL/Input\ Libs/tiff-4.0.3/libtiff/*.c"
    "ResIL/Input\ Libs/zlib-1.2.8/*.c"
    "ResIL/src-ILUT/src/*.cpp"
    "ResIL/src-ILU/src/*.cpp"
    "ResIL/src-IL/src/*.cpp"
)
#FREETYPE
add_subdirectory(freetype)

#MYGUI
file(GLOB MYGUI
    "MyGUI/MyGUIEngine/include/*.h"
    "MyGUI/MyGUIEngine/src/*.cpp"
    "MyGUI/Platforms/OpenGL/OpenGLPlatform/src/*.cpp"
    "MyGUI/Platforms/OpenGL/OpenGLPlatform/include/*.h"
)
#TODO:Сделать отдельно OGL рендер для GUI

IF(UNIX)
add_definitions(-D__LINUX__)
ENDIF(UNIX)

add_subdirectory(openal)
add_subdirectory(glfw)

add_library(Externals ${LIBOGGVORBIS} ${RESIL} ${FREETYPE} ${MYGUI} glfw openal)
#Nick
target_link_libraries(Externals "${LIBRARIES_FROM_REFERENCES}")

add_definitions(-D_LIB -DMYGUI_BUILD -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES -D_LARGEFILE_SOURCE -D_LARGE_FILES -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -DHAVE_GCC_DESTRUCTOR)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

