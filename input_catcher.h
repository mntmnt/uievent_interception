#pragma once

#include <QLabel>
#include <QDebug>

#include "raw_input.h"

class MyLabel : public QLabel {
    Q_OBJECT
    typedef QByteArray NativeEventType;

    bool nativeEvent(const NativeEventType &, void * message, long * result)
    {
       return processRawInput(message, result);
    }
};
