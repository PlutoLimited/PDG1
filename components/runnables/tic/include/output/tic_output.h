#pragma once

#include <iostream>

namespace runnable {
namespace tic {
enum class ETICState { INACTIVE = 0, ACTIVE, DEGRADED };

enum class EDeviceState {
  DEVICE_NOT_PRESENT = 0,
  DEVICE_ACTIVE_NOT_CONF,
  DEVICE_ACTIVE_CONFIGURED,
};

enum class ETouchInteraction {
  NOT_AVAILABLE = 0,
  NO_INTERACTION,
  PROXIMITY_DETECTED,
  SLIDING_DETECTED,
  FLICK_DETECTED,
  TOUCH_DETECTED,
  LONG_TOUCH_DETECTED_SETUP
};

enum class ESliderCoordinates { NOT_AVAILABLE = 0, AVAILABLE };

class CTicOutput {
 public:
  CTicOutput()
      : m_funcState(ETICState::INACTIVE),
        m_deviceState(EDeviceState::DEVICE_NOT_PRESENT),
        m_touchInteraction(ETouchInteraction::NO_INTERACTION),
        m_coordinateState(ESliderCoordinates::NOT_AVAILABLE),
        m_sliderLevel(0U){};
  ETICState m_funcState;
  EDeviceState m_deviceState;
  ETouchInteraction m_touchInteraction;
  ESliderCoordinates m_coordinateState;
  uint8_t m_sliderLevel;
};

}  // namespace tic
}  // namespace runnable
