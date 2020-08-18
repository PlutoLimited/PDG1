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
#include "output/llc_output.h"
#include "tic.h"
#include "ledc.h"
#include "memport.h"
#include <iostream>
namespace os
{
    static const std::string G_APP_TAG("[OS]");

    class App : public smooth::core::Application
    {
    public:
        App();

        void init() override;

        void tick() override;

    private:
        tinymemport::TDataPort<std::string> m_osOutputData;
        tinymemport::TDataPort<runnable::llc::llc_output> m_llcOutputData;
        tinymemport::TDataPort<std::string> m_ledcOutputData;
        tinymemport::TDataPort<std::string> m_ticOutputData;

        tasks::llc::LLCTask m_LLCtask;
        tasks::tic::TICTask m_TICtask;
        tasks::ledc::LEDCTask m_LEDCtask;
    };

} // namespace os
