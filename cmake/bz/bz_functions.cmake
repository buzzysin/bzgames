# Turns a bz_xxx_yyy string into a bz::xxx::yyy string
# if and only if the string starts with bz_
function(bz_alias_name out target)
    set(options)
    set(oneValueArgs PARENT)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(bz_alias_name "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT target MATCHES "^bz_")
        set(${out} ${target} PARENT_SCOPE)
        # message(STATUS "[bz_alia]: Converted ${target} to ${target}")
        return()
    endif()

    string(SUBSTRING ${target} 3 -1 target)
    string(REPLACE "_" "::" target ${target})

    if (NOT bz_alias_name_PARENT)
        set(${out} bz::${target} PARENT_SCOPE)
        # message(STATUS "[bz_alia]: Created alias ${target} => bz::${target}") 
    else()
        set(${out} bz::${bz_alias_name_PARENT}::${target} PARENT_SCOPE)
        # message(STATUS "[bz_alia]: Created alias ${target} => bz::${bz_alias_name_PARENT}::${target}") 
    endif()
        
endfunction()

# Add dependencies to a defined target
function(bz_deps target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(target_name bz_${target})
    bz_alias_name(target_alias ${target_name} PARENT "${bz_target_PARENT}")

    message(STATUS "[bz_deps]: ${target} ->  Dependencies - [${bz_target_DEPENDS}]")    
    target_link_libraries(${target_name} ${BZ_SCOPE} ${bz_target_DEPENDS})
endfunction()

# Common target pre-setup
function (bz_pre target)
    set(options)
    set(oneValueArgs PARENT)
    set(multiValueArgs DEPENDS COMPILE_ARGS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(target_name bz_${target})
    bz_alias_name(target_alias ${target_name} PARENT "${bz_target_PARENT}")
endfunction()

# Common target post-setup
function (bz_post target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs DEPENDS COMPILE_ARGS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Set scope based on target type
    if (BZ_TYPE STREQUAL "intf")
        set(BZ_SCOPE "INTERFACE")
        set(BZ_INCLUDE_SCOPE "INTERFACE")
    else()
        set(BZ_SCOPE "PRIVATE")
        set(BZ_INCLUDE_SCOPE "PUBLIC")
    endif()

    # Add the include directories
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_include_directories(${target_name} ${BZ_INCLUDE_SCOPE} ${CMAKE_CURRENT_SOURCE_DIR}/include)
    endif()

    # Add the dependencies
    bz_deps(${target} DEPENDS ${bz_target_DEPENDS})
    
    # Add the compile arguments
    if (bz_target_COMPILE_ARGS)
        target_compile_options(${target_name} ${BZ_SCOPE} ${bz_target_COMPILE_ARGS})
    endif()
endfunction()

# Add an executable target to the build
function(bz_app target)
    set(options)
    set(oneValueArgs PARENT)
    set(multiValueArgs SOURCES DEPENDS COMPILE_ARGS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # If there are no sources, assume the target is header-only
    if (NOT bz_target_SOURCES)
        bz_intf(${target} DEPENDS ${bz_target_DEPENDS} PARENT ${bz_target_PARENT} COMPILE_ARGS ${bz_target_COMPILE_ARGS})
        return()
    endif()
        
    set(BZ_TYPE "app")
    set(target_name bz_${target})
    bz_alias_name(target_alias ${target_name} PARENT "${bz_target_PARENT}")

    # Create the executable target
    add_executable(${target_name} ${bz_target_SOURCES})

    # Alias the target to make it easier to reference
    add_executable(${target_alias} ALIAS ${target_name})
    
    bz_post(${target} DEPENDS ${bz_target_DEPENDS} COMPILE_ARGS ${bz_target_COMPILE_ARGS})
endfunction()

# Add a library target to the build
function(bz_lib target)
    set(options)
    set(oneValueArgs PARENT LIB_TYPE)
    set(multiValueArgs SOURCES DEPENDS COMPILE_ARGS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT bz_target_LIB_TYPE)
        set(bz_target_LIB_TYPE "SHARED")
    endif()

    # If there are no sources, assume the library is header-only
    if (NOT bz_target_SOURCES)
        bz_intf(${target} DEPENDS ${bz_target_DEPENDS} PARENT ${bz_target_PARENT} COMPILE_ARGS ${bz_target_COMPILE_ARGS})
        return()
    endif()

    set(BZ_TYPE "lib")
    set(target_name bz_${target})
    bz_alias_name(target_alias ${target_name} PARENT "${bz_target_PARENT}")

    # Create the library target
    add_library(${target_name} ${bz_target_LIB_TYPE} ${bz_target_SOURCES})
    
    # Alias the target to make it easier to reference
    add_library(${target_alias} ALIAS ${target_name})
    
    bz_post(${target} DEPENDS ${bz_target_DEPENDS} COMPILE_ARGS ${bz_target_COMPILE_ARGS})
endfunction()

# Add a test target (alias for executable, but conditional on BZ_BUILD_TESTS being set)
function(bz_test test)
    if (NOT BZ_BUILD_TESTS)
        return()
    endif()

    bz_app(${test} ${ARGN})
endfunction()

# Add an interface target to the build
function(bz_intf target)
    set(options)
    set(oneValueArgs PARENT)
    set(multiValueArgs SOURCES DEPENDS COMPILE_ARGS)
    cmake_parse_arguments(bz_target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(BZ_TYPE "intf")
    set(target_name bz_${target})
    bz_alias_name(target_alias ${target_name} PARENT "${bz_target_PARENT}")

    # Create the interface target
    message(STATUS "[bz_intf]: ${target} -> Creating interface target ${target_name}")
    add_library(${target_name} INTERFACE)

    # Alias the target to make it easier to reference
    message(STATUS "[bz_intf]: ${target} -> Alias ${target_name} to ${target_alias}")
    add_library(${target_alias} ALIAS ${target_name})
    
    bz_post(${target} DEPENDS ${bz_target_DEPENDS} COMPILE_ARGS ${bz_target_COMPILE_ARGS})
endfunction()
