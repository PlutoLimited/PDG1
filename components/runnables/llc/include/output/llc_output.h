#pragma once

#include <iostream>

namespace runnable {
namespace llc {
enum class ELLCState { INACTIVE = 0, ACTIVE, DEGRADED };

enum class ELightState { LIGHT_OFF = 0, LIGHT_DIM, LIGHT_FULL_ON };

class CLlcOutput {
 public:
  CLlcOutput()
      : m_funcState(ELLCState::INACTIVE),
        m_lightState(ELightState::LIGHT_OFF),
        m_dimLevel(0U){};
  ELLCState m_funcState;
  ELightState m_lightState;
  uint8_t m_dimLevel;
};

}  // namespace llc
}  // namespace runnable
