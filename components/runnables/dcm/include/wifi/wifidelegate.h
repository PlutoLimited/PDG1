#pragma once

#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <fstream>

#include "runnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/filesystem/SPIFlash.h"
#include "smooth/core/json/JsonFile.h"
#include "smooth/core/network/Wifi.h"
#include "smooth/core/task_priorities.h"
#include "wificonfig.h"

using namespace smooth::core::filesystem;

namespace runnable {
namespace dcm {
namespace wifi {

static const std::string G_TAG("[RUN::DCM::WIFI]");

class CWifiDelegate {
 public:
  CWifiDelegate(smooth::core::network::Wifi*& f_wifi_p) : m_wifi_p(f_wifi_p){};

  void initWifi() {
    getCredentials();
    m_wifi_p->set_auto_connect(true);
  }

  void turnOffWifi() {}

  void connectAP() { m_wifi_p->connect_to_ap(); }

  bool isConnectedAP() { return m_wifi_p->is_connected_to_ap(); }

  bool isProvActive() { return m_wifi_p->is_provisioning_ble(); }

  bool isProvFinishedSuccess() { return m_wifi_p->is_should_save_creds(); }

  bool isProvFinishedFail() { return m_wifi_p->is_provisioning_failed(); }

  bool isConfigured() { return m_wifi_p->is_configured(); }

  void startBLProvisioning() { m_wifi_p->start_ble_provisioning(); }

  void saveCredentials() {
    SPIFlash flash{FlashMount::instance(), G_PARTITION_NAME, G_MAX_NO_FILES,
                   true};
    assert(flash.mount());

    // unlink(FlashMount::instance().mount_point() / G_FILE_NAME);
    smooth::core::json::JsonFile jf{FlashMount::instance().mount_point() /
                                    G_FILE_NAME};
    auto& v = jf.value();
    v[G_KEY_WIFI] = m_wifi_p->ssid;
    v[G_KEY_PWD] = m_wifi_p->password;
    assert(jf.save());

    m_wifi_p->setCredentialsSaved();
    flash.unmount();
  }

  void getCredentials() {
    SPIFlash flash{FlashMount::instance(), G_PARTITION_NAME, G_MAX_NO_FILES,
                   true};
    assert(flash.mount());

    smooth::core::json::JsonFile jf2{FlashMount::instance().mount_point() /
                                     G_FILE_NAME};
    auto& v2 = jf2.value();

    if (!v2[G_KEY_WIFI].is_null() && !v2[G_KEY_PWD].is_null()) {
      const auto ssid = v2[G_KEY_WIFI].get<std::string>();
      const auto pwd = v2[G_KEY_PWD].get<std::string>();
      Log::info(G_TAG, "Credentials are fetched and exist");
      // unlink(FlashMount::instance().mount_point() / G_FILE_NAME); DONT DELETE
      if (ssid.length() > 0U) {
        Log::info(G_TAG, "Credentials are fetched and valid");
        m_wifi_p->set_ap_credentials(ssid, pwd);
      }
    } else {
      Log::error(G_TAG, "Credentials null");
    }
    flash.unmount();
  }

 private:
  smooth::core::network::Wifi*& m_wifi_p;
};

}  // namespace wifi
}  // namespace dcm
}  // namespace runnable