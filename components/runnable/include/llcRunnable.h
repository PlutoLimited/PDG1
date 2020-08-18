#pragma once

#include "base/runnable.h"
#include "memport.h"
#include "smooth/core/Application.h"
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
                if (m_osInputPort->hasData())
                {
                    Log::info(G_TASK_TAG, "OS Input Port has data");
                }

                //run algo
                //set output
            }

            void attachInputPorts(tinymemport::TDataPort<std::string> *f_osInputPort, tinymemport::TDataPort<std::string> *f_ticInputPort)
            {
                Log::info(G_TASK_TAG, "Attaching input ports");
                m_osInputPort = f_osInputPort;
                m_ticInputPort = f_ticInputPort;
            }

            void attachOutputPorts(tinymemport::TDataPort<std::string> *f_llcOutputPort)
            {
                Log::info(G_TASK_TAG, "Attaching output ports");
                m_llcOutputPort = f_llcOutputPort;
            }

        private:
            tinymemport::TDataPort<std::string> *m_osInputPort;
            tinymemport::TDataPort<std::string> *m_ticInputPort;
            tinymemport::TDataPort<std::string> *m_llcOutputPort;
        };
    } // namespace llc
} // namespace runnable