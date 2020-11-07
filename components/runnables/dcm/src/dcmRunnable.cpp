
#include "dcmRunnable.h"

namespace runnable {
namespace dcm {

void CDCMRunnable::init() {}

void CDCMRunnable::run() {
  collectInput();
  doWork();
  sendOutput();
}

void CDCMRunnable::attachInputPorts(ticPort_p f_ticInputPort) {
  Log::info(G_TASK_TAG, "Attaching input ports");
  m_ticInputPort = f_ticInputPort;
}

void CDCMRunnable::attachWifi(wifiInstance_p f_wifi) {
  Log::info(G_TASK_TAG, "Attaching wifi instance");
  m_wifi_p = f_wifi;
}

void CDCMRunnable::attachOutputPorts(dcmPort_p f_dcmOutputPort) {
  Log::info(G_TASK_TAG, "Attaching output ports");
  m_dcmOutputPort = f_dcmOutputPort;
}

void CDCMRunnable::collectInput() {
  m_input.m_ticData = *(m_ticInputPort->getData());
}

void CDCMRunnable::doWork() { m_handler.handle(); }

void CDCMRunnable::sendOutput() { m_dcmOutputPort->setData(m_output); }

}  // namespace dcm
}  // namespace runnable
