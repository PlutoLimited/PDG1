#pragma once

#include "driver/ledc.h"
#include "smooth/core/io/Output.h"

namespace runnable {
namespace llc {
namespace hw {

static const auto G_PIN_EN = GPIO_NUM_4;
static const auto G_PIN_PWM = GPIO_NUM_2;

static const uint32_t G_LOOKUP_DIM_TABLE[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2047};

static const ledc_channel_t G_CHANNEL_PWM = LEDC_CHANNEL_0;
static const ledc_timer_bit_t G_PWM_RES_TIMER_BIT = LEDC_TIMER_10_BIT;
static const uint32_t G_PWM_FREQ_HZ = 10000U;

static const uint8_t G_SLIDER_DIM_LOW_BOUND = 20;
static const uint8_t G_SLIDER_DIM_UPP_BOUND = 170;

static const uint8_t G_SLIDER_HYST = 10;

static const uint8_t G_SLIDER_MIN_HYST_UPPER = 30;
static const uint8_t G_SLIDER_MIN_HYST_LOWER = 25;

}  // namespace hw
}  // namespace llc
}  // namespace runnable