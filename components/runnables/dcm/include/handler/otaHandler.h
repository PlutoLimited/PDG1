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
  COTAHandler(const runnable::dcm::input::CDCMInput& f_inputData,
              runnable::dcm::output::CDCMOutput& f_outputData)
      : m_input(f_inputData), m_output(f_outputData), m_otaDelegate() {}

  void init() {
    m_output.m_otaState = dcm::output::EOTAState::OTA_NOT_AVAILABLE;
  }

  void handle() {
    m_output.m_otaState = dcm::output::EOTAState::OTA_NOT_AVAILABLE;
    if (m_output.m_connState != output::EConnectionState::WIFI_CONNECTED) {
      return;
    }

    // wifi connected, check for OTA
  }

 private:
  const runnable::dcm::input::CDCMInput& m_input;
  runnable::dcm::output::CDCMOutput& m_output;
  runnable::dcm::ota::COTADelegate m_otaDelegate;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
