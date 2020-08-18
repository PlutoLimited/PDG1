#pragma once

#include <iostream>

namespace runnable
{
    namespace llc
    {

        class llc_output
        {
        public:
            llc_output() : m_int(0U), m_string(""){};
            //   ~llc_output() {}
            uint64_t m_int = 0U;
            std::string m_string = "";
        };

    } // namespace llc
} // namespace runnable
