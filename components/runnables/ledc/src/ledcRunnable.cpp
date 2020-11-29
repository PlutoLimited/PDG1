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
                                     ticPort_p f_ticInputPort,
                                     dcmPort_p f_dcmInputPort) {
  Log::info(G_TASK_TAG, "Attaching input ports");
  m_llcInputPort = f_llcInputPort;
  m_ticInputPort = f_ticInputPort;
  m_dcmInputPort = f_dcmInputPort;
}

void CLEDCRunnable::attachOutputPorts(ledcPort_p f_ledcOutputPort) {
  Log::info(G_TASK_TAG, "Attaching output ports");
  m_ledcOutputPort = f_ledcOutputPort;
}

void CLEDCRunnable::collectInput() {
  m_input.m_llcOutData = *(m_llcInputPort->getData());
  m_input.m_ticOutData = *(m_ticInputPort->getData());
  m_input.m_dcmOutData = *(m_dcmInputPort->getData());
}

void CLEDCRunnable::doWork() {
  // handle lightlvl feedback
  m_vfHandler.handleLightLvlFeedback();
  // handle device status feedback
  m_vfHandler.handleDeviceStatusFeedback();
}

void CLEDCRunnable::sendOutput() { m_ledcOutputPort->setData(m_output); }

}  // namespace ledc
}  // namespace runnable