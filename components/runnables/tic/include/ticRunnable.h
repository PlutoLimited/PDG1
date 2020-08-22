#pragma once

#include "hw/IQS263DeviceHandler.h"
#include "memport.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "smooth/core/io/i2c/Master.h"

namespace runnable {
namespace tic {
static const std::string G_TASK_TAG("[RUN::TIC]");

using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;

class CTICRunnable : public CRunnable {
 public:
  CTICRunnable()
      : m_deviceInitialized(false),
        m_ticOutputPort(),
        m_output(),
        m_deviceHandler(m_output) {}

  void init() override;
  void run() override;

  void attachInputPorts();
  void attachOutputPorts(ticPort_p f_ticOutputPort);

 private:
  void sendOutput();
  bool m_deviceInitialized;

  ticPort_p m_ticOutputPort;
  CTicOutput m_output;
  hw::CIQS263DeviceHandler m_deviceHandler;
};
}  // namespace tic
}  // namespace runnable