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

// ToDo don't use those namespaces per default
// using namespace smooth;
// using namespace smooth::core;
using namespace smooth::core::json;
using namespace smooth::core::timer;
using namespace smooth::core::ipc;
using namespace smooth::core::filesystem;

namespace runnable {
namespace dcm {
namespace wifi {

static const std::string G_HW_TAG("[RUN::DCM::DELEGATE]");

class CWifiDelegate {
 public:
  CWifiDelegate(smooth::core::network::Wifi*& f_wifi_p) : m_wifi_p(f_wifi_p){};

  void initWifi() { m_wifi.set_auto_connect(true); }

  void turnOffWifi() {}

  void connectAP() { m_wifi.connect_to_ap(); }

  bool isConnectedAP() { return m_wifi.is_connected_to_ap(); }

  bool isProvActive() { return m_wifi.is_provisioning_ble(); }

  bool isProvFinishedSuccess() { return m_wifi.is_should_save_creds(); }

  bool isProvFinishedFail() { return m_wifi.is_prov_fail(); }

  bool isConfigured() { return m_wifi.is_configured(); }

  void startBLProvisioning() { m_wifi.start_ble_provisioning(); }

  void saveCredentials() {
    SPIFlash flash{FlashMount::instance(), "app_storage", 10, true};
    assert(flash.mount());

    unlink(FlashMount::instance().mount_point() / "credentials.jsn");
    JsonFile jf{FlashMount::instance().mount_point() / "credentials.jsn"};
    auto& v = jf.value();
    v["ssid"] = m_wifi.ssid;
    v["pwd"] = m_wifi.password;
    assert(jf.save());

    m_wifi.setCredentialsSaved();
    flash.unmount();
  }

  void getCredentials() {
    SPIFlash flash{FlashMount::instance(), "app_storage", 10, true};
    assert(flash.mount());

    JsonFile jf2{FlashMount::instance().mount_point() / "credentials.jsn"};
    auto& v2 = jf2.value();
    const auto ssid = v2["ssid"].get<std::string>();
    const auto pwd = v2["pwd"].get<std::string>();
    unlink(FlashMount::instance().mount_point() / "credentials.json");
    if (ssid.length() > 0U) {
      m_wifi.set_ap_credentials(ssid, pwd);
    }
    flash.unmount();
  }

 private:
  smooth::core::network::Wifi*& m_wifi_p;
  smooth::core::network::Wifi m_wifi;
};

}  // namespace wifi
}  // namespace dcm
}  // namespace runnable