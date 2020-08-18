#pragma once

#include "runnable.h"

namespace runnable
{
    namespace tic
    {

        class CTICRunnable : public CRunnable
        {
        public:
            CTICRunnable()
            {
            }

            void init() override {}
            void run() override {}
        };
    } // namespace tic
} // namespace runnable