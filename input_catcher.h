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
       qDebug() << "OBTAIN DEV";
        auto v = getAllDevices();

        emit deviceNumChanged(v);
    }

public:
    InputCatcher() {
       setWindowTitle("Input Catcher WGT");
       resize(100,100);
       hide();
    }

    void initRI() {
        registerID((void*)this->winId());

        obtainDevices();
    }


signals:

    void keyboardEvent(DevHandle h, unsigned short vk, bool pressed);
    void mouseEvent(DevHandle h, const ButtonStates & bs, WheelState ws);

    void deviceNumChanged(const devinfo_vec &);
};
