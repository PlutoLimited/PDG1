#include "hw/IQS263B.h"

#include <vector>

#include "hw/IQS263Config.h"

namespace runnable {
namespace tic {
namespace hw {

// Constructor
IQS263B::IQS263B(i2c_port_t port, uint8_t address, std::mutex& guard)
    : smooth::core::io::i2c::I2CMasterDevice(port, address, guard) {}

bool IQS263B::clr_reset_bit() { return false; }

bool IQS263B::configure_device() {
  bool res = write(address, G_WRITE_DEVICE_PROX_CONFIG_DATA);

  // Read back value to confirm write.
  // smooth::core::util::FixedBuffer<uint8_t, 5> read_data;
  //  res = res && read(address, G_PROX_DEVICE_SETTINGS, read_data);
  return res;
}

bool IQS263B::read_wheel_coordinates(CSliderCoordinateData& f_sliderDataOut) {
  CSliderCoordinateData defaultData;
  f_sliderDataOut = defaultData;

  smooth::core::util::FixedBuffer<uint8_t, 1> data;
  auto res = read(address, G_REG_WHEEL_COORDINATES, data);

  if (!res) {
    return res;
  }

  f_sliderDataOut.m_commState = ECommState::SUCCESS;
  f_sliderDataOut.m_sliderCoord = data[0];

  return res;
}

bool IQS263B::read_system_flags_events(
    CSystemFlagsEventsData& f_sysEventDataOut) {
  CSystemFlagsEventsData defaultData;
  f_sysEventDataOut = defaultData;

  smooth::core::util::FixedBuffer<uint8_t, 2> data;
  auto res = read(address, G_REG_SYSTEM_FLAGS_EVENTS, data, true, true);

  if (!res) {
    return res;
  }

  f_sysEventDataOut.m_commState = ECommState::SUCCESS;

  if (data[0] & static_cast<uint8_t>(E_SYSTEM_FLAGS_BITMASK::DEVICE_RESET)) {
    f_sysEventDataOut.m_reset = EResetOccured::RESET_OCCURED;
  } else {
    f_sysEventDataOut.m_reset = EResetOccured::NO_RESET;
  }

  if (data[0] &
      static_cast<uint8_t>(E_SYSTEM_FLAGS_BITMASK::MOVEMENT_DETECTED)) {
    f_sysEventDataOut.m_movement = EMovementDetected::MOVEMENT;
  } else {
    f_sysEventDataOut.m_movement = EMovementDetected::NO_MOVEMENT;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::SLIDE_EVENT)) {
    f_sysEventDataOut.m_slide = ESlideEvent::SLIDE;
  } else {
    f_sysEventDataOut.m_slide = ESlideEvent::NO_SLIDE;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::TOUCH_EVENT)) {
    f_sysEventDataOut.m_touch = ETouchEvent::TOUCH;
  } else {
    f_sysEventDataOut.m_touch = ETouchEvent::NO_TOUCH;
  }

  if (data[1] & static_cast<uint8_t>(E_EVENTS_BITMASK::PROX_EVENT)) {
    f_sysEventDataOut.m_prox = EProxEvent::PROX;
  } else {
    f_sysEventDataOut.m_prox = EProxEvent::NO_PROX;
  }

  return res;
}

}  // namespace hw
}  // namespace tic
}  // namespace runnable