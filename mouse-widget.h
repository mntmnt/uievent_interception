#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>

#include <QtWinExtras>
#include <QLabel>
#include <QPixmap>

#include <QTimer>

class MouseWidget : public QWidget {
   Q_OBJECT

    QPixmap mouse, arrows;

public:
   MouseWidget(): mouse(":/imgs/mouse.png"),
   arrows(":/imgs/arrows.png") {
      setLayout(new QVBoxLayout(this));
      auto gb = new QGroupBox(this);
      this->layout()->addWidget(gb);
      gb->setTitle("MOUSE BOX");
      gb->setEnabled(false);
      gb->setLayout(new QVBoxLayout);

      if (QtWin::isCompositionEnabled()) {
               QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
               setAttribute(Qt::WA_TranslucentBackground, true);
               setAttribute(Qt::WA_NoSystemBackground, false);
               setStyleSheet("MouseWidget { background: transparent; }");
       }

       auto label = new QLabel;

       label->setPixmap(mouse.scaled(mouse.size()*2));
       gb->layout()->addWidget(label);
   }

public slots:

   void onMouseMove() {


   }

};
