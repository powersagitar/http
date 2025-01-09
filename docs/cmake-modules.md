# CMake Modules

All cmake modules are defined in `/cmake`.

## FetchBoost.cmake

### Description

This module downloads Boost v1.87.0 to [`FETCHCONTENT_BASE_DIR/boost-src`](https://cmake.org/cmake/help/latest/module/FetchContent.html#variable:FETCHCONTENT_BASE_DIR)
via FetchContent.

It exports a few Boost-related variables and library targets as requested.

### Variables

- `BOOST_SRC_DIR` -- Path to the downloaded Boost source directory.
- `BOOST_INCLUDE_DIR` -- Boost include directory. I.e., directory for `-I` compiler
  flag.
- `BOOST_BUILD_DIR` -- Directory where Boost is installed.
- `BOOST_LIB_DIR` -- Boost library directory. I.e., directory for `-L` compiler
  flag.
- `BOOST_B2_DIR` -- Where [b2](https://www.boost.org/doc/libs/1_87_0/tools/build/doc/html/index.html)
  is installed.
- `BOOST_B2_PATH` -- Path to [b2](https://www.boost.org/doc/libs/1_87_0/tools/build/doc/html/index.html)
  executable.
- `BOOST_COMPONENTS` -- Boost components to build.

### Functions

#### add_boost_component(component)

Recommended method to append to `BOOST_COMPONENTS`, where `component` is the name
of a Boost component.

`component` is later prepended with `--with-` and passed to b2.

To read more, see [Stack Overflow
answer](https://stackoverflow.com/a/4716251/20143641).

#### build_boost_components()

Build and export components specified in `BOOST_COMPONENTS`. Function is meant
to be called after all components have been added.

It exports a shared library target for each component, in the name of
`boost_<component>_shared`, and registers the associated include directories.

For exapmle, if a component `program_options` is added,
`boost_program_options_shared` will be exported. You can then use the target in
conjunction with `target_link_libraries`. No other configuration is required.

### Example

```cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FetchBoost)

add_boost_component(program_options)
add_boost_component(system)

build_boost_components()

add_executable(fetch-boost-example)

target_sources(
  fetch-boost-example
  PRIVATE
  main.cc
)

target_link_libraries(
  fetch-boost-example
  PRIVATE
  boost_program_options_shared
  boost_system_shared
)
```
