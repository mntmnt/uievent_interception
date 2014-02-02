#pragma once

#include <QLabel>
#include <QDebug>

#include "raw_input.h"

class MyLabel : public QLabel {
    Q_OBJECT

    bool nativeEvent(const QByteArray & eventType, void * message, long * result)
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        qDebug() << "MYHWND xx " << msg->hwnd;

        switch (msg->message) {
            case WM_INPUT:
                qDebug() << "WM_INPUT";
                return true;
        };
        return false;
    }
};
