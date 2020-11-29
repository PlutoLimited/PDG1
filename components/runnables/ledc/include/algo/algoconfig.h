#pragma once

#include <cstdint>

namespace runnable {
namespace ledc {
namespace algo {

static const uint16_t G_LLF_DURATION_CYCLES = 500U;  // 10secs

static const uint8_t G_LLF_BLINK_CYCLES = 50U;   // 1 sec
static const uint16_t G_LLF_CONT_CYCLES = 500U;  // 5 sec
static const float G_LLF_PULSE_BETA = 0.2;

}  // namespace algo
}  // namespace ledc
}  // namespace runnable