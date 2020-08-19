#pragma once

#include <iostream>

namespace runnable
{
    namespace tic
    {
        enum class ETICState
        {
            INACTIVE = 0,
            ACTIVE,
            DEGRADED
        };

        enum class EDeviceState
        {
            DEVICE_NO_COMM = 0,
            DEVICE_ACTIVE,
            DEVICE_INIT_ROUTINE
        };

        enum class ETouchInteraction
        {
            NO_INTERACTION = 0,
            PROXIMITY_DETECTED,
            SLIDING_DETECTED,
            TOUCH_DETECTED,
            LONG_TOUCH_DETECTED_SETUP
        };

        class CTicOutput
        {
        public:
            CTicOutput() : m_funcState(ETICState::INACTIVE), m_deviceState(EDeviceState::DEVICE_NO_COMM), m_touchInteraction(ETouchInteraction::NO_INTERACTION), m_sliderLevel(0U){};
            ETICState m_funcState;
            EDeviceState m_deviceState;
            ETouchInteraction m_touchInteraction;
            uint8_t m_sliderLevel;
        };

    } // namespace tic
} // namespace runnable
