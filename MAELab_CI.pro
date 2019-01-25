TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "/usr/local/include"
LIBS += -L"/usr/lib" \
	

QT           += opengl
CONFIG       += console
#RESOURCES     = Ipm.qrc
QMAKE_CXXFLAGS += -fPIC -lpthread
QMAKE_CFLAGS += -fPIC -lpthread

HEADERS += io/*.h io/LibJpeg/*.h imageModel/*.h ui/*.h utils/*.h segmentation/*.h correlation/*.h histograms/*.h pht/*.h pointInterest/*.h MAELab.h
	
SOURCES += io/*.cpp io/LibJpeg/*.c imageModel/*.cpp ui/*.cpp utils/*.cpp segmentation/*.cpp correlation/*.cpp histograms/*.cpp pht/*.cpp pointInterest/*.cpp MAELab.cpp MAELab_CI.cpp

