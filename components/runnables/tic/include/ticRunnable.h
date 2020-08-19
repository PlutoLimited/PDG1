#pragma once

#include "runnable.h"
#include "memport.h"
#include "smooth/core/Application.h"
#include "output/tic_output.h"

namespace runnable
{
    namespace tic
    {
        static const std::string G_TASK_TAG("[RUN::TIC]");

        using ticPort_p = tinymemport::TDataPort<runnable::tic::CTicOutput> *;
        class CTICRunnable : public CRunnable
        {
        public:
            CTICRunnable() : m_ticOutputPort(), m_output()
            {
            }

            void init() override;
            void run() override;

            void attachInputPorts();
            void attachOutputPorts(ticPort_p f_ticOutputPort);

        private:
            ticPort_p m_ticOutputPort;

            CTicOutput m_output;

            void collectInput();
            void doWork();
            void sendOutput();
        };
    } // namespace tic
} // namespace runnable