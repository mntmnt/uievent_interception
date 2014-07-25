#pragma once

#include <QWidget>
#include <QLabel>
#include <QtWinExtras>

#include <QPen>
#include <QBrush>

const QPoint MouseBodyBasePts[] = {
          {161, 364},   {43, 200},
          {43, 200},  {46, 98},
          {46, 98},   {117, 40},
          {117, 40},  {161, 40}
       };
const QPoint MouseBodyCtrlPts[] = {
          {59, 364},    {47, 241},
          {42, 197},{46, 116},
          {46, 66},   {92, 44},
          MouseBodyBasePts[6], MouseBodyBasePts[7]
       };

const QSize DesignedSize = {320, 400};

class MouseWidget : public QWidget {
    Q_OBJECT

    QPen pen;
    QBrush brush;

    QString mouseName;


    QPoint mirrorCoords(const QPoint & p) {
       return QPoint(DesignedSize.width() - p.x(), p.y());
    }

    QPoint adjustMirroredCoords(const QPoint & p) {
       return adjustCoords(mirrorCoords(p));
    }

    QPoint adjustCoords(const QPoint & p) {
       return QPoint(
             p.x() * size().width() / DesignedSize.width(),
             p.y() * size().height() /DesignedSize.height()
       );
    }

    void drawMouseBody() {
       QPainter painter(this);
       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.setBrush(QBrush(Qt::gray, Qt::NoBrush));
       painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

       QPainterPath mouse;

       mouse.moveTo(adjustCoords(MouseBodyBasePts[0]));
       mouse.cubicTo(adjustCoords(MouseBodyCtrlPts[0]), adjustCoords(MouseBodyCtrlPts[1]), adjustCoords(MouseBodyBasePts[1]));
       mouse.cubicTo(adjustCoords(MouseBodyCtrlPts[2]), adjustCoords(MouseBodyCtrlPts[3]), adjustCoords(MouseBodyBasePts[3]));
       mouse.cubicTo(adjustCoords(MouseBodyCtrlPts[4]), adjustCoords(MouseBodyCtrlPts[5]), adjustCoords(MouseBodyBasePts[5]));
       mouse.cubicTo(adjustCoords(MouseBodyCtrlPts[6]), adjustCoords(MouseBodyCtrlPts[7]), adjustCoords(MouseBodyBasePts[7]));

       mouse.cubicTo(adjustMirroredCoords(MouseBodyCtrlPts[7]), adjustMirroredCoords(MouseBodyCtrlPts[6]), adjustMirroredCoords(MouseBodyBasePts[5]));
       mouse.cubicTo(adjustMirroredCoords(MouseBodyCtrlPts[5]), adjustMirroredCoords(MouseBodyCtrlPts[4]), adjustMirroredCoords(MouseBodyBasePts[3]));
       mouse.cubicTo(adjustMirroredCoords(MouseBodyCtrlPts[3]), adjustMirroredCoords(MouseBodyCtrlPts[2]), adjustMirroredCoords(MouseBodyBasePts[1]));
       mouse.cubicTo(adjustMirroredCoords(MouseBodyCtrlPts[1]), adjustMirroredCoords(MouseBodyCtrlPts[0]), adjustMirroredCoords(MouseBodyBasePts[0]));

       mouse.closeSubpath();

       painter.drawPath(mouse);

    }

    void drawLeftButton(bool down) {
       const QPoint &lb_1 = MouseBodyBasePts[5], lb_2(MouseBodyBasePts[5].x(), MouseBodyBasePts[1].y()),
              &lb_3 = MouseBodyBasePts[1],
              &lb_4 = MouseBodyBasePts[3], &lb_5 = MouseBodyBasePts[5],

              ctrl_lb3 = MouseBodyCtrlPts[2], ctrl_lb4 = MouseBodyCtrlPts[3],
              ctrl_lb4_1 = MouseBodyCtrlPts[4],ctrl_lb5 = MouseBodyCtrlPts[5];

       QPainter button_painter(this);
       button_painter.setRenderHint(QPainter::Antialiasing, true);
       button_painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

       QLinearGradient linearGradient(0, 0, size().width() / 2, size().height() / 2);
       linearGradient.setColorAt(0.25, Qt::white);
       linearGradient.setColorAt(1.0, Qt::red);

       button_painter.setBrush(down ? linearGradient : QBrush(Qt::gray, Qt::NoBrush));

       QPainterPath lb;

       lb.moveTo(adjustCoords(lb_1));
       lb.lineTo(adjustCoords(lb_2));
       lb.lineTo(adjustCoords(lb_3));
       lb.cubicTo(adjustCoords(ctrl_lb3), adjustCoords(ctrl_lb4), adjustCoords(lb_4));
       lb.cubicTo(adjustCoords(ctrl_lb4_1), adjustCoords(ctrl_lb5), adjustCoords(lb_5));
       button_painter.drawPath(lb);

    }

    void drawRightButton(bool down) {
       const QPoint &lb_1 = MouseBodyBasePts[5], lb_2(MouseBodyBasePts[5].x(), MouseBodyBasePts[1].y()),
              &lb_3 = MouseBodyBasePts[1],
              &lb_4 = MouseBodyBasePts[3], &lb_5 = MouseBodyBasePts[5],

              ctrl_lb3 = MouseBodyCtrlPts[2], ctrl_lb4 = MouseBodyCtrlPts[3],
              ctrl_lb4_1 = MouseBodyCtrlPts[4],ctrl_lb5 = MouseBodyCtrlPts[5];

       QPainter button_painter(this);
       button_painter.setRenderHint(QPainter::Antialiasing, true);
       button_painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

       QLinearGradient linearGradient(size().width() / 2, 0, size().width(), size().height() / 2);
       linearGradient.setColorAt(0.25, Qt::white);
       linearGradient.setColorAt(1.0, Qt::red);

       button_painter.setBrush(down ? linearGradient : QBrush(Qt::gray, Qt::NoBrush));

       QPainterPath lb;

       lb.moveTo(adjustMirroredCoords(lb_1));
       lb.lineTo(adjustMirroredCoords(lb_2));
       lb.lineTo(adjustMirroredCoords(lb_3));
       lb.cubicTo(adjustMirroredCoords(ctrl_lb3), adjustMirroredCoords(ctrl_lb4), adjustMirroredCoords(lb_4));
       lb.cubicTo(adjustMirroredCoords(ctrl_lb4_1), adjustMirroredCoords(ctrl_lb5), adjustMirroredCoords(lb_5));
       button_painter.drawPath(lb);
    }

    void drawWheel(int dir) {
       QPainter painter(this);
       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

       painter.setBrush(QBrush(dir == 0 ? Qt::black : Qt::green, Qt::Dense5Pattern));

       QPoint lt(DesignedSize.width() / 2 - (
           DesignedSize.width() / 2 - MouseBodyBasePts[6].x()) / 2,
           MouseBodyBasePts[6].y() + (MouseBodyBasePts[2].y() - MouseBodyBasePts[6].y()) * 0.25
       );
       QPoint rb(DesignedSize.width() / 2 + (
           DesignedSize.width() / 2 - MouseBodyBasePts[6].x()) / 2,
           MouseBodyBasePts[6].y() + (MouseBodyBasePts[2].y() - MouseBodyBasePts[6].y()) * 0.75
       );

       QPoint tl = adjustCoords(lt), br = adjustCoords(rb);

       painter.drawRect(tl.x(), tl.y(), br.x() - tl.x(), br.y() - tl.y());
    }

    void drawMidButton(bool down) {
       const QPoint &mb_1 = MouseBodyBasePts[6],
               &mb_2 = MouseBodyBasePts[7],
               &mb_3 = {MouseBodyBasePts[5].x(), MouseBodyBasePts[1].y()};

       QPainter button_painter(this);
       button_painter.setRenderHint(QPainter::Antialiasing, true);
       button_painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

       QLinearGradient linearGradient(size().width() / 2, 0, size().width(), size().height() / 2);
       linearGradient.setColorAt(0.25, Qt::white);
       linearGradient.setColorAt(1.0, Qt::red);

       button_painter.setBrush(down ? linearGradient : QBrush(Qt::gray, Qt::NoBrush));

       QPainterPath lb;

       lb.moveTo(adjustCoords(mb_1));
       lb.lineTo(adjustCoords(mb_2));
       lb.lineTo(adjustMirroredCoords(mb_2));
       lb.lineTo(adjustMirroredCoords(mb_1));
       lb.lineTo(adjustMirroredCoords(mb_3));
       lb.lineTo(adjustCoords(mb_3));

       button_painter.drawPath(lb);
    }

public:

    MouseWidget(QWidget * o):QWidget(o) {
        if (QtWin::isCompositionEnabled()) {
                QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
                setAttribute(Qt::WA_TranslucentBackground, true);
                setAttribute(Qt::WA_NoSystemBackground, false);
                setStyleSheet("MouseWidget { background: transparent; }");
        }

        resize(80, 100);

        pen = QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
        brush = QBrush(Qt::gray, Qt::NoBrush);

    }

    void paintEvent(QPaintEvent *) {
       drawMouseBody();

       drawLeftButton(false);
       drawMidButton(false);
       drawRightButton(false);

       drawWheel( 0 );

       QPainter painter(this);

       painter.setPen(QPen(Qt::black));
       painter.setBrush(brush);

       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.drawText(QRect(0,0, size().width(), 20), mouseName);

//       painter.drawImage(
//          QPoint(size().width() / 2 - img.width() / 2, size().height() / 2), img);
    }

public slots:

    void setMouseName(QString name) {
       mouseName = name;
    }

};
