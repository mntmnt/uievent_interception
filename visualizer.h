#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <vector>
#include <utility>
#include <QTimer>

#include <QtWinExtras>

#include <QDebug>

#include "raw_input.h"

class Widget : public QWidget {
    Q_OBJECT

public:

    devinfo_vec devices;
    std::vector<QLabel*> label_ptrs;
    QVBoxLayout * layout;
    QLabel * label;

    Widget() {
       resize(700, 400);
       show();

       if (QtWin::isCompositionEnabled()) {
               QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
               setAttribute(Qt::WA_TranslucentBackground, true);
               setAttribute(Qt::WA_NoSystemBackground, false);
               setStyleSheet("Widget { background: transparent; }");
       }

       layout = new QVBoxLayout(this);
       this->setLayout(layout);
    }

public slots:

    void onKeyboardEvent(void * handle,unsigned short vk, bool pressed) {
        std::size_t index = 0u;
        for(; index < devices.size(); ++index) {
           if(devices[index].handle == handle)
              break;
        }
        if(index < label_ptrs.size()) {
           QLabel * l = label_ptrs[index];
           l->setText(l->text() + QString::number(vk) + (pressed ? "PRESSED" : "RELEASED"));
        }

        if(handle == nullptr) {
           label->setText(label->text() + QString::number(vk) + (pressed ? "PRESSED" : "RELEASED"));
        }
    }


    void updateDeviceList(const devinfo_vec & list) {
       devices = list;

       setUpdatesEnabled(false);

       for(QLabel * lbl : label_ptrs) {
          delete lbl;
       };

       label_ptrs.clear();
       label_ptrs.reserve(list.size() + 1);

       label = new QLabel;
       layout->addWidget(label);
       label->setText("SENDINPUT KB");

       for(auto & dev : devices) {
           QLabel * label = new QLabel;
           label_ptrs.push_back(label);
           label->setText((dev.name) + "( " + dev.typeName + ")");
           label->show();
           label->setFrameStyle(QFrame::Box | QFrame::Raised);
           layout->addWidget(label);
       }

       setUpdatesEnabled(true);
    }

};
