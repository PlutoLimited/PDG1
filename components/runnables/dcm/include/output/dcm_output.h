#pragma once

#include <iostream>

namespace runnable {
namespace dcm {
namespace output {

enum class EDCMState { INACTIVE = 0, ACTIVE, DEGRADED };

enum class EConnectionState {
  WIFI_OFF = 0,
  WIFI_NOT_CONN,
  WIFI_CONN_ATTEMPT,
  WIFI_CONNECTED,
  WIFI_NOT_CONF,
  BLUETOOTH_SETUP_ACTIVE
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
