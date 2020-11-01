#pragma once

#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <fstream>

#include "hwconfig.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/filesystem/SPIFlash.h"
#include "smooth/core/json/JsonFile.h"
#include "smooth/core/network/Wifi.h"
#include "smooth/core/task_priorities.h"

using namespace smooth;
using namespace smooth::core;
using namespace smooth::core::json;
using namespace smooth::core::timer;
using namespace smooth::core::ipc;
using namespace smooth::core::filesystem;
using namespace smooth::core::logging;
using namespace std::chrono;

namespace runnable {
namespace dcm {
namespace hw {

static const std::string G_HW_TAG("[RUN::DCM::HW]");

class CWifiDelegate {
 public:
  CWifiDelegate() : m_wifi() { initWifi(); };

  void initWifi() {}
  void turnOffWifi() {}
  void connectAP() {}

  bool isConnectedAP() { return m_wifi.is_connected_to_ap(); }

  bool isProvActive() { return m_wifi.is_provisioning_ble(); }
  bool isProvFinishedSuccess() { return m_wifi.is_should_save_creds(); }

  bool isConfigured() { return m_wifi.is_configured(); }

  void startBLProvisioning() { m_wifi.start_ble_provisioning(); }

  void saveCredentials() {
    SPIFlash flash{FlashMount::instance(), "app_storage", 10, true};
    assert(flash.mount());

    unlink(FlashMount::instance().mount_point() / "file.jsn");
    JsonFile jf{FlashMount::instance().mount_point() / "file.jsn"};
    auto& v = jf.value();
    // assert(v["Foo"].is_null());
    v["ssid"] = m_wifi.ssid;
    v["pwd"] = m_wifi.password;
    assert(jf.save());

    JsonFile jf2{FlashMount::instance().mount_point() / "file.jsn"};
    auto& v2 = jf2.value();
    assert(v2["ssid"].get<std::string>() == m_wifi.ssid);
    assert(v2["pwd"].get<std::string>() == m_wifi.password);
    unlink(FlashMount::instance().mount_point() / "file.json");

    m_wifi.setCredentialsSaved();
    flash.unmount();
  }

 private:
  smooth::core::network::Wifi m_wifi;
};

}  // namespace hw
}  // namespace dcm
}  // namespace runnable