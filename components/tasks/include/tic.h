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

        class TICTask : public smooth::core::Task
        {
        public:
            TICTask() : smooth::core::Task("TASK::TIC", 9000, smooth::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{20})
            {
            }

            void init() override
            {
                Log::info(G_TASK_TAG, "TIC task init");
            }

            void tick() override
            {
            }
        };
    } // namespace tic
} // namespace tasks