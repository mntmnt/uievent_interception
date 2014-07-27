#pragma once

#include <QWidget>
#include <QtWinExtras>

#include <QPen>
#include <QBrush>
#include <QHBoxLayout>
#include <QLabel>

#include <raw_input.h>

class DeviceWidget : public QWidget {
   Q_OBJECT
protected:
   QString name;
   void * handle;

public slots:
   void setName(QString str) { name = str; }

   void setHandle(void * h) {
      handle = h;
   }



public:
   DeviceWidget(QWidget * o): QWidget(o) {}

};

class MouseWidget : public DeviceWidget {
    Q_OBJECT

    QPen pen;
    QBrush brush;

    QPoint mirrorCoords(const QPoint &);

    QPoint adjustMirroredCoords(const QPoint &);

    QPoint adjustCoords(const QPoint &);

    void drawMouseBody();

    void drawLeftButton(bool down);

    void drawRightButton(bool down);

    void drawWheel(int dir);

    void drawMidButton(bool down);

    ButtonStates buttons;
    WheelState wheel;

public:

    void setButtonStates(const ButtonStates & st) {
       for(auto i = 0u; i < sizeof(buttons)/sizeof(*buttons); ++i) {
          buttons[i] = st[i];
       }
    }

    void setWheelState(const WheelState & st) {
       wheel = st;
    }


    MouseWidget(QWidget * o = nullptr): DeviceWidget(o) {
        if (QtWin::isCompositionEnabled() && o == nullptr) {
                QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
                setAttribute(Qt::WA_TranslucentBackground, true);
                setAttribute(Qt::WA_NoSystemBackground, false);
                setStyleSheet("MouseWidget { background: transparent; }");
        }

        for(auto & b: buttons) { b = KeyNeutral; };
        wheel = NeutralWheel;

        resize(80, 100);

        pen = QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
        brush = QBrush(Qt::gray, Qt::NoBrush);

    }

    QSize minimumSizeHint() const
    {
        return QSize(40, 50);
    }

    QSize sizeHint() const
    {
        return QSize(80, 100);
    }

    QSize maximumSize() const {
        return QSize(160, 200);
    }

    void paintEvent(QPaintEvent *) {
       drawMouseBody();

       drawLeftButton(buttons[MouseButtons::LeftKey] == ButtonState::KeyDown);
       drawMidButton(buttons[MouseButtons::MiddleKey] == ButtonState::KeyDown);
       drawRightButton(buttons[MouseButtons::RightKey] == ButtonState::KeyDown);

       drawWheel( static_cast<unsigned>(wheel)/WheelState::ForwardWheel);

       QPainter painter(this);

       painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
       painter.setBrush(QBrush(Qt::black, Qt::Dense5Pattern));

       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.drawText(QRect(10,10, size().width(), 40), name);

    }

};

class KeyboardWidget : public DeviceWidget {
   std::size_t clicks;
   QLabel * lbl;
public:
    KeyboardWidget(QWidget * o): DeviceWidget(o), clicks(0) {
        setLayout(new QHBoxLayout);
        layout()->addWidget(lbl = new QLabel(this));
    }

    void incClick() {
       clicks++;

       lbl->setText("<font color=red>[" + QString::number(clicks) + "]</font>");
    }
};
