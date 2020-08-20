#pragma once

#include "hw/hwdelegate.h"
#include "memport.h"
#include "output/llc_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace llc {

static const std::string G_TASK_TAG("[RUN::LLC]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
using llcPort_p = tinymemport::TDataPort<runnable::llc::CLlcOutput> *;

class CLLCRunnable : public CRunnable {
 public:
  CLLCRunnable()
      : m_ticInputPort(),
        m_llcOutputPort(),
        m_inputData(),
        m_output(),
        m_prevOutput() {}

  void init() override;
  void run() override;

  void attachInputPorts(ticPort_p f_ticInputPort);
  void attachOutputPorts(llcPort_p f_llcOutputPort);

 private:
  ticPort_p m_ticInputPort;
  llcPort_p m_llcOutputPort;

  runnable::tic::CTicOutput m_inputData;

  runnable::llc::CLlcOutput m_output;
  runnable::llc::CLlcOutput m_prevOutput;
  runnable::llc::hw::CHWDelegate m_hwDelegate;

  void collectInput();
  void doWork();
  void sendOutput();
};
}  // namespace llc
}  // namespace runnable