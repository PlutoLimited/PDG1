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

enum class EAWSDesiredLightState { NOT_DESIRED = 0, NEW_LVL_DESIRED };

class CDCMOutput {
 public:
  CDCMOutput()
      : m_funcState(EDCMState::INACTIVE),
        m_connState(EConnectionState::WIFI_OFF),
        m_awsState(EAWSState::AWS_NOT_CONNECTED),
        m_awsDesiredLightLvl(EAWSDesiredLightState::NOT_DESIRED),
        m_desiredLvl(0U),
        m_otaState(EOTAState::OTA_NOT_AVAILABLE){};
  EDCMState m_funcState;
  EConnectionState m_connState;
  EAWSState m_awsState;
  EAWSDesiredLightState m_awsDesiredLightLvl;
  uint8_t m_desiredLvl;
  EOTAState m_otaState;
};

}  // namespace output
}  // namespace dcm
}  // namespace runnable
