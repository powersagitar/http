if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
        FIND_PACKAGE_ARGS NAMES GTest
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

add_executable(
        socket_unittest
        socket_unittest.cc
)

target_link_libraries(
        socket_unittest
        ${LIB_NAME}
        GTest::gtest_main
)

gtest_discover_tests(socket_unittest)