#pragma once

#include <iostream>

namespace runnable {
namespace dcm {
namespace output {

enum class EDCMState { INACTIVE = 0, ACTIVE, DEGRADED };

enum class EConnectionState {
  WIFI_OFF = 0,
  WIFI_NOT_CONFIG,
  WIFI_NOT_CONNECTED_CONFIGURED,
  WIFI_CONNECTED,
  PROV_BLE_ACTIVE,
  PROV_BLE_SUCCESS,
  PROV_BLE_FAIL
};

class CDCMOutput {
 public:
  CDCMOutput()
      : m_funcState(EDCMState::INACTIVE),
        m_connState(EConnectionState::WIFI_OFF){};
  EDCMState m_funcState;
  EConnectionState m_connState;
};

}  // namespace output
}  // namespace dcm
}  // namespace runnable
