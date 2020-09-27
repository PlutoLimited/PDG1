#pragma once

#include "hw/wifidelegate.h"
#include "input/dcm_input.h"
#include "output/dcm_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace dcm {
namespace handler {

static const std::string G_TASK_TAG("[RUN::DCM]");

class CWifiHandler {
 public:
  CWifiHandler(runnable::dcm::input::CDCMInput& f_inputData,
               runnable::dcm::output::CDCMOutput& f_outputData)
      : m_input(f_inputData),
        m_output(f_outputData),
        m_wifiDelegate()
  //  m_storage()
  {}

 private:
  runnable::dcm::input::CDCMInput& m_input;
  runnable::dcm::output::CDCMOutput& m_output;
  hw::CWifiDelegate m_wifiDelegate;
  // storage::CLedcStorageData m_storage;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
