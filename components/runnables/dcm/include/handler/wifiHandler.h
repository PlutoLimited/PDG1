#pragma once

#include "hw/wifidelegate.h"
#include "input/dcm_input.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace dcm {
namespace handler {

static const std::string G_TASK_TAG("[RUN::DCM]");

class CWifiHandler {
 public:
  CWifiHandler(runnable::dcm::input::CDCMInput& f_inputData,
               runnable::dcm::output::CDCMOutput& f_outputData)
      : m_input(f_inputData), m_output(f_outputData), m_wifiDelegate() {}

  void handle() {
    m_output.m_funcState = dcm::output::EDCMState::ACTIVE;

    // prov process active
    if (m_wifiDelegate.isProvActive()) {
      m_output.m_connState =
          dcm::output::EConnectionState::BLUETOOTH_SETUP_ACTIVE;
      return;
    }

    if (m_wifiDelegate.isProvFinishedSuccess()) {
      m_wifiDelegate.saveCredentials();
      return;
    }

    // on long touch and is not setup --> setup
    if (m_input.m_ticData.m_funcState == tic::ETICState::ACTIVE &&
        m_input.m_ticData.m_deviceState ==
            tic::EDeviceState::DEVICE_ACTIVE_CONFIGURED &&
        m_input.m_ticData.m_touchInteraction ==
            tic::ETouchInteraction::LONG_TOUCH_DETECTED_SETUP) {
      m_output.m_connState =
          dcm::output::EConnectionState::BLUETOOTH_SETUP_ACTIVE;
      m_wifiDelegate.startBLProvisioning();
      return;
    }

    // if wifi creds available and not connected to ap and not in setup--> try
    // connect to ap
    // if connected to AP
    if (!m_wifiDelegate.isConnectedAP()) {
      if (m_wifiDelegate.isConfigured()) {
        m_output.m_connState = dcm::output::EConnectionState::WIFI_CONN_ATTEMPT;
        m_wifiDelegate.connectAP();
      } else {
        m_output.m_connState = dcm::output::EConnectionState::WIFI_NOT_CONN;
      }
      return;
    }

    // if connected to AP
    m_output.m_connState = dcm::output::EConnectionState::WIFI_CONNECTED;
  }

 private:
  runnable::dcm::input::CDCMInput& m_input;
  runnable::dcm::output::CDCMOutput& m_output;
  hw::CWifiDelegate m_wifiDelegate;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
