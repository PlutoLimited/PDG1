#pragma once

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config/aws_config.h"
#include "delegate/aws/aws.h"
#include "input/dcm_input.h"
#include "output/dcm_output.h"
#include "output/tic_output.h"
#include "runnable.h"
#include "smooth/core/Application.h"

namespace runnable {
namespace dcm {
namespace handler {

static const std::string G_TASK_TAG_AWS("[RUN::DCM::HANDLER::AWS]");

class CAWSHandler {
 public:
  CAWSHandler(runnable::dcm::input::CDCMInput &f_inputData,
              runnable::dcm::output::CDCMOutput &f_outputData)
      : m_input(f_inputData), m_output(f_outputData) {}

  void init() {
    m_output.m_funcState = dcm::output::EDCMState::ACTIVE;
    m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;
  }

  void handle() {
    if (m_output.m_connState != output::EConnectionState::WIFI_CONNECTED) {
      m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;
      return;
    }
    m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;
  }

 private:
  runnable::dcm::input::CDCMInput &m_input;
  runnable::dcm::output::CDCMOutput &m_output;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
