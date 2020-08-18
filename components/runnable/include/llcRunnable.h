#pragma once

#include "base/runnable.h"
#include "mempool.h"
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
                    Log::info(G_TASK_TAG, "OS Input Port HAS new data");
                }

                //run algo
                //set output
            }

            void attachInputPorts(tinydataportlib::TDataPort<std::string> *f_osInputPort, tinydataportlib::TDataPort<std::string> *f_ticInputPort)
            {
                Log::info(G_TASK_TAG, "Attaching input ports");
                m_osInputPort = f_osInputPort;
                m_ticInputPort = f_ticInputPort;
            }

            void attachOutputPorts(tinydataportlib::TDataPort<std::string> *f_llcOutputPort)
            {
                Log::info(G_TASK_TAG, "Attaching output ports");
                m_llcOutputPort = f_llcOutputPort;
            }

        private:
            tinydataportlib::TDataPort<std::string> *m_osInputPort;
            tinydataportlib::TDataPort<std::string> *m_ticInputPort;
            tinydataportlib::TDataPort<std::string> *m_llcOutputPort;
        };
    } // namespace llc
} // namespace runnable