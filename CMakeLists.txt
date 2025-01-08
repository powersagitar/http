cmake_minimum_required(VERSION 3.30)

project(http)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_NAME ${PROJECT_NAME})
set(CLI_EXE_NAME ${PROJECT_NAME}-cli)

set(LIB_SRC_DIR "src/http/core")
set(CLI_EXE_SRC_DIR "src/http/cli")

include(FetchContent)

FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.0.tar.gz
        FIND_PACKAGE_ARGS NAMES spdlog
)

FetchContent_Declare(
        boost
        URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.bz2
)

FetchContent_MakeAvailable(spdlog boost)

# Build Boost binaries
set(BOOST_SRC_DIR ${FETCHCONTENT_BASE_DIR}/boost-src)
set(BOOST_BINARIES_DIR ${FETCHCONTENT_BASE_DIR}/boost-build)

add_custom_command(
        OUTPUT ${BOOST_SRC_DIR}/b2
        COMMAND ./bootstrap.sh --prefix=${BOOST_BINARIES_DIR}
        WORKING_DIRECTORY ${BOOST_SRC_DIR}
)

add_custom_command(
        DEPENDS ${BOOST_SRC_DIR}/b2
        OUTPUT ${BOOST_BINARIES_DIR}/lib/libboost_program_options.dylib
        COMMAND ./b2 install --with-program_options
        WORKING_DIRECTORY ${BOOST_SRC_DIR}
)

add_custom_target(
        boost-program-options
        DEPENDS ${BOOST_BINARIES_DIR}/lib/libboost_program_options.dylib
)

include_directories(
        src
)

add_library(${LIB_NAME} SHARED)

target_include_directories(
        ${LIB_NAME} PRIVATE
        ${BOOST_SRC_DIR}
)

target_sources(
        ${LIB_NAME} PRIVATE
        ${LIB_SRC_DIR}/http.cc
        ${LIB_SRC_DIR}/request.cc
        ${LIB_SRC_DIR}/response.cc
        ${LIB_SRC_DIR}/tcp.cc
)

target_link_libraries(
        ${LIB_NAME}
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

add_executable(${CLI_EXE_NAME})

set_target_properties(
        ${CLI_EXE_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
)

add_dependencies(
        ${CLI_EXE_NAME}
        boost-program-options
)

target_include_directories(
        ${CLI_EXE_NAME} PRIVATE
        ${BOOST_SRC_DIR}
)

target_sources(
        ${CLI_EXE_NAME} PRIVATE
        ${CLI_EXE_SRC_DIR}/main.cc
)

target_link_directories(
        ${CLI_EXE_NAME} PRIVATE
        ${BOOST_BINARIES_DIR}/lib/
)

target_link_libraries(
        ${CLI_EXE_NAME}
        ${LIB_NAME}
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
        boost_program_options
)

add_subdirectory(tests)