cmake_minimum_required( VERSION 3.10 FATAL_ERROR )

project(untitled LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV.
find_package( OpenCV REQUIRED )
MESSAGE( STATUS "<< PROJ_NAME ${PROJECT_NAME} >>" )
MESSAGE( STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}" )
MESSAGE( STATUS "LINK LIBRARIES ${OpenCV_LIBS}" )

include_directories( ${CMAKE_SOURCE_DIR} )
include( ${CMAKE_CURRENT_SOURCE_DIR}/src/src.cmake )


add_executable( ${PROJECT_NAME}
                src/main.cpp
                ${SRC} )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
