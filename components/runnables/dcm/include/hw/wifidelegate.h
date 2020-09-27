#pragma once

#include "hwconfig.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/network/Wifi.h"

namespace runnable {
namespace dcm {
namespace hw {

static const std::string G_HW_TAG("[RUN::DCM::HW]");

class CWifiDelegate {
 public:
  CWifiDelegate() : m_wifi(){};

  void initWifi() {}
  void turnOffWifi() {}
  void connectWifi() {}

  void startBLProvisioning() {}

  void saveCredentials() {}

 private:
  smooth::core::network::Wifi m_wifi;
};

}  // namespace hw
}  // namespace dcm
}  // namespace runnable