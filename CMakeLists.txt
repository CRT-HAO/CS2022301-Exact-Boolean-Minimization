cmake_minimum_required(VERSION 3.18)

project(exact_boolean_minimization)

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp
  ${SRC_DIR}/boolean_algebra.cpp
  ${SRC_DIR}/qmc.cpp
  ${SRC_DIR}/petrick_method.cpp
  ${SRC_DIR}/pla.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})