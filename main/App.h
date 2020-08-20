
#pragma once

#include <iostream>

#include "ledc.h"
#include "llc.h"
#include "memport.h"
#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "tic.h"
namespace os {
static const std::string G_APP_TAG("[OS]");

using ticPort = tinymemport::TDataPort<runnable::tic::CTicOutput>;
using llcPort = tinymemport::TDataPort<runnable::llc::CLlcOutput>;
using ledcPort = tinymemport::TDataPort<runnable::ledc::CLedcOutput>;

class App : public smooth::core::Application {
 public:
  App()
      : Application(smooth::core::APPLICATION_BASE_PRIO,
                    std::chrono::seconds(20U)),
        m_llcOutputData(100U),
        m_ledcOutputData(100U),
        m_ticOutputData(100U),
        m_LLCtask(),
        m_TICtask(),
        m_LEDCtask() {}

  void init() override;

  void tick() override;

 private:
  llcPort m_llcOutputData;
  ledcPort m_ledcOutputData;
  ticPort m_ticOutputData;

  tasks::llc::LLCTask m_LLCtask;
  tasks::tic::TICTask m_TICtask;
  tasks::ledc::LEDCTask m_LEDCtask;
};

}  // namespace os
