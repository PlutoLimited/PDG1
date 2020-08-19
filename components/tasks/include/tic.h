#pragma once

#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "ticRunnable.h"
#include <iostream>

namespace tasks
{
    namespace tic
    {
        static const std::string G_TASK_TAG("[TASK::TIC]");

        using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
        using llcPort_p = tinymemport::TDataPort<runnable::llc::CLlcOutput> *;
        using ledcPort_p = tinymemport::TDataPort<runnable::ledc::CLedcOutput> *;

        class TICTask : public smooth::core::Task
        {
        public:
            TICTask() : smooth::core::Task("TASK::TIC", 9000, smooth::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{20}), m_ticOutputPort(), m_ticRunnable()
            {
            }

            void attachInputDataPorts()
            {
            }

            void attachOutputDataPorts(ticPort_p f_ticOutputPort)
            {
                m_ticOutputPort = f_ticOutputPort;
            }
            void init() override
            {
                Log::info(G_TASK_TAG, "TIC task init");
                m_ticRunnable.attachInputPorts();
                m_ticRunnable.attachOutputPorts(m_ticOutputPort);
            }

            void tick() override
            {
                m_ticRunnable.run();
                m_logCounter++;
                if (m_logCounter % 100 == 0U)
                {
                    printOutputData();
                }
            }

        private:
            ticPort_p m_ticOutputPort;

            runnable::tic::CTICRunnable m_ticRunnable;

            uint8_t m_logCounter;

            void printOutputData()
            {
                const auto ticOutput = *(m_ticOutputPort->getData());

                constexpr const char *stack_format = "{:>16} | {:>10}";

                Log::info(G_TASK_TAG, stack_format, "Signal", "Value");
                Log::info(G_TASK_TAG,
                          stack_format,
                          "Function State",
                          std::to_string(static_cast<uint8_t>(ticOutput.m_funcState)));
                Log::info(G_TASK_TAG,
                          stack_format,
                          "Device State",
                          std::to_string(static_cast<uint8_t>(ticOutput.m_deviceState)));
                Log::info(G_TASK_TAG,
                          stack_format,
                          "Touch Interact.",
                          std::to_string(static_cast<uint8_t>(ticOutput.m_touchInteraction)));
                Log::info(G_TASK_TAG,
                          stack_format,
                          "Slider Level",
                          std::to_string(ticOutput.m_sliderLevel));

                Log::info(G_TASK_TAG, "");
            }
        };
    } // namespace tic
} // namespace tasks