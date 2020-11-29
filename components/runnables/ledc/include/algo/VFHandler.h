#pragma once

#include "algoconfig.h"
#include "hw/hwdelegate.h"
#include "input/ledc_input.h"
#include "output/ledc_output.h"
#include "output/llc_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "storage/storage.h"

namespace runnable {
namespace ledc {
namespace algo {

static const std::string G_TASK_TAG("[RUN::LEDC]");

class CVFHandler {
 public:
  CVFHandler(runnable::ledc::input::CLedcInputData& f_inputData,
             runnable::ledc::CLedcOutput& f_outputData)
      : m_input(f_inputData),
        m_output(f_outputData),
        m_HWDelegate(),
        m_storage() {}

  void handleLightLvlFeedback() {
    if (!isLLCFuncStatePermissible()) {
      m_HWDelegate.setLEDOffState();
      m_output.m_funcState = runnable::ledc::ELEDCState::ACTIVE;
      m_output.m_ledState = runnable::ledc::ELedState::LED_OFF;
      return;
    }
    m_output.m_funcState = runnable::ledc::ELEDCState::ACTIVE;
    switch (m_input.m_llcOutData.m_lightState) {
      case runnable::llc::ELightState::LIGHT_OFF:
        m_output.m_ledState = handleLightOffLlf();
        break;
      case runnable::llc::ELightState::LIGHT_DIM:
        m_output.m_ledState = handleLightDimLlf();
        break;
      case runnable::llc::ELightState::LIGHT_FULL_ON:
        m_output.m_ledState = handleLightOnLlf();
        break;
    }
    m_storage.m_prevLLCState = m_input.m_llcOutData.m_lightState;
    m_storage.m_prevLLCLevel = m_input.m_llcOutData.m_dimLevel;
  }

  void handleDeviceStatusFeedback() {
    m_output.m_funcState = runnable::ledc::ELEDCState::ACTIVE;
    // if device degradation happened, indicate error
    if (!isLLCFuncStatePermissible()) {
      m_output.m_ledState = runnable::ledc::ELedState::LED_ERROR_INDICATION;
      m_HWDelegate.setDeviceDegradationAnim();
      return;
    }

    // if device setup currently in progress, indicate
    if (m_input.m_dcmOutData.m_connState ==
        runnable::dcm::output::EConnectionState::PROV_BLE_ACTIVE) {
      m_output.m_ledState =
          runnable::ledc::ELedState::LED_DEVICE_SETUP_ACTIVE_INDICATION;
      m_HWDelegate.setDeviceSetupActiveAnim();
      return;
    }

    // if any setup-related animation is currently happening, proceed until
    // finished
    if (m_HWDelegate.isDeviceSetupSuccessAnimActive()) {
      m_output.m_ledState =
          runnable::ledc::ELedState::LED_DEVICE_SETUP_DONE_SUCCESS_INDICATION;
      m_HWDelegate.setDeviceSetupSuccessAnim();
      if (!m_HWDelegate.isDeviceSetupSuccessAnimActive()) {
        m_HWDelegate.resetRGAnims();
      }
      return;
    }

    if (m_HWDelegate.isDeviceSetupFailedAnimActive()) {
      m_output.m_ledState =
          runnable::ledc::ELedState::LED_DEVICE_SETUP_DONE_FAIL_INDICATION;
      m_HWDelegate.setDeviceSetupFailedAnim();
      if (!m_HWDelegate.isDeviceSetupFailedAnimActive()) {
        m_HWDelegate.resetRGAnims();
      }
      return;
    }

    // if device setup failed, start animation
    if (m_input.m_dcmOutData.m_connState ==
        runnable::dcm::output::EConnectionState::PROV_BLE_FAIL) {
      m_output.m_ledState =
          runnable::ledc::ELedState::LED_DEVICE_SETUP_DONE_FAIL_INDICATION;
      m_HWDelegate.setDeviceSetupFailedAnim();
      return;
    }

    // if device setup successful, start animation
    if (m_input.m_dcmOutData.m_connState ==
        runnable::dcm::output::EConnectionState::PROV_BLE_SUCCESS) {
      m_output.m_ledState =
          runnable::ledc::ELedState::LED_DEVICE_SETUP_DONE_SUCCESS_INDICATION;
      m_HWDelegate.setDeviceSetupSuccessAnim();
      return;
    }

    // status indicator off
    m_HWDelegate.setRGOffState();
  }

 private:
  runnable::ledc::ELedState handleLightOnLlf() {
    if (hasLLCLevelChanged()) {
      // start keep alive counter
      startKeepAliveCounter();
      m_HWDelegate.setConsecLEDOnLvl(
          dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
      return runnable::ledc::ELedState::LED_LIGHT_LVL_INDICATION;
      // also notify output
    } else {
      if (isKeepAliveCounterActive()) {
        m_storage.m_keepAliveCounterValue++;
        if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
          endKeepAliveCounter();
          m_HWDelegate.setLEDOffState();
          return runnable::ledc::ELedState::LED_OFF;
        } else {
          m_HWDelegate.setConsecLEDOnLvl(
              dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
          return runnable::ledc::ELedState::LED_LIGHT_LVL_INDICATION;
        }
      } else {
        // here may be the place for proximity indication logic
        if (isKeepAliveCounterActiveProximity()) {
          m_storage.m_keepAliveCounterValue++;
          if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
            endKeepAliveCounter();
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          } else {
            m_HWDelegate.setConsecLEDOnLvl(
                dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          }
        } else {
          if (isTICInteractionProximity()) {
            startKeepAliveCounterProx();
            m_HWDelegate.setConsecLEDOnLvl(
                dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          } else {
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          }
        }
      }
    }
  }

  runnable::ledc::ELedState handleLightDimLlf() {
    if (hasLLCLevelChanged()) {
      // start keep alive counter
      startKeepAliveCounter();
      m_HWDelegate.setConsecLEDOnLvl(
          dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
      return runnable::ledc::ELedState::LED_LIGHT_LVL_INDICATION;
      // also notify output
    } else {
      if (isKeepAliveCounterActive()) {
        m_storage.m_keepAliveCounterValue++;
        if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
          endKeepAliveCounter();
          m_HWDelegate.setLEDOffState();
          return runnable::ledc::ELedState::LED_OFF;
        } else {
          m_HWDelegate.setConsecLEDOnLvl(
              dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
          return runnable::ledc::ELedState::LED_LIGHT_LVL_INDICATION;
        }
      } else {
        // here may be the place for proximity indication logic
        if (isKeepAliveCounterActiveProximity()) {
          m_storage.m_keepAliveCounterValue++;
          if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
            endKeepAliveCounter();
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          } else {
            m_HWDelegate.setConsecLEDOnLvl(
                dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          }
        } else {
          if (isTICInteractionProximity()) {
            startKeepAliveCounterProx();
            m_HWDelegate.setConsecLEDOnLvl(
                dimLevelToLLFLeds(m_input.m_llcOutData.m_dimLevel));
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          } else {
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          }
        }
      }
    }
  }

  runnable::ledc::ELedState handleLightOffLlf() {
    if (hasLLCLevelChanged()) {
      // start keep alive counter
      startKeepAliveCounter();
      m_HWDelegate.setLEDOffState();
      return runnable::ledc::ELedState::LED_OFF;
      // also notify output
    } else {
      if (isKeepAliveCounterActive()) {
        m_storage.m_keepAliveCounterValue++;
        if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
          endKeepAliveCounter();
          m_HWDelegate.setLEDOffState();
          return runnable::ledc::ELedState::LED_OFF;
        } else {
          m_HWDelegate.setLEDOffState();
          return runnable::ledc::ELedState::LED_OFF;
        }
      } else {
        // here may be the place for proximity indication logic
        if (isKeepAliveCounterActiveProximity()) {
          m_storage.m_keepAliveCounterValue++;
          if (m_storage.m_keepAliveCounterValue >= G_LLF_DURATION_CYCLES) {
            endKeepAliveCounter();
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          } else {
            m_HWDelegate.setLEDProxAnim();
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          }
        } else {
          if (isTICInteractionProximity()) {
            startKeepAliveCounterProx();
            m_HWDelegate.setLEDProxAnim();
            return runnable::ledc::ELedState::LED_PROXIMITY_INDICATION;
          } else {
            m_HWDelegate.setLEDOffState();
            return runnable::ledc::ELedState::LED_OFF;
          }
        }
      }
    }
  }

  bool hasLLCLevelChanged() {
    if ((m_storage.m_prevLLCState != m_input.m_llcOutData.m_lightState) ||
        (m_storage.m_prevLLCLevel != m_input.m_llcOutData.m_dimLevel)) {
      return true;
    }
    return false;
  }

  bool isTICInteractionProximity() {
    if (m_input.m_ticOutData.m_touchInteraction ==
        runnable::tic::ETouchInteraction::PROXIMITY_DETECTED) {
      return true;
    }
    return false;
  }

  bool isKeepAliveCounterActive() {
    return (m_storage.m_keepAliveCounterState ==
            runnable::ledc::storage::EKeepAliveCounterState::ACTIVE);
  }
  bool isKeepAliveCounterActiveProximity() {
    return (m_storage.m_keepAliveCounterState ==
            runnable::ledc::storage::EKeepAliveCounterState::ACTIVE_PROX);
  }

  bool isLLCFuncStatePermissible() {
    switch (m_input.m_llcOutData.m_funcState) {
      case runnable::llc::ELLCState::ACTIVE:
        return true;
        break;
      case runnable::llc::ELLCState::INACTIVE:
        return false;
        break;
      case runnable::llc::ELLCState::DEGRADED:
        return false;
        break;
    }
    return false;
  }

  uint8_t dimLevelToLLFLeds(uint8_t f_dimLvl) {
    if (f_dimLvl == 0) {
      return 0U;
    } else if (f_dimLvl >= 1 && f_dimLvl <= 10) {
      return 1U;
    } else if (f_dimLvl >= 11 && f_dimLvl <= 21) {
      return 2U;
    } else if (f_dimLvl >= 22 && f_dimLvl <= 32) {
      return 3U;
    } else if (f_dimLvl >= 33 && f_dimLvl <= 43) {
      return 4U;
    } else if (f_dimLvl >= 44 && f_dimLvl <= 54) {
      return 5U;
    } else if (f_dimLvl >= 55 && f_dimLvl <= 65) {
      return 6U;
    } else if (f_dimLvl >= 66 && f_dimLvl <= 76) {
      return 7U;
    } else if (f_dimLvl >= 77 && f_dimLvl <= 87) {
      return 8U;
    } else if (f_dimLvl >= 88 && f_dimLvl <= 99) {
      return 9U;
    } else if (f_dimLvl >= 100) {
      return 10U;
    }
    return 0U;
  }

  void startKeepAliveCounter() {
    m_storage.m_keepAliveCounterState =
        runnable::ledc::storage::EKeepAliveCounterState::ACTIVE;
    m_storage.m_keepAliveCounterValue = 0U;
  }

  void startKeepAliveCounterProx() {
    m_storage.m_keepAliveCounterState =
        runnable::ledc::storage::EKeepAliveCounterState::ACTIVE_PROX;
    m_storage.m_keepAliveCounterValue = 0U;
  }

  void endKeepAliveCounter() {
    m_storage.m_keepAliveCounterState =
        runnable::ledc::storage::EKeepAliveCounterState::INACTIVE;
    m_storage.m_keepAliveCounterValue = 0U;
  }

  runnable::ledc::input::CLedcInputData& m_input;
  runnable::ledc::CLedcOutput& m_output;
  hw::CHWDelegate m_HWDelegate;
  storage::CLedcStorageData m_storage;
};
}  // namespace algo
}  // namespace ledc
}  // namespace runnable
