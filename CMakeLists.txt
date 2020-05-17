# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.10)

if(${ESP_PLATFORM})
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Include Smooth as a component
set(EXTRA_COMPONENT_DIRS
         externals/smooth/smooth_component)

project(hello-world)
else()
    if(${APPLE})
        include_directories(SYSTEM /usr/local/include)
        link_directories(/usr/local/lib)
    endif()


    add_subdirectory(main)
    add_subdirectory(externals/smooth/lib)
    add_subdirectory(externals/smooth/mock-idf)
endif()