#include "ticRunnable.h"

namespace runnable {
namespace tic {

void CTICRunnable::attachInputPorts() {
  Log::info(G_TASK_TAG, "Attaching input ports");
}

void CTICRunnable::attachOutputPorts(ticPort_p f_ticOutputPort) {
  Log::info(G_TASK_TAG, "Attaching output ports");
  m_ticOutputPort = f_ticOutputPort;
}

void CTICRunnable::init() { m_deviceHandler.configureDevice(); }

void CTICRunnable::run() {
  m_deviceHandler.handleDeviceAndSetOutput();
  sendOutput();
}

void CTICRunnable::sendOutput() { m_ticOutputPort->setData(m_output); }

}  // namespace tic
}  // namespace runnable