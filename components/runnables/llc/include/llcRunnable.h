#pragma once

#include "runnable.h"
#include "memport.h"
#include "smooth/core/Application.h"
#include "output/llc_output.h"

namespace runnable
{
    namespace llc
    {

        static const std::string G_TASK_TAG("[RUN::LLC]");
        class CLLCRunnable : public CRunnable
        {
        public:
            CLLCRunnable()
            {
            }

            void init() override {}
            void run() override
            {
                //collect input

                if (!m_osInputPort->hasData())
                {
                    Log::info(G_TASK_TAG, "OS Input Port is nullptr");
                    return;
                }
                m_output.m_int = m_output.m_int + 1;
                m_output.m_string = m_output.m_string;
                m_llcOutputPort->setData(m_output);
                //Log::info(G_TASK_TAG, "set: " + m_llcOutputPort->getData()->m_string);
                //run algo
                //set output
            }

            void attachInputPorts(tinymemport::TDataPort<std::string> *f_osInputPort, tinymemport::TDataPort<std::string> *f_ticInputPort)
            {
                Log::info(G_TASK_TAG, "Attaching input ports");
                m_osInputPort = f_osInputPort;
                m_ticInputPort = f_ticInputPort;
            }

            void attachOutputPorts(tinymemport::TDataPort<runnable::llc::llc_output> *f_llcOutputPort)
            {
                Log::info(G_TASK_TAG, "Attaching output ports");
                m_llcOutputPort = f_llcOutputPort;
            }

        private:
            tinymemport::TDataPort<std::string> *m_osInputPort;
            tinymemport::TDataPort<std::string> *m_ticInputPort;
            tinymemport::TDataPort<runnable::llc::llc_output> *m_llcOutputPort;

            runnable::llc::llc_output m_output;
            void collectInput();
            void doWork();
            void sendOutput();
        };
    } // namespace llc
} // namespace runnable