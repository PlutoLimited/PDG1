#pragma once

#include <esp_log.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <fstream>

#include "config/ota_config.h"
#include "esp_https_ota.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/network/Wifi.h"
#include "smooth/core/task_priorities.h"

namespace runnable {
namespace dcm {
namespace ota {

static const std::string G_TAG("[RUN::DCM::OTA]");

class COTADelegate {
 public:
  COTADelegate(){};

  esp_err_t do_firmware_upgrade(const char *url) {
    if (!url) {
      return ESP_FAIL;
    }
    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = (char *)upgrade_server_cert_pem_start,
    };
    return esp_https_ota(&config);
  }

 private:
};

}  // namespace ota
}  // namespace dcm
}  // namespace runnable