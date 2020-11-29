#pragma once

#include <iostream>

#include "memport.h"
#include "output/dcm_output.h"
#include "output/llc_output.h"
#include "output/tic_output.h"

namespace runnable {
namespace ledc {
namespace input {

class CLedcInputData {
 public:
  CLedcInputData() : m_llcOutData(), m_ticOutData(), m_dcmOutData(){};

  runnable::llc::CLlcOutput m_llcOutData;
  runnable::tic::CTicOutput m_ticOutData;
  runnable::dcm::output::CDCMOutput m_dcmOutData;
};

}  // namespace input
}  // namespace ledc
}  // namespace runnable
