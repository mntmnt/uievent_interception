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

        std::size_t index = 0u;
        for(; index < devices.size(); ++index) {
           if(devices[index].handle == handle)
              break;
        }
        if(index < label_ptrs.size()) {
           auto * l = label_ptrs[index];
           KeyboardWidget * wgt = dynamic_cast<KeyboardWidget*>(l);
           if(!pressed)
              wgt->incClick();
        }

        if(handle == nullptr) {
           if(!pressed)
              dynamic_cast<KeyboardWidget*>(kb)->incClick();
        }
    }

    void onMouseEvent(DevHandle handle, const ButtonStates & bs,WheelState ws) {
        std::size_t index = 0u;
        for(; index < devices.size(); ++index) {
           if(devices[index].handle == handle)
              break;
        }
        MouseWidget * wgt;

        if(index < label_ptrs.size()) {
           wgt = dynamic_cast<MouseWidget*>(label_ptrs[index]);
        } else if(handle == nullptr) {
           wgt = dynamic_cast<MouseWidget*>(mouse);
        } else {
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
