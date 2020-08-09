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
