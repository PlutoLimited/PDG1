#set(COMPONENT_SRCS "tasks/include/llc.h" "tasks/include/ledc.h" "tasks/include/tic.h" "runnable/include/llcRunnable.h")
#set(COMPONENT_ADD_INCLUDEDIRS "tasks/include")
#register_component()

idf_component_register(SRCS "include/llc.h" "include/ledc.h" "include/tic.h"
                  INCLUDE_DIRS "include"
                  REQUIRES runnables)