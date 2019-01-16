# MAELab2019

16 January 2019

MAELab is a framework in image processing domain.  <br/>
It provides some common functions in image processing such as segmentation, binary operations, filter operations. <br/>
Besides, the most important function (and also the main function) of this framework is automatic predict the landmarks. <br/>
Currently, MAELab framework is distributed in 2 versions with and without Graphic User Interface (GUI). <br/>
With GUI version, the users can mostly use the functions of MAELab, while MAELab without GUI provides only the main function of framework. <br/>
This framework is implemented on C++. It is using LibJpeg to decompress/compress the image. <br/>

## Requiremented libraries:
- Qt4 (for GUI)

## Build and run the program
The main function to run program GUI mode is written in MAELab_CI.cpp <br/>
The main function to run program without GUI mode is written in MAELab_OI.cpp <br/>

So, if you would like to change between two modes, you should change MAELab_CI.pro file and rebuilt the program. <br/>

Modify <b>MAELab_CI.pro</b> file at the end of line 16 (Change MAELab_OI.cpp <-> MAELab_CI.cpp) <br/>

<b>Build program</b> <br/>
qmake <br/>
make all <br/>

<b>Run program </b><br/>
./MAELab_CI <br/>

## More detail:
You can find more details about MAELab at [MorphoBoid](http://morphoboid.labri.fr/devmap.html) website.
## Contact
Email: van-linh.le@u-bordeaux.fr/ linhlevandlu@gmail.com
