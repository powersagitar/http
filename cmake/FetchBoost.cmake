include(FetchContent)

FetchContent_Declare(
    boost
    URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.bz2
)

FetchContent_MakeAvailable(boost)

set(BOOST_SRC_DIR ${FETCHCONTENT_BASE_DIR}/boost-src)
set(BOOST_INCLUDE_DIR ${BOOST_SRC_DIR})
set(BOOST_BUILD_DIR ${FETCHCONTENT_BASE_DIR}/boost-build)
set(BOOST_LIB_DIR ${BOOST_BUILD_DIR}/lib)
set(BOOST_B2_DIR ${BOOST_SRC_DIR})
set(BOOST_B2_PATH ${BOOST_B2_DIR}/b2)

add_custom_command(
    OUTPUT ${BOOST_B2_PATH}
    COMMAND ./bootstrap.sh --prefix=${BOOST_BUILD_DIR}
    WORKING_DIRECTORY ${BOOST_SRC_DIR}
)

set(BOOST_COMPONENTS "")

function(add_boost_component component)
  message("Adding Boost component: ${component}")

  list(APPEND BOOST_COMPONENTS ${component})
  set(BOOST_COMPONENTS ${BOOST_COMPONENTS} PARENT_SCOPE)
endfunction()

function(build_boost_components)
  message("Building Boost components: ${BOOST_COMPONENTS}")

  set(boost_b2_args ${BOOST_COMPONENTS})
  list(TRANSFORM boost_b2_args PREPEND "--with-")
  list(JOIN boost_b2_args " " boost_b2_args)

  set(boost_shared_libs ${BOOST_COMPONENTS})
  list(TRANSFORM boost_shared_libs PREPEND "${BOOST_LIB_DIR}/libboost_")
  list(TRANSFORM boost_shared_libs APPEND "${CMAKE_SHARED_LIBRARY_SUFFIX}")
  list(JOIN boost_shared_libs " " boost_shared_libs)

  add_custom_command(
      DEPENDS ${BOOST_B2_PATH}
      OUTPUT ${boost_shared_libs}
      COMMAND ./b2 install ${boost_b2_args}
      WORKING_DIRECTORY ${BOOST_B2_DIR}
      COMMENT "In ${BOOST_B2_DIR}, running ./b2 install ${boost_b2_args}"
  )

  foreach(component IN ITEMS ${BOOST_COMPONENTS})
    message("Exporting Boost component: ${component}")

    set(lib_name ${CMAKE_SHARED_LIBRARY_PREFIX})
    set(lib_name ${lib_name}boost_${component})
    set(lib_name ${lib_name}${CMAKE_SHARED_LIBRARY_SUFFIX})

    set(lib_path ${BOOST_LIB_DIR}/${lib_name})

    set(lib_export_name boost_${component}_shared)
    set(lib_target ${lib_export_name}_target)

    add_custom_target(
        ${lib_target}
        DEPENDS ${lib_path}
    )

    add_library(
        ${lib_export_name}
        SHARED IMPORTED GLOBAL
    )

    add_dependencies(
        ${lib_export_name}
        ${lib_target}
    )

    set_target_properties(
        ${lib_export_name}
        PROPERTIES
        IMPORTED_LOCATION ${lib_path}
        INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR}
    )

    message("Exported Boost ${component} as ${lib_export_name}")
  endforeach()
endfunction()

