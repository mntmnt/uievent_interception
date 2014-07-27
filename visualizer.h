#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

#include <vector>
#include <utility>
#include <QTimer>

#include <QtWinExtras>

#include <QDebug>

#include "raw_input.h"

#include "mouse-widget.h"

class DeviceVisualizer : public QWidget {
    Q_OBJECT

public:

    devinfo_vec devices;
    std::vector<DeviceWidget *> label_ptrs;
    QHBoxLayout * layout;
    DeviceWidget * mouse, * kb;

    DeviceVisualizer():mouse(nullptr),kb(nullptr) {
       resize(800, 200);
       show();

       if (QtWin::isCompositionEnabled()) {
               QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
               setAttribute(Qt::WA_TranslucentBackground, true);
               setAttribute(Qt::WA_NoSystemBackground, false);
               setStyleSheet("DeviceVisualizer { background: transparent; }");
       }

       layout = new QHBoxLayout(this);
       this->setLayout(layout);
    }

public slots:

    void onKeyboardEvent(DevHandle handle,unsigned short, bool pressed) {
        KeyboardWidget * wgt = nullptr;

        if(handle == nullptr) {
            wgt = dynamic_cast<KeyboardWidget*>(mouse);
        } else {
            for(DeviceWidget * i : label_ptrs) {
               if(i->devhandle() == handle)
                 wgt = dynamic_cast<KeyboardWidget*>(i);
            }
        }
        if(!wgt) {
           qDebug() << "RECEIVED INPUT FOR UNKNOWN DEVICE";
           return;
        }

        if(!pressed)
              wgt->incClick();
    }

    void onMouseEvent(DevHandle handle, const ButtonStates & bs,WheelState ws) {
        MouseWidget * wgt = nullptr;

        if(handle == nullptr) {
            wgt = dynamic_cast<MouseWidget*>(mouse);
        } else {
            for(DeviceWidget * i : label_ptrs) {
               if(i->devhandle() == handle)
                 wgt = dynamic_cast<MouseWidget*>(i);
            }
        }
        if(!wgt) {
           qDebug() << "RECEIVED INPUT FOR UNKNOWN DEVICE";
           return;
        }

        wgt->setWheelState(ws);
        wgt->setButtonStates(bs);
        wgt->update();
    }


    void updateDeviceList(const devinfo_vec & list) {
       qDebug() << "UPDATE DEV LIST WGT";
       devices = list;

       setUpdatesEnabled(false);

       if(mouse)delete mouse;
       if(kb)delete kb;

       for(auto * lbl : label_ptrs) {
          delete lbl;
       };

       label_ptrs.clear();
       label_ptrs.reserve(list.size() + 1);

       mouse = new MouseWidget(this);
       layout->addWidget(mouse);
       mouse->setName("SENDINPUT Mouse");
       mouse->setHandle(nullptr);

       kb = new KeyboardWidget(this);
       layout->addWidget(kb);
       kb->setName("SENDINPUT Keyboard");
       kb->setHandle(nullptr);

       for(devinfo & dev : devices) {
         if(dev.rimType == RimDevType::RimMouse) {
           DeviceWidget * label = new MouseWidget(this);
           label_ptrs.push_back(label);
           label->setName(dev.name);
           label->setHandle( dev.handle );

           layout->addWidget(label);
         } else if(dev.rimType == RimDevType::RimKeyboard) {
           DeviceWidget * label = new KeyboardWidget(this);
           label_ptrs.push_back(label);
           label->setName(dev.name);
           label->setHandle( dev.handle );

           layout->addWidget(label);

         }
       }

       setUpdatesEnabled(true);
    }

};
