#pragma once

#include "runnable.h"

namespace runnable
{
    namespace ledc
    {

        class CLEDCRunnable : public CRunnable
        {
        public:
            CLEDCRunnable()
            {
            }

            void init() override {}
            void run() override {}
        };
    } // namespace ledc
} // namespace runnable