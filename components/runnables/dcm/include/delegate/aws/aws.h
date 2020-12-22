#pragma once

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <fstream>

#include "aws_iot_config.h"
#include "aws_iot_error.h"
#include "aws_iot_log.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_shadow_interface.h"
#include "aws_iot_shadow_json_data.h"
#include "aws_iot_version.h"
#include "aws_utils.h"
#include "config/aws_config.h"
#include "delegate/ota/otadelegate.h"
#include "esp_system.h"
#include "smooth/core/Application.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

namespace runnable {
namespace dcm {
namespace aws {
/*
 * The Json Document in the cloud will be:
 * {
 *   "reported": {
 *      "ota_url": "",
 *      "output": true,
 *      "fw_version": "<version-string>"
 *    },
 *   "desired": {
 *      "ota_url": "",
 *      "output": false
 *   }
 * }
 */

/* Per-Device Unique components:
 * - Device ID
 * - Certificate
 * - Private Key
 */

static const std::string G_TASK_TAG("[RUN::DCM::DEL::AWS]");

class CAWSDelegate {
 public:
  CAWSDelegate() {}

  static void output_state_change_callback(const char *pJsonString,
                                           uint32_t JsonStringDataLen,
                                           jsonStruct_t *pContext) {
    if (pContext != NULL) {
      bool state = *(bool *)(pContext->pData);
      ESP_LOGI(TAG, "Delta - Output state changed to %s",
               state ? "true" : "false");
      //  app_driver_set_state(state);
      // output_changed_locally = false;
    }
  }

  static void ota_url_state_change_callback(const char *pJsonString,
                                            uint32_t JsonStringDataLen,
                                            jsonStruct_t *pContext) {
    if (pContext != NULL) {
      ESP_LOGI(TAG, "Delta - OTA URL received: %s", (char *)pContext->pData);
      if (m_otaDelegate.do_firmware_upgrade((char *)pContext->pData) ==
          ESP_OK) {
        // Firmware upgrade successful
        // ota_update_done = true;
      }
    }
  }

  static void update_status_callback(const char *pThingName,
                                     ShadowActions_t action,
                                     Shadow_Ack_Status_t status,
                                     const char *pReceivedJsonDocument,
                                     void *pContextData) {
    IOT_UNUSED(pThingName);
    IOT_UNUSED(action);
    IOT_UNUSED(pReceivedJsonDocument);
    IOT_UNUSED(pContextData);

    // shadowUpdateInProgress = false;

    if (SHADOW_ACK_TIMEOUT == status) {
      ESP_LOGE(TAG, "Update timed out");
    } else if (SHADOW_ACK_REJECTED == status) {
      ESP_LOGE(TAG, "Update rejected");
    } else if (SHADOW_ACK_ACCEPTED == status) {
      // shadow doc OTA URL reset successful
      // if (ota_update_done) {
      esp_restart();
      //}
      ESP_LOGI(TAG, "Update accepted");
    }
  }

  void initAWS() {}

  void disconnect() {
    /* if (SUCCESS != rc) {
       Log::info(G_TASK_TAG, "Disconnecting AWS");
     }
     if (reported_handles) {
       free(reported_handles);
     }
     if (desired_handles) {
       free(desired_handles);
     }

     rc = aws_iot_shadow_disconnect(&mqttClient);

     if (SUCCESS != rc) {
       Log::error(TAG, "Disconnect error %d", rc);
     }*/
  }
  void initMqtt() {}

  void initShadows() {
    IoT_Error_t rc = FAILURE;

    AWS_IoT_Client mqttClient;

    ShadowInitParameters_t sp = ShadowInitParametersDefault;
    sp.pHost = (char *)endpoint_txt_start;
    sp.port = AWS_IOT_MQTT_PORT;
    sp.pClientCRT = (const char *)certificate_pem_crt_start;
    sp.pClientKey = (const char *)private_pem_key_start;
    sp.pRootCA = (const char *)aws_root_ca_pem_start;
    sp.enableAutoReconnect = false;
    sp.disconnectHandler = NULL;

    Log::info(G_TASK_TAG, "Shadow Init");
    rc = aws_iot_shadow_init(&mqttClient, &sp);

    if (SUCCESS != rc) {
      Log::error(G_TASK_TAG, "aws_iot_shadow_init returned error {}", rc);
      return;
    }

    ShadowConnectParameters_t scp = ShadowConnectParametersDefault;
    scp.pMyThingName = (const char *)deviceid_txt_start;
    scp.pMqttClientId = (const char *)deviceid_txt_start;
    scp.mqttClientIdLen = (uint16_t)strlen((const char *)deviceid_txt_start);

    Log::info(G_TASK_TAG, "Connecting to AWS...");

    rc = aws_iot_shadow_connect(&mqttClient, &scp);
    if (SUCCESS != rc) {
      Log::error(G_TASK_TAG, "Error: {} connecting to {}:{}", rc, sp.pHost,
                 sp.port);
      return;
    }

    rc = aws_iot_shadow_set_autoreconnect_status(&mqttClient, true);
    if (SUCCESS != rc) {
      Log::error(G_TASK_TAG, "Unable to set Auto Reconnect to true - {}", rc);
      return;
    }
  }

  void runAWS() {
    /*if (NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc ||
        SUCCESS == rc) {
      rc = aws_iot_shadow_yield(&mqttClient, 200);
      if (NETWORK_ATTEMPTING_RECONNECT == rc || shadowUpdateInProgress) {
        rc = aws_iot_shadow_yield(&mqttClient, 1000);
        // If the client is attempting to reconnect, or already waiting on a
        // shadow update, we will skip the rest of the loop.
        return;
      }
      desired_count = 0;
      reported_count = 0;

      if (ota_update_done) {
        // OTA update was successful
        // Reset OTA URL
        strcpy(ota_url, "");
        reported_handles[reported_count++] = &ota_handler;
        desired_handles[desired_count++] = &ota_handler;
      }

      // output_state = app_driver_get_state();
      if (reported_state != output_state) {
        reported_handles[reported_count++] = &output_handler;
        if (output_changed_locally == true) {
          desired_handles[desired_count++] = &output_handler;
        }
        output_changed_locally = true;
        reported_state = output_state;
      }

      if (reported_count > 0 || desired_count > 0) {
        rc = shadow_update(&mqttClient, reported_handles, reported_count,
                           desired_handles, desired_count);
      }
    }*/
  }

  void registerDeviceOutputShadow() {
    // output_state = app_driver_get_state();
    /*
    jsonStruct_t output_handler;
    output_handler.cb = output_state_change_callback;
    output_handler.pData = &output_state;
    output_handler.dataLength = sizeof(output_state);
    output_handler.pKey = "output";
    output_handler.type = SHADOW_JSON_BOOL;
    rc = aws_iot_shadow_register_delta(&mqttClient, &output_handler);
    if (SUCCESS != rc) {
      Log::error(TAG, "Shadow Register State Delta Error %d", rc);
      // goto aws_error;
      return;
    }*/
  }

  /* static void output_state_change_callback(const char *pJsonString,
                                            uint32_t JsonStringDataLen,
                                            jsonStruct_t *pContext) {
     if (pContext != NULL) {
       bool state = *(bool *)(pContext->pData);
       ESP_LOGI(TAG, "Delta - Output state changed to %s",
                state ? "true" : "false");
       // app_driver_set_state(state);
       output_changed_locally = false;
     }
   }
 */
  /* static void ota_url_state_change_callback(const char *pJsonString,
                                             uint32_t JsonStringDataLen,
                                             jsonStruct_t *pContext) {
     if (pContext != NULL) {
       ESP_LOGI(TAG, "Delta - OTA URL received: %s", (char *)pContext->pData);
       // if (do_firmware_upgrade((char *)pContext->pData) == ESP_OK) {
       // Firmware upgrade successful
       // ota_update_done = true;
       //}
     }
   }

   static void update_status_callback(const char *pThingName,
                                      ShadowActions_t action,
                                      Shadow_Ack_Status_t status,
                                      const char *pReceivedJsonDocument,
                                      void *pContextData) {
     IOT_UNUSED(pThingName);
     IOT_UNUSED(action);
     IOT_UNUSED(pReceivedJsonDocument);
     IOT_UNUSED(pContextData);

     shadowUpdateInProgress = false;

     if (SHADOW_ACK_TIMEOUT == status) {
       ESP_LOGE(TAG, "Update timed out");
     } else if (SHADOW_ACK_REJECTED == status) {
       ESP_LOGE(TAG, "Update rejected");
     } else if (SHADOW_ACK_ACCEPTED == status) {
       // shadow doc OTA URL reset successful
       if (ota_update_done) {
         esp_restart();
       }
       ESP_LOGI(TAG, "Update accepted");
     }
   }
 */
  void registerOtaHandlerShadow() {
    /* jsonStruct_t ota_handler;
     char ota_url[MAX_LENGTH_URL];
     strcpy(ota_url, "");
     ota_handler.cb = ota_url_state_change_callback;
     ota_handler.pData = &ota_url;
     ota_handler.pKey = "ota_url";
     ota_handler.dataLength = sizeof(ota_url);
     ota_handler.type = SHADOW_JSON_STRING;
     rc = aws_iot_shadow_register_delta(&mqttClient, &ota_handler);
     if (SUCCESS != rc) {
       ESP_LOGE(TAG, "Shadow Register OTA Delta Error");
       // goto aws_error;
     }

     jsonStruct_t fw_handler;
     // fw_handler.pData = FW_VERSION;
     // fw_handler.dataLength = sizeof(FW_VERSION);
     fw_handler.pKey = "fw_version";
     fw_handler.type = SHADOW_JSON_STRING;

     jsonStruct_t **desired_handles =
         (jsonStruct_t **)malloc(MAX_DESIRED_PARAM * sizeof(jsonStruct_t *));
     if (desired_handles == NULL) {
       ESP_LOGE(TAG, "Failed to allocate memory");
       // goto aws_error;
     }

     jsonStruct_t **reported_handles =
         (jsonStruct_t **)malloc(MAX_REPORTED_PARAM * sizeof(jsonStruct_t *));
     if (reported_handles == NULL) {
       ESP_LOGE(TAG, "Failed to allocate memory");
       free(desired_handles);
       // goto aws_error;
     }*/
  }

  void updateShadows() {
    /*  // Report the initial values once
      size_t desired_count = 0, reported_count = 0;
      reported_handles[reported_count++] = &fw_handler;
      reported_handles[reported_count++] = &output_handler;
      rc = shadow_update(&mqttClient, reported_handles, reported_count,
                         desired_handles, desired_count);
      reported_state = output_state;*/
  }

 private:
  bool shadowUpdateInProgress;
  int reported_state = false;
  bool output_changed_locally = false;
  bool ota_update_done = false;
  static ota::COTADelegate m_otaDelegate;
};

}  // namespace aws
}  // namespace dcm
}  // namespace runnable