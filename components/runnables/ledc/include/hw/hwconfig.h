#pragma once

#include "driver/ledc.h"
#include "smooth/core/io/Output.h"

namespace runnable {
namespace ledc {
namespace hw {

static const auto G_PIN_LED_1 = GPIO_NUM_14;
static const auto G_PIN_LED_2 = GPIO_NUM_15;
static const auto G_PIN_LED_3 = GPIO_NUM_16;
static const auto G_PIN_LED_4 = GPIO_NUM_17;
static const auto G_PIN_LED_5 = GPIO_NUM_18;
static const auto G_PIN_LED_6 = GPIO_NUM_19;
static const auto G_PIN_LED_7 = GPIO_NUM_23;
static const auto G_PIN_LED_8 = GPIO_NUM_25;
static const auto G_PIN_LED_9 = GPIO_NUM_26;
static const auto G_PIN_LED_10 = GPIO_NUM_27;

static const auto G_PIN_LED_RGB_R1 = GPIO_NUM_32;
static const auto G_PIN_LED_RGB_G1 = GPIO_NUM_33;
static const auto G_PIN_LED_RGB_B1 = GPIO_NUM_12;

static const ledc_channel_t G_CHANNEL_R1_PWM = LEDC_CHANNEL_1;
static const ledc_channel_t G_CHANNEL_G1_PWM = LEDC_CHANNEL_2;
static const ledc_channel_t G_CHANNEL_B1_PWM = LEDC_CHANNEL_3;

static const ledc_timer_bit_t G_PWM_RES_TIMER_BIT = LEDC_TIMER_10_BIT;
static const uint32_t G_PWM_FREQ_HZ = 10000U;

}  // namespace hw
}  // namespace ledc
}  // namespace runnable