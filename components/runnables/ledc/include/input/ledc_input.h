#pragma once

#include <iostream>

#include "memport.h"
#include "output/llc_output.h"
#include "output/tic_output.h"

namespace runnable {
namespace ledc {
namespace input {

class CLedcInputData {
 public:
  CLedcInputData() : m_llcOutData(), m_ticOutData(){};

  runnable::llc::CLlcOutput m_llcOutData;
  runnable::tic::CTicOutput m_ticOutData;
};

}  // namespace input
}  // namespace ledc
}  // namespace runnable
