#pragma once

#include <QLabel>
#include <QDebug>

#include "raw_input.h"

class InputCatcher : public QLabel {
    Q_OBJECT
    typedef QByteArray NativeEventType;

    bool nativeEvent(const NativeEventType &, void * message, long * result)
    {
       return processRawInput(message, result,
           [this](DevHandle h, unsigned short vk, bool pressed) {
              emit keyboardEvent(h, vk, pressed);
           },
           [this](DevHandle h, const ButtonStates & bs, WheelState ws) {
              emit mouseEvent(h, bs, ws);
           },
           [this](DevHandle, bool) {
              obtainDevices();
           }

       );
    }

    void obtainDevices() {
       auto deviced = getDevices();
        devinfo_vec v;
        v.reserve(deviced.size());
        for(RAWINPUTDEVICELIST & dev : deviced) {
           devinfo di;
           di.type = dev.dwType;
           di.typeName = devTypeStr(dev.dwType);
           di.name = getDeviceName(dev.hDevice);
           di.handle = dev.hDevice;
           di.rimType = getType(dev.dwType);
           v.push_back(di);
        }

        emit deviceNumChanged(v);
    }

public:
    InputCatcher() {
       setWindowTitle("Input Catcher WGT");
       resize(100,100);
       hide();
    }

    void initRI() {
        registerID((HWND)this->winId());

        obtainDevices();
    }


signals:

    void keyboardEvent(DevHandle h, unsigned short vk, bool pressed);
    void mouseEvent(DevHandle h, const ButtonStates & bs, WheelState ws);

    void deviceNumChanged(const devinfo_vec &);
};
