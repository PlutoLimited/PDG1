cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp App.cpp App.h)

file(TO_CMAKE_PATH "$ENV{IDF_PATH}" normalized_path)

    idf_component_register(SRCS ${SOURCES}
                            INCLUDE_DIRS
                                ${CMAKE_CURRENT_LIST_DIR}
                            REQUIRES
                                smooth_component
                                wifi_provisioning
                                esp_https_ota
                                tasks
                                runnables)

# Create and import the library targets
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../externals/tinymemport build)
# Publicly link `tiny_mempool_lib` to `main` component
target_link_libraries(${COMPONENT_LIB} PUBLIC tiny_memport)