cmake_minimum_required(VERSION 2.8.4)
project(Common)
set(LIBRARIES_FROM_REFERENCES "")


file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_library(Common ${SOURCE})
#target_link_libraries(Common "${LIBRARIES_FROM_REFERENCES}")

#add_definitions(-D__LINUX__)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")