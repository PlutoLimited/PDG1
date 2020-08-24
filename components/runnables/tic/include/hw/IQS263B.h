
#pragma once

#include "hw/IQS263DeviceData.h"
#include "smooth/core/io/Output.h"
#include "smooth/core/io/i2c/I2CMasterDevice.h"
#include "smooth/core/util/FixedBuffer.h"

namespace runnable {
namespace tic {
namespace hw {

static const std::string G_HW_TAG("[HW::TIC]");

class IQS263B : public smooth::core::io::i2c::I2CMasterDevice {
 public:
  IQS263B(i2c_port_t port, uint8_t address, std::mutex& guard);

  bool clr_reset_bit();

  bool configure_device();

  bool read_wheel_coordinates(CSliderCoordinateData& f_sliderDataOut);

  bool read_system_flags_events(CSystemFlagsEventsData& f_sysEventDataOut);

 private:
  // TODO: add rdy line logic for comms and reduce brute force comms attempts
  smooth::core::io::Output m_pinReadyOut;  // States: Current Drain (active low)
                                           // or high impedance (pull up)
};
}  // namespace hw
}  // namespace tic
}  // namespace runnable
