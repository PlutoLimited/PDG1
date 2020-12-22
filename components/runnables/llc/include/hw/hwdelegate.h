#pragma once

#include "hwconfig.h"
#include "output/llc_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace llc {
namespace hw {

static const std::string G_HW_TAG("[RUN::LLC::HW]");

class CHWDelegate {
 public:
  CHWDelegate()
      : m_pinEN{G_PIN_EN, false, false, false,
                true},  // GPIO_MODE_INPUT_OUTPUT_OD
        m_pinPWM{G_PIN_PWM, true, false, false, true},
        m_prevDim(0U) {
    m_pinPWM.setupPwm(G_CHANNEL_PWM, G_PWM_RES_TIMER_BIT, G_PWM_FREQ_HZ);
    m_pinPWM.setDutyCycle(0U);
  };

  void setSafeStateHW() {
    m_pinEN.clr();
    m_pinPWM.setDutyCycle(0U);
    m_prevDim = 0U;
  }

  void setHW(const runnable::llc::CLlcOutput& f_dataOut) {
    switch (f_dataOut.m_funcState) {
      case runnable::llc::ELLCState::DEGRADED:
        setSafeStateHW();
        break;
      case runnable::llc::ELLCState::INACTIVE:
        setSafeStateHW();
        break;
      case runnable::llc::ELLCState::ACTIVE:
        if (f_dataOut.m_lightState == runnable::llc::ELightState::LIGHT_OFF) {
          setSafeStateHW();
        } else {
          if (m_prevDim == 0U) {
            m_pinEN.set();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
          }

          if (f_dataOut.m_dimLevel > 0 && f_dataOut.m_dimLevel <= 100) {
            uint32_t setpoint = G_LOOKUP_DIM_TABLE[f_dataOut.m_dimLevel - 1];
            m_prevDim = f_dataOut.m_dimLevel;
            // const auto diff = float(setpoint - m_prevPWM);
            // m_prevPWM += uint32_t(
            //     diff * (G_LPF_BETA_PT1 / (1.F + 0.08 * std::abs(diff))));

            m_pinPWM.setDutyCycle(setpoint);
          }
        }
        break;
    }
  }

 private:
  smooth::core::io::Output m_pinEN;   // active low
  smooth::core::io::Output m_pinPWM;  // active high
  uint8_t m_prevDim;
};
}  // namespace hw
}  // namespace llc
}  // namespace runnable