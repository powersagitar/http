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
        tcp_socket_unittest
        tcp_socket_unittest.cc
)

target_link_libraries(
        tcp_socket_unittest
        ${LIB_NAME}
        GTest::gtest_main
)

gtest_discover_tests(tcp_socket_unittest)

add_executable(
        http_request_unittest
        http_request_unittest.cc
)

target_link_libraries(
        http_request_unittest
        ${LIB_NAME}
        GTest::gtest_main
)

gtest_discover_tests(http_request_unittest)