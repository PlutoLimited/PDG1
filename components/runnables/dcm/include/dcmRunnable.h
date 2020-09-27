#pragma once

#include <iostream>

#include "handler/wifiHandler.h"
#include "input/dcm_input.h"
#include "memport.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace dcm {

static const std::string G_TASK_TAG("[RUN::DCM]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using dcmPort_p = tinymemport::TDataPort<runnable::dcm::output::CDCMOutput> *;

class CDCMRunnable : public CRunnable {
 public:
  CDCMRunnable()
      : m_ticInputPort(),
        m_dcmOutputPort(),
        m_input(),
        m_output(),
        m_handler(m_input, m_output) {}

  void init() override;
  void run() override;

  void attachInputPorts(ticPort_p f_ticInputPort);
  void attachOutputPorts(dcmPort_p f_dcmOutputPort);

 private:
  ticPort_p m_ticInputPort;
  dcmPort_p m_dcmOutputPort;

  runnable::dcm::input::CDCMInput m_input;
  runnable::dcm::output::CDCMOutput m_output;
  runnable::dcm::handler::CWifiHandler m_handler;

  void collectInput();
  void doWork();
  void sendOutput();
};
}  // namespace dcm
}  // namespace runnable
