/* Copyright Pluto Technologies - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 * Author: Oliver Altergott
 */

#include "App.h"

extern "C"
{
    void app_main()
    {
        os::App app;
        app.start();
    }
}
