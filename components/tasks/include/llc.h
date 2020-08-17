#pragma once

#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "llcRunnable.h"
#include <iostream>

namespace tasks
{
    namespace llc
    {
        static const std::string G_TASK_TAG("[TASK::LLC]");

        class LLCTask : public smooth::core::Task
        {
        public:
            LLCTask() : smooth::core::Task("TASK::LLC", 9000, smooth::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{20})
            {
            }

            void init() override
            {
                Log::info(G_TASK_TAG, "LLC task init");
            }

            void tick() override
            {
                Log::info(G_TASK_TAG, "LLC task tick");
            }
        };
    } // namespace llc
} // namespace tasks