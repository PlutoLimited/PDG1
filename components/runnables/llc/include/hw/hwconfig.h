#pragma once

#include "driver/ledc.h"
#include "smooth/core/io/Output.h"

namespace runnable {
namespace llc {
namespace hw {

static const auto G_PIN_EN = GPIO_NUM_4;
static const auto G_PIN_PWM = GPIO_NUM_2;

static const uint32_t G_LOOKUP_DIM_TABLE[100] = {
    3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,
    18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
    33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
    48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  67,  68,  69,  70,  72,  74,  76,  78,  80,  82,  84,
    86,  88,  90,  92,  94,  96,  98,  100, 102, 104, 106, 108, 110, 112, 114,
    116, 118, 120, 130, 140, 150, 160, 170, 190, 230};

static const uint8_t G_ENABLE_PWM_DELAY_CYCLES = 2;

static const ledc_channel_t G_CHANNEL_PWM = LEDC_CHANNEL_0;
static const ledc_timer_bit_t G_PWM_RES_TIMER_BIT = LEDC_TIMER_10_BIT;
static const uint32_t G_PWM_FREQ_HZ = 10000U;

static const uint8_t G_SLIDER_DIM_LOW_BOUND = 30;
static const uint8_t G_SLIDER_DIM_UPP_BOUND = 150;

static const uint8_t G_SLIDER_HYST = 5;

}  // namespace hw
}  // namespace llc
}  // namespace runnable