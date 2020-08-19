#include "ticRunnable.h"

namespace runnable
{
    namespace tic
    {

        void CTICRunnable::init() {}

        void CTICRunnable::run()
        {
            collectInput();
            doWork();
            sendOutput();
        }

        void CTICRunnable::attachInputPorts()
        {
            Log::info(G_TASK_TAG, "Attaching input ports");
        }

        void CTICRunnable::attachOutputPorts(ticPort_p f_ticOutputPort)
        {
            Log::info(G_TASK_TAG, "Attaching output ports");
            m_ticOutputPort = f_ticOutputPort;
        }

        void CTICRunnable::collectInput()
        {
        }

        void CTICRunnable::doWork()
        {
        }

        void CTICRunnable::sendOutput()
        {
            m_ticOutputPort->setData(m_output);
        }

    } // namespace tic
} // namespace runnable