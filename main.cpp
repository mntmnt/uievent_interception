#include <QApplication>

#include "input_catcher.h"

#include "visualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DeviceVisualizer wgt;
    wgt.show();

    InputCatcher ic;

    QObject::connect(&ic,
          SIGNAL(deviceNumChanged(const devinfo_vec &)),
          &wgt, SLOT(updateDeviceList(const devinfo_vec &))
          );

    QObject::connect(&ic,
          SIGNAL(keyboardEvent(DevHandle,unsigned short,bool)),
          &wgt, SLOT(onKeyboardEvent(DevHandle,unsigned short,bool))
          );

    QObject::connect(&ic,
          SIGNAL(mouseEvent(DevHandle,const ButtonStates &,WheelState)),
          &wgt, SLOT(onMouseEvent(DevHandle,const ButtonStates &,WheelState))
          );

    ic.initRI();




    return a.exec();
}




