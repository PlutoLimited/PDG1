#pragma once

#include "algo/algoconfig.h"
#include "hwconfig.h"
#include "output/ledc_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "tools.h"

namespace runnable {
namespace ledc {
namespace hw {

static const std::string G_HW_TAG("[RUN::LLC::HW]");

class CHWDelegate {
 public:
  CHWDelegate()
      : m_pinLED1{G_PIN_LED_1, true, false, false, true},
        m_pinLED2{G_PIN_LED_2, true, false, false, true},
        m_pinLED3{G_PIN_LED_3, true, false, false, true},
        m_pinLED4{G_PIN_LED_4, true, false, false, true},
        m_pinLED5{G_PIN_LED_5, true, false, false, true},
        m_pinLED6{G_PIN_LED_6, true, false, false, true},
        m_pinLED7{G_PIN_LED_7, true, false, false, true},
        m_pinLED8{G_PIN_LED_8, true, false, false, true},
        m_pinLED9{G_PIN_LED_9, true, false, false, true},
        m_pinLED10{G_PIN_LED_10, true, false, false, true},
        m_led_arr_p{&m_pinLED1, &m_pinLED2, &m_pinLED3, &m_pinLED4,
                    &m_pinLED5, &m_pinLED6, &m_pinLED7, &m_pinLED8,
                    &m_pinLED9, &m_pinLED10},
        m_pinLED_R1{G_PIN_LED_12_RED, true, false, false, true},
        m_pinLED_G1{G_PIN_LED_11_GREEN, true, false, false, true},
        m_rg_arr_p{&m_pinLED_R1, &m_pinLED_G1},
        m_pinHF1{G_PIN_VIB_HF_1, true, false, false, true},
        m_statusSetupPulse(algo::G_LLF_BLINK_CYCLES, 9999U,
                           algo::G_LLF_PULSE_BETA),
        m_statusSetupFailed(algo::G_LLF_CONT_CYCLES, 2U,
                            algo::G_LLF_PULSE_BETA),
        m_statusSetupSuccessful(algo::G_LLF_CONT_CYCLES, 2U,
                                algo::G_LLF_PULSE_BETA) {
    m_pinLED_R1.setupPwm(G_CHANNEL_R1_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinLED_R1.setDutyCycle(0U);

    m_pinLED_G1.setupPwm(G_CHANNEL_G1_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinLED_G1.setDutyCycle(0U);

    setLEDOffState();
    setRGOffState();
    setHFOffState();
  };

  void setLEDOffState() {
    for (uint8_t i = 0; i < std::size(m_led_arr_p); i++) {
      m_led_arr_p[i]->clr();
    }
  }

  // f_lvl, range 1 - 10
  void setConsecLEDOnLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < std::size(m_led_arr_p); i++) {
      if (i < f_lvl) {
        m_led_arr_p[i]->set();
      } else {
        m_led_arr_p[i]->clr();
      }
    }
  }

  // f_lvl, range 1 - 10
  void setConsecLEDOffLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < std::size(m_led_arr_p); i++) {
      if (i < f_lvl) {
        m_led_arr_p[i]->clr();
      } else {
        m_led_arr_p[i]->set();
      }
    }
  }

  // f_lvl, range 1 - 10
  void setSingleLEDOnLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < std::size(m_led_arr_p); i++) {
      if (i == (f_lvl - 1)) {
        m_led_arr_p[i]->set();
      } else {
        m_led_arr_p[i]->clr();
      }
    }
  }

  void setLEDProxAnim() {}

  void setDeviceDegradationAnim() {
    // continuous red
    setRGfromHSV(0.F, 1.F, 1.F);
  }

  void setDeviceSetupActiveAnim() {
    const auto brightness = m_statusSetupPulse.runAndGetBrightness();
    // pluto color pulsing
    setRGfromHSV(51.F, 1.F, brightness);
  }

  bool isDeviceSetupFailedAnimActive() {
    return (!m_statusSetupFailed.hasFinished() &&
            m_statusSetupFailed.isActive());
  }

  void setDeviceSetupFailedAnim() {
    const auto brightness = m_statusSetupFailed.runAndGetBrightness();
    // pluto color pulsing
    setRGfromHSV(0.F, 1.F, brightness);
  }

  bool isDeviceSetupSuccessAnimActive() {
    return (!m_statusSetupSuccessful.hasFinished() &&
            m_statusSetupSuccessful.isActive());
  }

  void setDeviceSetupSuccessAnim() {
    const auto brightness = m_statusSetupSuccessful.runAndGetBrightness();
    // pluto color pulsing
    setRGfromHSV(120.F, 1.F, brightness);
  }

  void resetRGAnims() {
    m_statusSetupSuccessful.reset();
    m_statusSetupFailed.reset();
    m_statusSetupPulse.reset();
  }

  void setRGOffState() {
    resetRGAnims();
    for (uint8_t i = 0; i < std::size(m_rg_arr_p); i++) {
      m_rg_arr_p[i]->clr();
      m_rg_arr_p[i]->setDutyCycle(0U);
    }
  }
  // hue in deg, saturation, brightness range [0,1]
  void setRGfromHSV(const float f_hue, const float f_saturation,
                    const float f_brightness) {
    hsv in{f_hue, f_saturation, f_brightness};
    rgb out;
    out = hsv2rgb(in);
    // needs to be scaled to [0,255]
    uint8_t r_scaled = uint8_t(out.r * 254.F);
    uint8_t g_scaled = uint8_t(out.g * 254.F);
    // uint8_t b_scaled = uint8_t(out.b * 255.F);

    m_rg_arr_p[0]->setDutyCycle(r_scaled);
    m_rg_arr_p[1]->setDutyCycle(g_scaled);
  }

  void setHFOffState() { m_pinHF1.clr(); }

 private:
  smooth::core::io::Output m_pinLED1;   // active high
  smooth::core::io::Output m_pinLED2;   // active high
  smooth::core::io::Output m_pinLED3;   // active high
  smooth::core::io::Output m_pinLED4;   // active high
  smooth::core::io::Output m_pinLED5;   // active high
  smooth::core::io::Output m_pinLED6;   // active high
  smooth::core::io::Output m_pinLED7;   // active high
  smooth::core::io::Output m_pinLED8;   // active high
  smooth::core::io::Output m_pinLED9;   // active high
  smooth::core::io::Output m_pinLED10;  // active high
  smooth::core::io::Output* m_led_arr_p[10];
  smooth::core::io::Output m_pinLED_R1;  // active high,pwm
  smooth::core::io::Output m_pinLED_G1;  // active high,pwm
  smooth::core::io::Output* m_rg_arr_p[2];
  smooth::core::io::Output m_pinHF1;  // active high,pwm

  CPulseBrightness m_statusSetupPulse;
  CPulseBrightness m_statusSetupFailed;
  CPulseBrightness m_statusSetupSuccessful;
};

}  // namespace hw
}  // namespace ledc
}  // namespace runnable