#pragma once

#include "input/dcm_input.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "wifi/wifidelegate.h"

namespace runnable {
namespace dcm {
namespace handler {

static const std::string G_TASK_TAG("[RUN::DCM::HANDLER]");

class CWifiHandler {
 public:
  CWifiHandler(runnable::dcm::input::CDCMInput& f_inputData,
               runnable::dcm::output::CDCMOutput& f_outputData,
               smooth::core::network::Wifi*& f_wifi_p_r)
      : m_input(f_inputData),
        m_output(f_outputData),
        m_wifiDelegate(f_wifi_p_r) {
    m_wifiDelegate.getCredentials();
    m_wifiDelegate.initWifi();
  }

  void handle() {
    m_output.m_funcState = dcm::output::EDCMState::ACTIVE;

    // prov process active, do nothing
    if (m_wifiDelegate.isProvActive()) {
      m_output.m_connState = dcm::output::EConnectionState::PROV_BLE_ACTIVE;
      return;
    }

    if (m_wifiDelegate.isProvFinishedSuccess()) {
      m_wifiDelegate.saveCredentials();
      m_output.m_connState = dcm::output::EConnectionState::PROV_BLE_SUCCESS;
      return;
    }

    if (m_wifiDelegate.isProvFinishedFail()) {
      m_output.m_connState = dcm::output::EConnectionState::PROV_BLE_FAIL;
      return;
    }

    // on long touch and is not setup --> setup
    if (m_input.m_ticData.m_funcState == tic::ETICState::ACTIVE &&
        m_input.m_ticData.m_deviceState ==
            tic::EDeviceState::DEVICE_ACTIVE_CONFIGURED &&
        m_input.m_ticData.m_touchInteraction ==
            tic::ETouchInteraction::LONG_TOUCH_DETECTED_SETUP) {
      m_output.m_connState = dcm::output::EConnectionState::PROV_BLE_ACTIVE;
      m_wifiDelegate.startBLProvisioning();
      return;
    }

    // if wifi creds available and not connected to ap --> try
    // connect to ap, otherwise get credentials
    if (!m_wifiDelegate.isConnectedAP()) {
      if (m_wifiDelegate.isConfigured()) {
        m_output.m_connState =
            dcm::output::EConnectionState::WIFI_NOT_CONNECTED_CONFIGURED;
        m_wifiDelegate.connectAP();
      } else {
        // only periodically
        // m_wifiDelegate.getCredentials();
        m_output.m_connState = dcm::output::EConnectionState::WIFI_NOT_CONFIG;
      }
      return;
    }

    // if connected to AP
    m_output.m_connState = dcm::output::EConnectionState::WIFI_CONNECTED;
  }

 private:
  runnable::dcm::input::CDCMInput& m_input;
  runnable::dcm::output::CDCMOutput& m_output;
  wifi::CWifiDelegate m_wifiDelegate;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
