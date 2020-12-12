
idf_component_register(SRCS "llc/src/llcRunnable.cpp" "ledc/src/ledcRunnable.cpp"  "tic/src/ticRunnable.cpp" "tic/src/hw/IQS263B.cpp" "dcm/src/dcmRunnable.cpp"
                    INCLUDE_DIRS "ledc/include" "llc/include" "tic/include" "dcm/include" "base/include"
                    ${CMAKE_CURRENT_LIST_DIR}/../../externals/tinymemport/include
                    REQUIRES
                    smooth_component
                    esp-aws-iot
                    esp_https_ota
                    )

target_add_binary_data(${COMPONENT_TARGET} "certs/aws-root-ca.pem" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/certificate.pem.crt" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/private.pem.key" TEXT)    
target_add_binary_data(${COMPONENT_TARGET} "certs/deviceid.txt" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/endpoint.txt" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/github_server.cert" TEXT)