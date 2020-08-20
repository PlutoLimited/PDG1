/*
Smooth - A C++ framework for embedded programming on top of Espressif's ESP-IDF
Copyright 2019 Per Malmberg (https://gitbub.com/PerMalmberg)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "App.h"

#include "smooth/core/SystemStatistics.h"
#include "smooth/core/Task.h"
#include "smooth/core/task_priorities.h"

namespace os {

void App::init() {
  Log::info(G_APP_TAG, "Init Application Tasks");

  m_LLCtask.attachInputDataPorts(&m_ticOutputData);
  m_LLCtask.attachOutputDataPorts(&m_llcOutputData);
  m_LLCtask.start();

  m_TICtask.attachInputDataPorts();
  m_TICtask.attachOutputDataPorts(&m_ticOutputData);
  m_TICtask.start();

  m_LEDCtask.attachInputDataPorts(&m_llcOutputData, &m_ticOutputData);
  m_LEDCtask.attachOutputDataPorts(&m_ledcOutputData);
  m_LEDCtask.start();
}

void App::tick() { smooth::core::SystemStatistics::instance().dump(); }
}  // namespace os
