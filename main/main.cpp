/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "App.h"
//#include "smooth/core/Application.h"
#include "smooth/core/SystemStatistics.h"


//using namespace app;

using namespace start;

extern "C"
{
#ifdef ESP_PLATFORM
void app_main()
{
    App app;
    app.start();
}
#else
    int main(int /*argc*/, char** /*argv*/)
    {
        smooth::core::SystemStatistics::instance().dump();
        App app{};
        app.start();
        return 0;
    }
#endif
    
}

