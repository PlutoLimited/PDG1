
idf_component_register(SRCS "llc/src/llcRunnable.cpp" "ledc/src/ledcRunnable.cpp"  "tic/src/ticRunnable.cpp"
                    INCLUDE_DIRS "ledc/include" "llc/include" "tic/include" "base/include"
                    ${CMAKE_CURRENT_LIST_DIR}/../../externals/tinymemport/include
                    REQUIRES
                    smooth_component
                      )
