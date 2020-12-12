#pragma once

#include <string>

namespace runnable {
namespace dcm {
namespace wifi {

static const uint32_t G_WIFI_CONN_TIMEOUT = 5000U;

static const char* G_PARTITION_NAME = "app_storage";
static const char* G_FILE_NAME = "credentials.jsn";
static const int G_MAX_NO_FILES = 2U;

static const char* G_KEY_WIFI = "ssid";
static const char* G_KEY_PWD = "pwd";

static const uint16_t G_PROV_TIMEOUT_CYCLES = 600U;         // 5mins (300secs)
static const uint8_t G_PROV_FEEDBACK_TIMEOUT_CYCLES = 10U;  // 5secs
}  // namespace wifi
}  // namespace dcm
}  // namespace runnable