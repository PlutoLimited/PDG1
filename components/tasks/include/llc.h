#pragma once

#include <iostream>

#include "dcmRunnable.h"
#include "ledcRunnable.h"
#include "llcRunnable.h"
#include "memport.h"
#include "output/llc_output.h"
#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "ticRunnable.h"

namespace tasks {
namespace llc {
static const std::string G_TASK_TAG("[TASK::LLC]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using llcPort_p = tinymemport::TDataPort<runnable::llc::CLlcOutput> *;
using ledcPort_p = tinymemport::TDataPort<runnable::ledc::CLedcOutput> *;
using dcmPort_p = tinymemport::TDataPort<runnable::dcm::output::CDCMOutput> *;

class LLCTask : public smooth::core::Task {
 public:
  explicit LLCTask()
      : smooth::core::Task("TASK::LLC", 9000,
                           smooth::core::APPLICATION_BASE_PRIO,
                           std::chrono::milliseconds{80}),
        m_ticInputPort(),
        m_dcmInputPort(),
        m_llcOutputPort(),
        m_llcRunnable(),
        m_logCounter(0) {}

  void attachInputDataPorts(ticPort_p f_ticInputPort, dcmPort_p f_dcmInputPort) {
    m_ticInputPort = f_ticInputPort;
    m_dcmInputPort = f_dcmInputPort;
  }

  void attachOutputDataPorts(llcPort_p f_llcOutputPort) {
    m_llcOutputPort = f_llcOutputPort;
  }

  void init() override {
    Log::info(G_TASK_TAG, "LLC task init");
    m_llcRunnable.attachInputPorts(m_ticInputPort, m_dcmInputPort);
    m_llcRunnable.attachOutputPorts(m_llcOutputPort);
    m_llcRunnable.init();
  }

  void tick() override {
    m_llcRunnable.run();
    m_logCounter++;
    if (m_logCounter % 100 == 0U) {
      printOutputData();
    }
  }

 private:
  ticPort_p m_ticInputPort;
  dcmPort_p m_dcmInputPort;
  llcPort_p m_llcOutputPort;

  runnable::llc::CLLCRunnable m_llcRunnable;
  uint8_t m_logCounter;

  void printOutputData() {
    const auto llcOutput = *(m_llcOutputPort->getData());

    constexpr const char *stack_format = "{:>16} | {:>10}";
    Log::info(G_TASK_TAG, stack_format, "Signal", "Value");
    Log::info(G_TASK_TAG, stack_format, "Function State",
              std::to_string(static_cast<uint8_t>(llcOutput.m_funcState)));
    Log::info(G_TASK_TAG, stack_format, "Light State",
              std::to_string(static_cast<uint8_t>(llcOutput.m_lightState)));

    Log::info(G_TASK_TAG, stack_format, "Dim Level",
              std::to_string(llcOutput.m_dimLevel));
    Log::info(G_TASK_TAG, "");
  }
};
}  // namespace llc
}  // namespace tasks