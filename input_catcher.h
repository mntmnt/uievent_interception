#pragma once

#include <QLabel>
#include <QDebug>

#include "raw_input.h"

class MyLabel : public QLabel {
    Q_OBJECT
    typedef QByteArray NativeEventType;

    bool nativeEvent(const NativeEventType &, void * message, long * result)
    {
       return processRawInput(message, result,
           [this](void * h, unsigned short vk, bool pressed) {
              keyboardEvent(h, vk, pressed);
           }
       );
    }

public:

    void initRI() {
        registerID((HWND)this->winId());

        auto deviced = getDevices();
        devinfo_vec v;
        v.reserve(deviced.size());
        for(RAWINPUTDEVICELIST & dev : deviced) {
           devinfo di;
           di.type = dev.dwType;
           di.typeName = devTypeStr(dev.dwType);
           di.name = getDeviceName(dev.hDevice);
           di.handle = dev.hDevice;
           v.push_back(di);
        }

        emit deviceNumChanged(v);
    }


signals:

    void keyboardEvent(void *h, unsigned short vk, bool pressed);

    void deviceNumChanged(const devinfo_vec &);
};
