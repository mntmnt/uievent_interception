#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QtWinExtras>


class MouseWidget : public QWidget {
    Q_OBJECT

    QLabel * label, * capture;


public:

    MouseWidget(QWidget * o):QWidget(o) {
        this->setLayout(new QHBoxLayout(this));
        label = new QLabel();
        this->layout()->addWidget(label);
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

        if (QtWin::isCompositionEnabled()) {
                QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
                setAttribute(Qt::WA_TranslucentBackground, true);
                setAttribute(Qt::WA_NoSystemBackground, false);
                setStyleSheet("MouseWidget { background: transparent; }");
        }
        auto l = new QVBoxLayout;
        label->setLayout(l);
        capture = new QLabel;
        capture->setText("NAME OF MOUSE");
        capture->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        l->addWidget(capture, 0, Qt::AlignTop);

        auto lbl = new QLabel;
        lbl->setText("SOMETHING TO SHOW");
        lbl->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        l->addWidget(lbl);
    }
};
