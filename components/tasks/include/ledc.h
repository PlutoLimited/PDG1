#pragma once

#include <iostream>

#include "ledcRunnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"

namespace tasks {
namespace ledc {
static const std::string G_TASK_TAG("[TASK::LEDC]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using llcPort_p = tinymemport::TDataPort<runnable::llc::CLlcOutput> *;
using dcmPort_p = tinymemport::TDataPort<runnable::dcm::output::CDCMOutput> *;
using ledcPort_p = tinymemport::TDataPort<runnable::ledc::CLedcOutput> *;

class LEDCTask : public smooth::core::Task {
 public:
  explicit LEDCTask()
      : smooth::core::Task("TASK::LEDC", 9000,
                           smooth::core::APPLICATION_BASE_PRIO,
                           std::chrono::milliseconds{20}),
        m_ticInputPort(),
        m_llcInputPort(),
        m_dcmInputPort(),
        m_ledcOutputPort(),
        m_ledcRunnable(),
        m_logCounter(0U) {}

  void attachInputDataPorts(llcPort_p f_llcInputPort, ticPort_p f_ticInputPort,
                            dcmPort_p f_dcmInputPort) {
    m_llcInputPort = f_llcInputPort;
    m_ticInputPort = f_ticInputPort;
    m_dcmInputPort = f_dcmInputPort;
  }

  void attachOutputDataPorts(ledcPort_p f_ledcOutputPort) {
    m_ledcOutputPort = f_ledcOutputPort;
  }

  void init() override {
    Log::info(G_TASK_TAG, "LEDC task init");
    m_ledcRunnable.attachInputPorts(m_llcInputPort, m_ticInputPort,
                                    m_dcmInputPort);
    m_ledcRunnable.attachOutputPorts(m_ledcOutputPort);
  }

  void tick() override {
    m_ledcRunnable.run();
    m_logCounter++;
    if (m_logCounter % 100 == 0U) {
      printOutputData();
    }
  }

 private:
  ticPort_p m_ticInputPort;
  llcPort_p m_llcInputPort;
  dcmPort_p m_dcmInputPort;
  ledcPort_p m_ledcOutputPort;

  runnable::ledc::CLEDCRunnable m_ledcRunnable;

  uint8_t m_logCounter;

  void printOutputData() {
    const auto ledcOutput = *(m_ledcOutputPort->getData());
    constexpr const char *stack_format = "{:>16} | {:>10}";
    Log::info(G_TASK_TAG, stack_format, "Signal", "Value");
    Log::info(G_TASK_TAG, stack_format, "Function State",
              std::to_string(static_cast<uint8_t>(ledcOutput.m_funcState)));
    Log::info(G_TASK_TAG, stack_format, "LED State",
              std::to_string(static_cast<uint8_t>(ledcOutput.m_ledState)));
    Log::info(G_TASK_TAG, "");
  }
};
}  // namespace ledc
}  // namespace tasks
