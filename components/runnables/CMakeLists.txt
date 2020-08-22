
idf_component_register(SRCS "llc/src/llcRunnable.cpp" "ledc/src/ledcRunnable.cpp"  "tic/src/ticRunnable.cpp" "tic/src/hw/IQS263B.cpp"
                    INCLUDE_DIRS "ledc/include" "llc/include" "tic/include" "base/include"
                    ${CMAKE_CURRENT_LIST_DIR}/../../externals/tinymemport/include
                    REQUIRES
                    smooth_component
                      )
