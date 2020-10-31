/*******************************************************************************
 ****************Azoteq_IQS263B_Configuration***********************************
 ********************IQS263Config.h*********************************************
 *******************************************************************************/

#pragma once

#include <vector>

namespace runnable {
namespace tic {
namespace hw {

static const auto G_PIN_SCL = GPIO_NUM_22;
static const auto G_PIN_SDA = GPIO_NUM_21;
static const auto G_PIN_RDY = GPIO_NUM_13;
static const uint32_t G_I2C_FREQ = 100000U;

// long touch threshold
static const uint8_t G_LONG_TOUCH_SETUP = 100U;

// device address
static const uint8_t G_IQS263_ADDR = 0x44;
// registers
static const uint8_t G_REG_SYSTEM_FLAGS_EVENTS = 0x01;   // 2 bytes
static const uint8_t G_REG_WHEEL_COORDINATES = 0x02;     // 3 bytes
static const uint8_t G_REG_PROX_DEVICE_SETTINGS = 0x09;  // 5 bytes

// write - configuration
static std::vector<uint8_t> G_WRITE_CLEAR_RESET_DATA{G_REG_SYSTEM_FLAGS_EVENTS,
                                                     0x00};

static std::vector<uint8_t> G_WRITE_DEVICE_PROX_CONFIG_DATA{
    G_REG_PROX_DEVICE_SETTINGS, 0x00, 0x19, 0x00, 0x00, 0xFF};

// read - bitmasks
enum class E_SYSTEM_FLAGS_BITMASK : uint8_t {
  DEVICE_RESET = 0b10000000,
  MOVEMENT_DETECTED = 0b01000000,
  PROJECTED_MODE = 0b00010000,
};

enum class E_EVENTS_BITMASK : uint8_t {
  FLICK_LEFT = 0b10000000,
  FLICK_RIGHT = 0b01000000,
  TAP = 0b00100000,
  MOVEMENT = 0b00010000,
  ATI_EVENT = 0b00001000,
  SLIDE_EVENT = 0b00000100,
  TOUCH_EVENT = 0b00000010,
  PROX_EVENT = 0b00000001
};

}  // namespace hw
}  // namespace tic
}  // namespace runnable