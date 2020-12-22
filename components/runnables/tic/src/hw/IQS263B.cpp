#include "hw/IQS263B.h"

#include <vector>

#include "hw/IQS263Config.h"
#include "smooth/core/Application.h"
#include "smooth/core/logging/log.h"

namespace runnable {
namespace tic {
namespace hw {

// Constructor
IQS263B::IQS263B(i2c_port_t port, uint8_t address, std::mutex& guard)
    : smooth::core::io::i2c::I2CMasterDevice(port, address, guard),
      m_pinReadyOut{G_PIN_RDY, false, false, false, true} {}

bool IQS263B::clr_reset_bit() {
  m_pinReadyOut.set();

  auto res = false;
  for (uint8_t i = 0; i < 30; i++) {
    res = write(address, G_WRITE_CLEAR_RESET_DATA);
    if (res) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  m_pinReadyOut.clr();
  return res;
}

bool IQS263B::configure_device() {
  m_pinReadyOut.set();

  bool res(false);
  for (uint8_t i = 0; i < 10; i++) {
    res = write(address, G_WRITE_DEVICE_PROX_CONFIG_DATA);

    if (res) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  if (res) {
    res = false;
    for (uint8_t i = 0; i < 10; i++) {
      res = write(address, G_WRITE_THRESHOLDS_CONFIG_DATA);

      if (res) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  m_pinReadyOut.clr();
  return res;
}

bool IQS263B::read_wheel_coordinates(CSliderCoordinateData& f_sliderDataOut) {
  CSliderCoordinateData defaultData;
  f_sliderDataOut = defaultData;

  m_pinReadyOut.set();

  smooth::core::util::FixedBuffer<uint8_t, 1> data;
  auto res = false;
  for (uint8_t i = 0; i < 30; i++) {
    res = read(address, G_REG_WHEEL_COORDINATES, data);
    if (res) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  if (!res) {
    m_pinReadyOut.clr();
    return res;
  }
  Log::info(G_HW_TAG, "SLIDER COORDINATES --- {}", data[0]);
  f_sliderDataOut.m_commState = ECommState::SUCCESS;
  f_sliderDataOut.m_sliderCoord = data[0];
  m_pinReadyOut.clr();
  return res;
}

bool IQS263B::read_system_flags_events(
    CSystemFlagsEventsData& f_sysEventDataOut) {
  CSystemFlagsEventsData defaultData;
  f_sysEventDataOut = defaultData;

  m_pinReadyOut.set();

  smooth::core::util::FixedBuffer<uint8_t, 2> data;
  auto res = false;
  for (uint8_t i = 0; i < 30; i++) {
    res = read(address, G_REG_SYSTEM_FLAGS_EVENTS, data, true, true);
    if (res) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  if (!res) {
    m_pinReadyOut.clr();
    return res;
  }

  f_sysEventDataOut.m_commState = ECommState::SUCCESS;

  if (data[0] & static_cast<uint8_t>(E_SYSTEM_FLAGS_BITMASK::DEVICE_RESET)) {
    f_sysEventDataOut.m_reset = EResetOccured::RESET_OCCURED;
    Log::info(G_HW_TAG, "RESET OCCURED");
  } else {
    f_sysEventDataOut.m_reset = EResetOccured::NO_RESET;
  }

  if (data[0] &
      static_cast<uint8_t>(E_SYSTEM_FLAGS_BITMASK::MOVEMENT_DETECTED)) {
    f_sysEventDataOut.m_movement = EMovementDetected::MOVEMENT;
    Log::info(G_HW_TAG, "MOVEMENT DETECTED");
  } else {
    f_sysEventDataOut.m_movement = EMovementDetected::NO_MOVEMENT;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::SLIDE_EVENT)) {
    f_sysEventDataOut.m_slide = ESlideEvent::SLIDE;
    Log::info(G_HW_TAG, "SLIDE EVENT");
  } else {
    f_sysEventDataOut.m_slide = ESlideEvent::NO_SLIDE;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::TOUCH_EVENT)) {
    f_sysEventDataOut.m_touch = ETouchEvent::TOUCH;
    Log::info(G_HW_TAG, "TOUCH EVENT");
  } else {
    f_sysEventDataOut.m_touch = ETouchEvent::NO_TOUCH;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::PROX_EVENT)) {
    f_sysEventDataOut.m_prox = EProxEvent::PROX;
    Log::info(G_HW_TAG, "PROX EVENT");
  } else {
    f_sysEventDataOut.m_prox = EProxEvent::NO_PROX;
  }

  m_pinReadyOut.clr();
  return res;
}

}  // namespace hw
}  // namespace tic
}  // namespace runnable