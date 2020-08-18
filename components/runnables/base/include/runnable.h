#pragma once

namespace runnable
{

    class CRunnable
    {
    public:
        CRunnable()
        {
        }

        virtual void init() = 0;
        virtual void run() = 0;
    };
} // namespace runnable
