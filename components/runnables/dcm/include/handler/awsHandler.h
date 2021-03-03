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
  CAWSHandler(const runnable::dcm::input::CDCMInput &f_inputData,
              runnable::dcm::output::CDCMOutput &f_outputData)
      : m_input(f_inputData),
        m_output(f_outputData),
        m_AWSdelegate(),
        m_AWSinitialized(false),
        m_lastReportedLightState(0U) {}

  void init() {
    m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;
  }

  void handle() {
    if (m_output.m_connState != output::EConnectionState::WIFI_CONNECTED) {
      m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;
      return;
    }

    // wifi is connected, ensure active aws connection
    m_output.m_awsState = dcm::output::EAWSState::AWS_NOT_CONNECTED;

    if (!m_AWSinitialized) {
      m_AWSinitialized = m_AWSdelegate.initAWS();
      return;
    }

    m_output.m_awsState = dcm::output::EAWSState::AWS_CONNECTED;
    if (m_input.m_llcData.m_funcState == llc::ELLCState::ACTIVE) {
      if (m_input.m_llcData.m_dimLevel != m_lastReportedLightState) {
        m_AWSdelegate.updateLightLevel(m_input.m_llcData.m_dimLevel);
      }
    } else {
      m_AWSdelegate.updateLightLevel(m_input.m_llcData.m_dimLevel);
    }
    m_lastReportedLightState = m_input.m_llcData.m_dimLevel;

    m_AWSdelegate.runAWS();

    m_output.m_awsDesiredLightLvl = output::EAWSDesiredLightState::NOT_DESIRED;

    if (m_AWSdelegate.getDesiredLightLevel() != m_lastReportedLightState) {
      m_output.m_awsDesiredLightLvl =
          output::EAWSDesiredLightState::NEW_LVL_DESIRED;
      m_output.m_desiredLvl = m_AWSdelegate.getDesiredLightLevel();
    }
  }

 private:
  const runnable::dcm::input::CDCMInput &m_input;
  runnable::dcm::output::CDCMOutput &m_output;
  runnable::dcm::aws::CAWSDelegate m_AWSdelegate;
  bool m_AWSinitialized;
  uint8_t m_lastReportedLightState;
};
}  // namespace handler
}  // namespace dcm
}  // namespace runnable
