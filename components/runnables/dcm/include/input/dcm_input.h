#pragma once

#include "output/tic_output.h"

namespace runnable {
namespace dcm {
namespace input {

class CDCMInput {
 public:
  CDCMInput() : m_ticData(){};

  runnable::tic::CTicOutput m_ticData;
};

}  // namespace input
}  // namespace dcm
}  // namespace runnable
