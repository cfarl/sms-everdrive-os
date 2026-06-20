SMS Everdrive source
====================

This project is a fork of the source code used to generate the V1 Krikzz Master System everdrive firmware

The original source code can be found here: https://github.com/smspower/sms-everdrive-os

The last compiled source code is the "MSOS.smo file" at the dist folder

Important: This firmware works in the chinese everdrive sold in Ali Express.

------------------------------------------------------------------------------------
Source Code Compiling
------------------------------------------------------------------------------------

1) Install SDCC 3.3 in the c:\SDCC-3.3 folder

- Download it here: https://sourceforge.net/projects/sdcc/files/sdcc-win64/3.3.0/

2) Tools in folder "everdrive-sdk":

- They where obtained from the "mega-ed-sdk.7z" file at https://krikzz.com/pub/support/mega-everdrive/v1/dev/

3) Compile the source code

- Run "_build.bat" file

4) Get the "MSOS.smo" file and put it in the everdrive sd card (this is the firmware)

5) Run the "MSOS.smo" file in the everdrive to update the firmware