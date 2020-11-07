#pragma once

#include <iostream>

#include "dcmRunnable.h"
#include "memport.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "smooth/core/Application.h"
#include "smooth/core/network/Wifi.h"
#include "smooth/core/task_priorities.h"

namespace tasks {
namespace dcm {

static const std::string G_TASK_TAG("[TASK::DCM]");

using dcmPort_p = tinymemport::TDataPort<runnable::dcm::output::CDCMOutput> *;
using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using wifiInstance_p = smooth::core::network::Wifi *;

class DCMTask : public smooth::core::Task {
 public:
  explicit DCMTask()
      : smooth::core::Task("TASK::DCM", 9000,
                           smooth::core::APPLICATION_BASE_PRIO,
                           std::chrono::milliseconds{500}),
        m_ticInputPort(),
        m_dcmOutputPort(),
        m_dcmRunnable(),
        m_logCounter(0) {}

  void attachInputDataPorts(ticPort_p f_ticInputPort) {
    m_ticInputPort = f_ticInputPort;
  }

  void attachWifi(wifiInstance_p f_wifi) { m_wifi = f_wifi; }

  void attachOutputDataPorts(dcmPort_p f_dcmOutputPort) {
    m_dcmOutputPort = f_dcmOutputPort;
  }

  void init() override {
    Log::info(G_TASK_TAG, "DCM task init");
    m_dcmRunnable.attachInputPorts(m_ticInputPort);
    m_dcmRunnable.attachOutputPorts(m_dcmOutputPort);
    m_dcmRunnable.init();
  }

  void tick() override {
    m_dcmRunnable.run();
    m_logCounter++;
    if (m_logCounter % 10 == 0U) {
      printOutputData();
    }
  }

 private:
  ticPort_p m_ticInputPort;
  dcmPort_p m_dcmOutputPort;
  wifiInstance_p m_wifi;

  runnable::dcm::CDCMRunnable m_dcmRunnable;
  uint8_t m_logCounter;

  void printOutputData() {
    const auto dcmOutput = *(m_dcmOutputPort->getData());
    constexpr const char *stack_format = "{:>16} | {:>10}";
    Log::info(G_TASK_TAG, stack_format, "Signal", "Value");
    Log::info(G_TASK_TAG, stack_format, "Function State",
              std::to_string(static_cast<uint8_t>(dcmOutput.m_funcState)));
    Log::info(G_TASK_TAG, stack_format, "DCM State",
              std::to_string(static_cast<uint8_t>(dcmOutput.m_connState)));
    Log::info(G_TASK_TAG, "");
  }
};
}  // namespace dcm
}  // namespace tasks
