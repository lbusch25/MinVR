# Author: Daniel F. Keefe
# Copyright 2017,2018, Daniel F. Keefe and Regents of the University of Minnesota
# All Rights Reserved.


# Usage:
# AutoBuild_use_package_Vulkan(
#    # The 1st argument is required.  It is the name of the target you wish to link this dependency to.
#    my-program 
#
#    # The 2nd argument is required.  It impacts how dependencies are propogated through CMake.  You can treat
#    # this dependency as an INTERFACE, PUBLIC, or PRIVATE.  See the following URL for details:
#    # https://cmake.org/cmake/help/latest/command/target_include_directories.html
#    PUBLIC 
# )
#
macro(AutoBuild_use_package_Vulkan YOUR_TARGET INTERFACE_PUBLIC_OR_PRIVATE)

    message(STATUS "AutoBuild: Searching for package Vulkan...")

	# Check to see if the library is already installed on the system    
    # CMake ships with FindVulkan.cmake and in CMake 3.9+ it defines
    # the imported target Vulkan::Vulkan.  Using this is
    # now the preferred way to link with OpenGL and all of its dependencies.
	# See https://cmake.org/cmake/help/v3.7/module/FindVulkan.html
	find_package(Vulkan)

	if (NOT ${Vulkan_FOUND})
	    message(FATAL_ERROR "AutoBuild: Vulkan was not found on the system.  AutoBuild can auto-download 
        and build many dependencies for you, but not Vulkan. It should come pre-installed on your system.")
	endif()

    message(STATUS "AutoBuild: Ok. Found Vulkan.")
    message(STATUS "  Vulkan headers: ${Vulkan_INCLUDE_DIR}")
    message(STATUS "  Vulkan libs: ${Vulkan_LIBRARIES}")

    message(STATUS "Linking target ${YOUR_TARGET} with ${INTERFACE_PUBLIC_OR_PRIVATE} dependency Vulkan::Vulkan.")
    target_link_libraries(${YOUR_TARGET} ${INTERFACE_PUBLIC_OR_PRIVATE} Vulkan::Vulkan)

	#Not sure what this does ask Bret
	target_compile_definitions(${YOUR_TARGET} ${INTERFACE_PUBLIC_OR_PRIVATE} -DUSE_OPENGL)

endmacro()