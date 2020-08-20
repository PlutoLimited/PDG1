#include "llcRunnable.h"

namespace runnable {
namespace llc {

void CLLCRunnable::init() {
  // configure gpio output ports
}

void CLLCRunnable::run() {
  if (!m_ticInputPort->hasAlloc()) {
    Log::error(G_TASK_TAG, "TIC Input Port is nullptr");
    return;
  }
  collectInput();
  doWork();
  sendOutput();
}

void CLLCRunnable::attachInputPorts(ticPort_p f_ticInputPort) {
  Log::info(G_TASK_TAG, "Attaching input ports");
  m_ticInputPort = f_ticInputPort;
}

void CLLCRunnable::attachOutputPorts(llcPort_p f_llcOutputPort) {
  Log::info(G_TASK_TAG, "Attaching output ports");
  m_llcOutputPort = f_llcOutputPort;
}

void CLLCRunnable::collectInput() {
  m_inputData = *(m_ticInputPort->getData());
  CLlcOutput defaultOut;
  m_output = defaultOut;
}

void CLLCRunnable::doWork() {
  // handle tic function state
  switch (m_inputData.m_funcState) {
    case runnable::tic::ETICState::INACTIVE:
      // set HW safe state
      // set output safe state
      m_output.m_funcState = ELLCState::ACTIVE;
      m_output.m_lightState = ELightState::LIGHT_OFF;
      m_output.m_dimLevel = 0U;
      break;

    case runnable::tic::ETICState::ACTIVE:
      // set llc output
      if (m_inputData.m_deviceState ==
          runnable::tic::EDeviceState::DEVICE_ACTIVE) {
        if (m_inputData.m_touchInteraction ==
                runnable::tic::ETouchInteraction::SLIDING_DETECTED ||
            m_inputData.m_touchInteraction ==
                runnable::tic::ETouchInteraction::TOUCH_DETECTED) {
          // set a new light level
        } else {
          // keep previous output
          m_output.m_funcState = ELLCState::ACTIVE;
          m_output.m_lightState = m_prevOutput.m_lightState;
          m_output.m_dimLevel = m_prevOutput.m_dimLevel;
        }
      } else {
        // keep previous output
        m_output.m_funcState = ELLCState::ACTIVE;
        m_output.m_lightState = m_prevOutput.m_lightState;
        m_output.m_dimLevel = m_prevOutput.m_dimLevel;
      }
      break;

    case runnable::tic::ETICState::DEGRADED:
      // set llc safe state
      m_output.m_funcState = ELLCState::INACTIVE;
      m_output.m_lightState = ELightState::LIGHT_OFF;
      m_output.m_dimLevel = 0U;
      break;
  }
}

void CLLCRunnable::sendOutput() {
  m_prevOutput = m_output;
  m_llcOutputPort->setData(m_output);
}

}  // namespace llc
}  // namespace runnable