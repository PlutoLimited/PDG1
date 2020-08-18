#pragma once

#include "smooth/core/Application.h"
#include "smooth/core/task_priorities.h"
#include "llcRunnable.h"
#include "memport.h"
#include <iostream>

namespace tasks
{
    namespace llc
    {
        static const std::string G_TASK_TAG("[TASK::LLC]");

        class LLCTask : public smooth::core::Task
        {
        public:
            explicit LLCTask() : smooth::core::Task("TASK::LLC", 9000, smooth::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{20})
            {
            }

            void attachInputDataPorts(tinymemport::TDataPort<std::string> *f_osInputPort, tinymemport::TDataPort<std::string> *f_ticInputPort)
            {
                m_osInputPort = f_osInputPort;
                m_ticInputPort = f_ticInputPort;
            }

            void attachOutputDataPorts(tinymemport::TDataPort<std::string> *f_llcOutputPort)
            {
                m_llcOutputPort = f_llcOutputPort;
            }

            void init() override
            {
                Log::info(G_TASK_TAG, "LLC task init");
                m_llcRunnable.attachInputPorts(m_osInputPort, m_ticInputPort);
                m_llcRunnable.attachOutputPorts(m_llcOutputPort);
            }

            void tick() override
            {
                m_llcRunnable.run();
            }

        private:
            tinymemport::TDataPort<std::string> *m_osInputPort;
            tinymemport::TDataPort<std::string> *m_ticInputPort;
            tinymemport::TDataPort<std::string> *m_llcOutputPort;

            runnable::llc::CLLCRunnable m_llcRunnable;
        };
    } // namespace llc
} // namespace tasks