# MAELab2019

16 January 2019

MAELab is a framework in image processing domain. 
It provides some common functions in image processing such as segmentation, binary operations, filter operations.
Besides, the most important function (and also the main function) of this framework is automatic predict the landmarks.
Currently, MAELab framework is distributed in 2 versions with and without Graphic User Interface (GUI).
With GUI version, the users can mostly use the functions of MAELab, while MAELab without GUI provides only the main function of framework.
This framework is implemented on C++. It is using LibJpeg to decompress/compress the image.

## Requiremented libraries:
- Qt4 (for GUI)

## Build program
The main function to run program GUI mode is written in MAELab_CI.cpp <br/>
The main function to run program without GUI mode is written in MAELab_OI.cpp <br/>

So, if you would like to change between two modes, you should change MAELab_CI.pro file and rebuilt the program. <br/>

Modify <b>MAELab_CI.pro</b> file at the end of line 16 (Change MAELab_OI.cpp <-> MAELab_CI.cpp) <br/>

Build program <br/>
qmake <br/>
make all <br/>

Run program <br/>
./MAELab_CI <br/>


