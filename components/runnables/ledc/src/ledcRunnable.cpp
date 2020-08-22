#include "ledcRunnable.h"

namespace runnable {
namespace ledc {

void CLEDCRunnable::init() {}

void CLEDCRunnable::run() {
  collectInput();
  doWork();
  sendOutput();
}

void CLEDCRunnable::attachInputPorts(llcPort_p f_llcInputPort,
                                     ticPort_p f_ticInputPort) {
  Log::info(G_TASK_TAG, "Attaching input ports");
  m_llcInputPort = f_llcInputPort;
  m_ticInputPort = f_ticInputPort;
}

void CLEDCRunnable::attachOutputPorts(ledcPort_p f_ledcOutputPort) {
  Log::info(G_TASK_TAG, "Attaching output ports");
  m_ledcOutputPort = f_ledcOutputPort;
}

void CLEDCRunnable::collectInput() {}

void CLEDCRunnable::doWork() {}

void CLEDCRunnable::sendOutput() { m_ledcOutputPort->setData(m_output); }

}  // namespace ledc
}  // namespace runnable