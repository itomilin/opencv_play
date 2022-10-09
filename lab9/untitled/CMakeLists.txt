cmake_minimum_required(VERSION 3.5)

project(untitled LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV.
find_package( OpenCV REQUIRED )
MESSAGE( STATUS "<< PROJ_NAME ${PROJECT_NAME} >>" )
MESSAGE( STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}" )
MESSAGE( STATUS "LINK LIBRARIES ${OpenCV_LIBS}" )

add_executable(untitled main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
