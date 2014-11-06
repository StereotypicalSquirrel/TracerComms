TracerComms
===========

Application to monitor an EP Solar Tracer MPPT Battery Charger

See https://www.john.geek.nz/category/projects/solar-power/ for info on how to physically connect up to the EP Solar charger.

It uses port /dev/ttyAMA0 at 9600 by default

Usage:
>./TracerComms --help
-----------------------------------------------------------------------------
|          EP Solar Tracer communicator v1.0                                |
-----------------------------------------------------------------------------
--port <port>
    0 = /dev/ttyS0       1 = /dev/ttyS1       2 = /dev/ttyS2   
    3 = /dev/ttyS3       4 = /dev/ttyS4       5 = /dev/ttyS5   
    6 = /dev/ttyS6       7 = /dev/ttyS7       8 = /dev/ttyS8   
    9 = /dev/ttyS9       10 = /dev/ttyS10     11 = /dev/ttyS11 
    12 = /dev/ttyS12     13 = /dev/ttyS13     14 = /dev/ttyS14 
    15 = /dev/ttyS15     16 = /dev/ttyUSB0    17 = /dev/ttyUSB1
    18 = /dev/ttyUSB2    19 = /dev/ttyUSB3    20 = /dev/ttyUSB4
    21 = /dev/ttyUSB5    22 = /dev/ttyAMA0    23 = /dev/ttyAMA1
    24 = /dev/ttyACM0    25 = /dev/ttyACM1    26 = /dev/rfcomm0
    27 = /dev/rfcomm1    28 = /dev/ircomm0    29 = /dev/ircomm1
--debug Do some debug stuff
--demo Fake a response from the Tracer
--verbose Talk more
--help Show this help
--baud <baudrate>
