#pragma once

#include "hw/hwconfig.h"
#include "hw/hwdelegate.h"
#include "memport.h"
#include "output/llc_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"
#include "storage/storage.h"

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
        m_inputDataTIC(),
        m_storage(),
        m_output(),
        m_prevOutput(),
        m_hwDelegate() {}

  void init() override;
  void run() override;

  void attachInputPorts(ticPort_p f_ticInputPort);
  void attachOutputPorts(llcPort_p f_llcOutputPort);

 private:
  ticPort_p m_ticInputPort;
  llcPort_p m_llcOutputPort;

  runnable::tic::CTicOutput m_inputDataTIC;

  runnable::llc::storage::CStorage m_storage;
  runnable::llc::CLlcOutput m_output;
  runnable::llc::CLlcOutput m_prevOutput;
  runnable::llc::hw::CHWDelegate m_hwDelegate;

  void collectInput();
  void doWork();
  void sendOutput();
  void updateStorage();
  void setSafeStateOutput();
  void setNewSetpointFromSlider();
  ELightState setpointToLightState(const uint8_t f_setpoint);
  uint8_t convertSliderCoordToLLCSetpoint(const uint8_t f_sliderCoord);
  uint8_t convertDimCoordToLLCSetpoint(const uint8_t f_dimCoord);
};
}  // namespace llc
}  // namespace runnable