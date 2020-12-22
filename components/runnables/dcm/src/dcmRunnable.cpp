
#include "dcmRunnable.h"

namespace runnable {
namespace dcm {

void CDCMRunnable::init() {
  assert(m_ticInputPort != nullptr);
  assert(m_dcmOutputPort != nullptr);
  assert(m_wifi_p != nullptr);

  m_wifiHandler.init();
  m_AWSHandler.init();
  m_OTAHandler.init();
}

void CDCMRunnable::run() {
  if ((m_ticInputPort != nullptr) && (m_dcmOutputPort != nullptr) &&
      (m_wifi_p != nullptr)) {
    collectInput();
    doWork();
    sendOutput();
  }
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

void CDCMRunnable::doWork() {
  m_output = output::CDCMOutput();
  m_output.m_funcState = dcm::output::EDCMState::ACTIVE;
  m_wifiHandler.handle();
  m_AWSHandler.handle();
  m_OTAHandler.handle();
}

void CDCMRunnable::sendOutput() { m_dcmOutputPort->setData(m_output); }

}  // namespace dcm
}  // namespace runnable
