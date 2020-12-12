#pragma once

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

enum class EAWSState { AWS_NOT_CONNECTED = 0, AWS_CONNECTED };

enum class EOTAState { OTA_NOT_AVAILABLE = 0, OTA_IN_PROGRESS, OTA_SUCCESS };

class CDCMOutput {
 public:
  CDCMOutput()
      : m_funcState(EDCMState::INACTIVE),
        m_connState(EConnectionState::WIFI_OFF),
        m_awsState(EAWSState::AWS_NOT_CONNECTED),
        m_otaState(EOTAState::OTA_NOT_AVAILABLE){};
  EDCMState m_funcState;
  EConnectionState m_connState;
  EAWSState m_awsState;
  EOTAState m_otaState;
};

}  // namespace output
}  // namespace dcm
}  // namespace runnable
