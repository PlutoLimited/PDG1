
#pragma once

#include <cstdint>

namespace runnable {
namespace tic {
namespace hw {

enum class ECommState { FAIL = 0, SUCCESS };

enum class EResetOccured { NO_RESET = 0, RESET_OCCURED };

enum class EMovementDetected { NO_MOVEMENT = 0, MOVEMENT };

enum class EFlickEvent { NO_FLICK = 0, FLICK_LEFT, FLICK_RIGHT };

enum class ESlideEvent { NO_SLIDE = 0, SLIDE };

enum class ETouchEvent { NO_TOUCH = 0, TOUCH };

enum class EProxEvent { NO_PROX = 0, PROX };

struct CSystemFlagsEventsData {
  CSystemFlagsEventsData()
      : m_commState(ECommState::FAIL),
        m_reset(EResetOccured::NO_RESET),
        m_movement(EMovementDetected::NO_MOVEMENT),
        m_flick(EFlickEvent::NO_FLICK),
        m_slide(ESlideEvent::NO_SLIDE),
        m_touch(ETouchEvent::NO_TOUCH),
        m_prox(EProxEvent::NO_PROX){};

  ECommState m_commState;
  EResetOccured m_reset;
  EMovementDetected m_movement;
  EFlickEvent m_flick;
  ESlideEvent m_slide;
  ETouchEvent m_touch;
  EProxEvent m_prox;
};

struct CSliderCoordinateData {
  CSliderCoordinateData() : m_commState(ECommState::FAIL), m_sliderCoord(0U){};
  ECommState m_commState;
  uint8_t m_sliderCoord;
};

}  // namespace hw
}  // namespace tic
}  // namespace runnable
