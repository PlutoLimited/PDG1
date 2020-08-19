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

#pragma once

#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "llc.h"
#include "tic.h"
#include "ledc.h"
#include "memport.h"
#include <iostream>
namespace os
{
    static const std::string G_APP_TAG("[OS]");

    using ticPort = tinymemport::TDataPort<runnable::tic::CTicOutput>;
    using llcPort = tinymemport::TDataPort<runnable::llc::CLlcOutput>;
    using ledcPort = tinymemport::TDataPort<runnable::ledc::CLedcOutput>;

    class App : public smooth::core::Application
    {
    public:
        App()
            : Application(smooth::core::APPLICATION_BASE_PRIO, std::chrono::seconds(20U)), m_llcOutputData(), m_ledcOutputData(), m_ticOutputData(), m_LLCtask(), m_TICtask(), m_LEDCtask()
        {
        }

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

} // namespace os
