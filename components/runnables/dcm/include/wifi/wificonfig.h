#pragma once

#include <string>

namespace runnable {
namespace dcm {
namespace wifi {

static const uint32_t G_WIFI_CONN_TIMEOUT = 5000U;

static const std::string G_PARTITION_NAME = "app_storage";
static const std::string G_FILE_NAME = "credentials.jsn";
static const uint8_t G_MAX_NO_FILES = 10U;

static const std::string G_KEY_WIFI = "ssid";
static const std::string G_KEY_PWD = "pwd";
}  // namespace wifi
}  // namespace dcm
}  // namespace runnable