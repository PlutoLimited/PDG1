#pragma once

#include "output/dcm_output.h"
#include "output/tic_output.h"

namespace runnable {
namespace llc {

class CLLCInput {
 public:
  CLLCInput() : m_ticInput(), m_dcmInput() {}

  runnable::tic::CTicOutput m_ticInput;
  runnable::dcm::output::CDCMOutput m_dcmInput;
};

}  // namespace llc
}  // namespace runnable