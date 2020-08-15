# PDG1

## Prerequisites

+ <https://github.com/espressif/esp-idf>

## Build

~~~bash
%userprofile%\esp\esp-idf\export.bat
~~~

or with Windows PowerShell

~~~bash
.$HOME/esp/esp-idf/export.ps1
~~~

Linux and macOS

~~~bash
. $HOME/esp/esp-idf/export.sh
~~~

~~~bash
idf.py set-target esp32
idf.py menuconfig
~~~

~~~bash
idf.py build
~~~

~~~bash
idf.py -p PORT [-b BAUD] flash
~~~

~~~bash
idf.py -p PORT monitor
~~~

Alternative Build

~~~bash
cd your_project_root
mkdir build && cd build
cmake .. -G "Ninja" -DESP_PLATFORM=1 -DCMAKE_TOOLCHAIN_FILE=$IDF_PATH/tools/cmake/toolchain-esp32.cmake && ninja
~~~
