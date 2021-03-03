#pragma once

#include "output/llc_output.h"
#include "output/tic_output.h"

namespace runnable {
namespace dcm {
namespace input {

class CDCMInput {
 public:
  CDCMInput() : m_ticData(), m_llcData(){};

  runnable::tic::CTicOutput m_ticData;
  runnable::llc::CLlcOutput m_llcData;
};

}  // namespace input
}  // namespace dcm
}  // namespace runnable
