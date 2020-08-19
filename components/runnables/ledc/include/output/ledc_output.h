#pragma once

#include <iostream>

namespace runnable
{
    namespace ledc
    {
        enum class ELEDCState
        {
            INACTIVE = 0,
            ACTIVE,
            DEGRADED
        };

        enum class ELedState
        {
            LED_OFF = 0,
            LED_BOOT_ANIMATION,
            LED_PROXIMITY_INDICATION,
            LED_LIGHT_LVL_INDICATION,
            LED_DEVICE_SETUP_ACTIVE_INDICATION,
            LED_DEVICE_SETUP_DONE_SUCCESS_INDICATION,
            LED_DEVICE_SETUP_DONE_FAIL_INDICATION,
            LED_ERROR_INDICATION
        };

        class CLedcOutput
        {
        public:
            CLedcOutput() : m_funcState(ELEDCState::INACTIVE), m_ledState(ELedState::LED_OFF){};
            ELEDCState m_funcState;
            ELedState m_ledState;
        };

    } // namespace ledc
} // namespace runnable
