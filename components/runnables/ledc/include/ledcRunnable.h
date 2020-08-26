#pragma once

#include "algo/VFHandler.h"
#include "hw/hwdelegate.h"
#include "input/ledc_input.h"
#include "memport.h"
#include "output/ledc_output.h"
#include "output/llc_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace ledc {
static const std::string G_TASK_TAG("[RUN::LEDC]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using llcPort_p = tinymemport::TDataPort<runnable::llc::CLlcOutput> *;
using ledcPort_p = tinymemport::TDataPort<runnable::ledc::CLedcOutput> *;
class CLEDCRunnable : public CRunnable {
 public:
  CLEDCRunnable()
      : m_ticInputPort(),
        m_llcInputPort(),
        m_ledcOutputPort(),
        m_output(),
        m_input(),
        m_vfHandler(m_input, m_output) {}

  void init() override;
  void run() override;

  void attachInputPorts(llcPort_p f_llcInputPort, ticPort_p f_ticInputPort);
  void attachOutputPorts(ledcPort_p f_ledcOutputPort);

 private:
  ticPort_p m_ticInputPort;
  llcPort_p m_llcInputPort;
  ledcPort_p m_ledcOutputPort;

  CLedcOutput m_output;
  input::CLedcInputData m_input;
  algo::CVFHandler m_vfHandler;

  void collectInput();
  void doWork();
  void sendOutput();
};
}  // namespace ledc
}  // namespace runnable