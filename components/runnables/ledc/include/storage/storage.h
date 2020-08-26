#pragma once

#include <iostream>

#include "input/ledc_input.h"
#include "memport.h"
#include "output/ledc_output.h"
#include "output/llc_output.h"
#include "output/tic_output.h"

namespace runnable {
namespace ledc {
namespace storage {

enum class EKeepAliveCounterState { INACTIVE = 0, ACTIVE, ACTIVE_PROX };

class CLedcStorageData {
 public:
  CLedcStorageData()
      : m_keepAliveCounterState(EKeepAliveCounterState::INACTIVE),
        m_keepAliveCounterValue(0U),
        m_prevLLCState(runnable::llc::ELightState::LIGHT_OFF),
        m_prevLLCLevel(0U){};

  EKeepAliveCounterState m_keepAliveCounterState;
  uint16_t m_keepAliveCounterValue;
  runnable::llc::ELightState m_prevLLCState;
  uint8_t m_prevLLCLevel;
};

}  // namespace storage
}  // namespace ledc
}  // namespace runnable
