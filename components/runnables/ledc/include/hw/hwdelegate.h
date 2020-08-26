#pragma once

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
        m_pinLED_R1{G_PIN_LED_RGB_R1, true, false, false, true},
        m_pinLED_G1{G_PIN_LED_RGB_G1, true, false, false, true},
        m_pinLED_B1{G_PIN_LED_RGB_B1, true, false, false, true},
        m_rgb_arr_p{&m_pinLED_R1, &m_pinLED_G1, &m_pinLED_B1} {
    m_pinLED_R1.setupPwm(G_CHANNEL_R1_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinLED_R1.setDutyCycle(0U);

    m_pinLED_G1.setupPwm(G_CHANNEL_G1_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinLED_G1.setDutyCycle(0U);

    m_pinLED_B1.setupPwm(G_CHANNEL_B1_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinLED_B1.setDutyCycle(0U);
  };

  void setLEDOffState() {
    for (uint8_t i = 0; i < 10; i++) {
      m_led_arr_p[i]->clr();
    }
  }

  // f_lvl, range 1 - 10
  void setConsecLEDOnLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < 10; i++) {
      if (i < f_lvl) {
        m_led_arr_p[i]->set();
      } else {
        m_led_arr_p[i]->clr();
      }
    }
  }

  // f_lvl, range 1 - 10
  void setConsecLEDOffLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < 10; i++) {
      if (i < f_lvl) {
        m_led_arr_p[i]->clr();
      } else {
        m_led_arr_p[i]->set();
      }
    }
  }

  // f_lvl, range 1 - 10
  void setSingleLEDOnLvl(const uint8_t f_lvl) {
    for (uint8_t i = 0; i < 10; i++) {
      if (i == (f_lvl - 1)) {
        m_led_arr_p[i]->set();
      } else {
        m_led_arr_p[i]->clr();
      }
    }
  }

  void setLEDProxAnim() {}

  void setRGBOffState() {
    for (uint8_t i = 0; i < 3; i++) {
      m_rgb_arr_p[i]->clr();
    }
  }
  // hue in deg, saturation, brightness range [0,1]
  void setRGBfromHSV(const float f_hue, const float f_saturation,
                     const float f_brightness) {
    hsv in{f_hue, f_saturation, f_brightness};
    rgb out;
    out = hsv2rgb(in);
    // needs to be scaled to [0,255]
    uint8_t r_scaled = uint8_t(out.r * 255.F);
    uint8_t g_scaled = uint8_t(out.g * 255.F);
    uint8_t b_scaled = uint8_t(out.b * 255.F);

    m_rgb_arr_p[0]->setDutyCycle(r_scaled);
    m_rgb_arr_p[1]->setDutyCycle(g_scaled);
    m_rgb_arr_p[2]->setDutyCycle(b_scaled);
  }

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
  smooth::core::io::Output m_pinLED_B1;  // active high,pwm
  smooth::core::io::Output* m_rgb_arr_p[3];
};

}  // namespace hw
}  // namespace ledc
}  // namespace runnable