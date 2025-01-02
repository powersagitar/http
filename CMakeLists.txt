cmake_minimum_required(VERSION 3.30)

project(http)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED)

set(LIB_SRC_DIR "src/http/core")

target_sources(
        ${PROJECT_NAME} PRIVATE
        ${LIB_SRC_DIR}/socket.cc
)

include_directories(
        include
        src
)

add_executable(${PROJECT_NAME}-cli)

set_target_properties(
        ${PROJECT_NAME}-cli
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
)

set(CLI_EXE_SRC_DIR "src/http/cli")

target_sources(
        ${PROJECT_NAME}-cli PRIVATE
        ${CLI_EXE_SRC_DIR}/main.cc
)

add_subdirectory(tests)