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
      : m_pinEN{G_PIN_EN, false, false, false, true},
        m_pinPWM{G_PIN_PWM, true, false, false, true} {};

  void setSafeStateHW() {
    m_pinEN.clr();
    m_pinPWM.clr();
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
          m_pinEN.set();
          // G_LOOKUP_DIM_TABLE[f_dataOut.m_dimLevel];
        }
        break;
    }
  }

 private:
  smooth::core::io::Output m_pinEN;   // active low
  smooth::core::io::Output m_pinPWM;  // active high
};
}  // namespace hw
}  // namespace llc
}  // namespace runnable