#include "llcRunnable.h"

namespace runnable
{
    namespace llc
    {

        void CLLCRunnable::init() {}

        void CLLCRunnable::run()
        {
            if (!m_ticInputPort->hasData())
            {
                Log::info(G_TASK_TAG, "TIC Input Port is nullptr");
                return;
            }
            collectInput();
            doWork();
            sendOutput();
        }

        void CLLCRunnable::attachInputPorts(ticPort_p f_ticInputPort)
        {
            Log::info(G_TASK_TAG, "Attaching input ports");
            m_ticInputPort = f_ticInputPort;
        }

        void CLLCRunnable::attachOutputPorts(llcPort_p f_llcOutputPort)
        {
            Log::info(G_TASK_TAG, "Attaching output ports");
            m_llcOutputPort = f_llcOutputPort;
        }

        void CLLCRunnable::collectInput()
        {
        }

        void CLLCRunnable::doWork()
        {
            //Log::info(G_TASK_TAG, "set: " + m_llcOutputPort->getData()->m_string);
        }

        void CLLCRunnable::sendOutput()
        {
            //m_llcOutputPort->setData(m_output);
        }

    } // namespace llc
} // namespace runnable