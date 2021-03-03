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

static const std::string G_TASK_TAG("[DCM::DEL::AWS]");

class CAWSDelegate {
 public:
  CAWSDelegate() : rc(FAILURE) {}

  static void output_state_change_callback(const char *pJsonString,
                                           uint32_t JsonStringDataLen,
                                           jsonStruct_t *pContext) {
    if (pContext != NULL) {
      desired_light_state = *(uint8_t *)(pContext->pData);
      ESP_LOGI(TAG, "Delta - Light state changed to %d", desired_light_state);
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
        ota_update_done = true;
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

    shadowUpdateInProgress = false;

    if (SHADOW_ACK_TIMEOUT == status) {
      ESP_LOGE(TAG, "Update timed out");
    } else if (SHADOW_ACK_REJECTED == status) {
      ESP_LOGE(TAG, "Update rejected");
    } else if (SHADOW_ACK_ACCEPTED == status) {
      // shadow doc OTA URL reset successful
      ESP_LOGI(TAG, "Update accepted");
      if (ota_update_done) {
        esp_restart();
      }
    }
  }

  static IoT_Error_t shadow_update(AWS_IoT_Client *mqttClient,
                                   jsonStruct_t **reported_handles,
                                   size_t reported_count,
                                   jsonStruct_t **desired_handles,
                                   size_t desired_count) {
    IoT_Error_t rc = FAILURE;
    char JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
    size_t sizeOfJsonDocumentBuffer =
        sizeof(JsonDocumentBuffer) / sizeof(JsonDocumentBuffer[0]);
    rc = aws_iot_shadow_init_json_document(JsonDocumentBuffer,
                                           sizeOfJsonDocumentBuffer);
    if (rc != SUCCESS) {
      return rc;
    }

    if (reported_count > 0) {
      rc = custom_aws_iot_shadow_add_reported(JsonDocumentBuffer,
                                              sizeOfJsonDocumentBuffer,
                                              reported_count, reported_handles);
      if (rc != SUCCESS) {
        return rc;
      }
    }

    if (desired_count > 0) {
      rc = custom_aws_iot_shadow_add_desired(JsonDocumentBuffer,
                                             sizeOfJsonDocumentBuffer,
                                             desired_count, desired_handles);
      if (rc != SUCCESS) {
        return rc;
      }
    }

    rc = aws_iot_finalize_json_document(JsonDocumentBuffer,
                                        sizeOfJsonDocumentBuffer);
    if (rc != SUCCESS) {
      return rc;
    }
    ESP_LOGI(TAG, "Update Shadow: %s", JsonDocumentBuffer);
    rc = aws_iot_shadow_update(mqttClient, (const char *)deviceid_txt_start,
                               JsonDocumentBuffer, update_status_callback, NULL,
                               4, true);
    if (rc != SUCCESS) {
      return rc;
    }

    shadowUpdateInProgress = true;
    return rc;
  }

  void setupShadowHandles() {
    ota_handler.cb = ota_url_state_change_callback;
    ota_handler.pData = &ota_url;
    ota_handler.pKey = "ota_url";
    ota_handler.dataLength = sizeof(ota_url);
    ota_handler.type = SHADOW_JSON_STRING;

    output_handler.cb = output_state_change_callback;
    output_handler.pData = &light_state;
    output_handler.dataLength = sizeof(light_state);
    output_handler.pKey = "light_level";
    output_handler.type = SHADOW_JSON_UINT8;

    char fw[sizeof(FW_VERSION)];
    strcpy(fw, FW_VERSION);
    fw_handler.pData = &fw;
    fw_handler.dataLength = sizeof(fw);
    fw_handler.pKey = "fw_version";
    fw_handler.type = SHADOW_JSON_STRING;
  }

  bool initAWS() {
    // setupShadowHandles();
    rc = FAILURE;
    ota_update_done = false;

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
      return false;
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
      return false;
    }

    rc = aws_iot_shadow_set_autoreconnect_status(&mqttClient, true);
    if (SUCCESS != rc) {
      Log::error(G_TASK_TAG, "Unable to set Auto Reconnect to true - {}", rc);
      return false;
    }

    ota_handler.cb = ota_url_state_change_callback;
    ota_handler.pData = &ota_url;
    ota_handler.pKey = "ota_url";
    ota_handler.dataLength = sizeof(ota_url);
    ota_handler.type = SHADOW_JSON_STRING;
    rc = aws_iot_shadow_register_delta(&mqttClient, &ota_handler);
    if (SUCCESS != rc) {
      ESP_LOGE(TAG, "Shadow Register OTA Delta Error");
      return false;
    }
    output_handler.cb = output_state_change_callback;
    output_handler.pData = &light_state;
    output_handler.dataLength = sizeof(light_state);
    output_handler.pKey = "light_level";
    output_handler.type = SHADOW_JSON_UINT8;

    rc = aws_iot_shadow_register_delta(&mqttClient, &output_handler);
    if (SUCCESS != rc) {
      ESP_LOGE(TAG, "Shadow Register State Delta Error %d", rc);
      return false;
    }

    char fw[sizeof(FW_VERSION)];
    strcpy(fw, FW_VERSION);
    fw_handler.pData = &fw;
    fw_handler.dataLength = sizeof(fw);
    fw_handler.pKey = "fw_version";
    fw_handler.type = SHADOW_JSON_STRING;

    strcpy(ota_url, "");
    desired_handles = reinterpret_cast<jsonStruct_t **>(
        malloc(MAX_DESIRED_PARAM * sizeof(jsonStruct_t *)));
    if (desired_handles == NULL) {
      ESP_LOGE(TAG, "Failed to allocate memory");
      return false;
    }

    reported_handles = reinterpret_cast<jsonStruct_t **>(
        malloc(MAX_REPORTED_PARAM * sizeof(jsonStruct_t *)));

    if (reported_handles == NULL) {
      ESP_LOGE(TAG, "Failed to allocate memory");
      free(desired_handles);
      return false;
    }
    desired_count = 0;
    reported_count = 0;
    // Report the initial values once
    reported_handles[reported_count++] = &fw_handler;
    reported_handles[reported_count++] = &output_handler;

    rc = shadow_update(&mqttClient, reported_handles, reported_count,
                       desired_handles, desired_count);

    if (SUCCESS != rc) {
      ESP_LOGE(TAG, "Shadow Initial Update Error %d", rc);
      return false;
    }

    reported_lightstate = light_state;
    return true;
  }

  void runAWS() {
    desired_count = 0;
    reported_count = 0;

    if (NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc ||
        SUCCESS == rc) {
      rc = aws_iot_shadow_yield(&mqttClient, 200);
      if (NETWORK_ATTEMPTING_RECONNECT == rc || shadowUpdateInProgress) {
        rc = aws_iot_shadow_yield(&mqttClient, 1000);
        // If the client is attempting to reconnect, or already waiting on a
        // shadow update, we will skip the rest of the loop.
        return;
      }

      if (ota_update_done) {
        // OTA update was successful
        // Reset OTA URL
        strcpy(ota_url, "");
        reported_handles[reported_count++] = &ota_handler;
        desired_handles[desired_count++] = &ota_handler;
      }

      if (reported_lightstate != light_state) {
        reported_handles[reported_count++] = &output_handler;
        desired_handles[desired_count++] = &output_handler;
        reported_lightstate = light_state;
        desired_light_state = light_state;
      }

      if (reported_count > 0 || desired_count > 0) {
        rc = shadow_update(&mqttClient, reported_handles, reported_count,
                           desired_handles, desired_count);
      }
    }
  }

  void updateLightLevel(uint8_t f_lightlvl) { light_state = f_lightlvl; }

  uint8_t getDesiredLightLevel() { return desired_light_state; }

 private:
  static bool shadowUpdateInProgress;
  static bool ota_update_done;

  char ota_url[MAX_LENGTH_URL];
  static ota::COTADelegate m_otaDelegate;
  jsonStruct_t **desired_handles;
  jsonStruct_t **reported_handles;
  jsonStruct_t ota_handler;
  jsonStruct_t output_handler;
  jsonStruct_t fw_handler;

  IoT_Error_t rc;
  AWS_IoT_Client mqttClient;
  static uint8_t light_state;
  static uint8_t desired_light_state;
  uint8_t reported_lightstate = 0;
  size_t desired_count = 0, reported_count = 0;
};

}  // namespace aws
}  // namespace dcm
}  // namespace runnable