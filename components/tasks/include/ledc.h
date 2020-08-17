#pragma once

#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "ledcRunnable.h"
#include <iostream>

namespace tasks
{
    namespace ledc
    {
        static const std::string G_TASK_TAG("[TASK::LEDC]");

        class LEDCTask : public smooth::core::Task
        {
        public:
            LEDCTask() : smooth::core::Task("TASK::LEDC", 9000, smooth::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{20})
            {
            }

            void init() override
            {
                Log::info(G_TASK_TAG, "LEDC task init");
            }

            void tick() override
            {
                Log::info(G_TASK_TAG, "Hello from LEDC task");
            }
        };
    } // namespace ledc
} // namespace tasks
