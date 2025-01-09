cmake_minimum_required(VERSION 3.30)

project(http)

if(WIN32)
  message(FATAL_ERROR "${PROJECT_NAME} is not compilable on Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_NAME ${PROJECT_NAME})
set(CLI_EXE_NAME ${PROJECT_NAME}-cli)

set(LIB_SRC_DIR "src/http/core")
set(CLI_EXE_SRC_DIR "src/http/cli")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FetchContent)

FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.0.tar.gz
    FIND_PACKAGE_ARGS NAMES spdlog
)

FetchContent_MakeAvailable(spdlog)

include(FetchBoost)

add_boost_component(program_options)

build_boost_components()

add_library(${LIB_NAME} SHARED)

target_include_directories(
    ${LIB_NAME} PUBLIC
    src
)

target_include_directories(
    ${LIB_NAME} PRIVATE
    ${BOOST_INCLUDE_DIR}
)

target_sources(
    ${LIB_NAME} PRIVATE
    ${LIB_SRC_DIR}/http.cc
    ${LIB_SRC_DIR}/request.cc
    ${LIB_SRC_DIR}/response.cc
    ${LIB_SRC_DIR}/tcp.cc
)

target_link_libraries(
    ${LIB_NAME} PUBLIC
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

add_executable(${CLI_EXE_NAME})

set_target_properties(
    ${CLI_EXE_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)

target_sources(
    ${CLI_EXE_NAME} PRIVATE
    ${CLI_EXE_SRC_DIR}/main.cc
)

target_link_libraries(
    ${CLI_EXE_NAME} PRIVATE
    ${LIB_NAME}
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    boost_program_options_shared
)

add_subdirectory(tests)
