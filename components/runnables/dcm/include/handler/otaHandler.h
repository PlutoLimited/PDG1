#pragma once

#include "delegate/ota/otadelegate.h"
#include "input/dcm_input.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
namespace runnable {
namespace dcm {
namespace handler {

static const std::string G_TASK_TAG_OTA("[RUN::DCM::HANDLER::OTA]");

class COTAHandler {
 public:
  COTAHandler(runnable::dcm::input::CDCMInput& f_inputData,
              runnable::dcm::output::CDCMOutput& f_outputData)
      : m_input(f_inputData), m_output(f_outputData) {}

  void init() {
    m_output.m_funcState = dcm::output::EDCMState::ACTIVE;
    m_output.m_otaState = dcm::output::EOTAState::OTA_NOT_AVAILABLE;
  }

  void handle() {
    if (m_output.m_connState != output::EConnectionState::WIFI_CONNECTED) {
      m_output.m_otaState = dcm::output::EOTAState::OTA_NOT_AVAILABLE;
      return;
    }
    m_output.m_otaState = dcm::output::EOTAState::OTA_NOT_AVAILABLE;
  }

 private:
  runnable::dcm::input::CDCMInput& m_input;
  runnable::dcm::output::CDCMOutput& m_output;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
