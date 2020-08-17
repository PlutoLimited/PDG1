#pragma once

#include "base/runnable.h"

namespace runnable
{
    namespace llc
    {

        class CLLCRunnable : public CRunnable
        {
        public:
            CLLCRunnable()
            {
            }

            void init() override {}
            void run() override {}
        };
    } // namespace llc
} // namespace runnable