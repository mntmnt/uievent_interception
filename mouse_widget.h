#pragma once

#include <QWidget>
#include <QLabel>
#include <QtWinExtras>

#include <QPen>
#include <QBrush>


class MouseWidget : public QWidget {
    Q_OBJECT

    QPen pen;
    QBrush brush;

    const QSize DesignedSize = {320, 400};


public:

    void setMouseName(QString name) {
    }

    MouseWidget(QWidget * o):QWidget(o) {
        if (QtWin::isCompositionEnabled()) {
                QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
                setAttribute(Qt::WA_TranslucentBackground, true);
                setAttribute(Qt::WA_NoSystemBackground, false);
                setStyleSheet("MouseWidget { background: transparent; }");
        }

        pen = QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);

        QLinearGradient linearGradient(0, 0, 100, 100);
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.2, Qt::green);
        linearGradient.setColorAt(1.0, Qt::black);
        brush = linearGradient;
        brush = QBrush(Qt::gray, Qt::NoBrush);

    }

    void drawCoords(QPainter & painter) {
        painter.setPen(QPen(Qt::black));
        painter.drawLine(QPoint(0, size().height() / 2),
                         QPoint(size().width(), size().height() / 2));
        painter.drawLine(QPoint(size().width() / 2, 0),
                         QPoint(size().width() / 2, size().height()));

    }

    void paintEvent(QPaintEvent * /* event */) {
       QPainter painter(this);
       drawCoords(painter);

       painter.setPen(pen);
       painter.setBrush(brush);

       painter.setRenderHint(QPainter::Antialiasing, true);

       drawCoords(painter);

       QPoint p1(161, 364), p2(43.3, 200),
              ctrl_p1(59, 364), ctrl_p2(47.3, 241.3),

              &p3 = p2, p4(46, 98.4),
              ctrl_p3(42.4, 196.7), ctrl_p4(45.5, 116),

              &p5 = p4, p6(116.7, 40),
              ctrl_p5(46, 66.2), ctrl_p6(91.5, 44),

              &p7 = p6, p8(161, 40),
              ctrl_p7 = p6, ctrl_p8 = p8;

       QPoint &lb_1 = p6, lb_2(p6.x(), p2.y()), &lb_3 = p2,
              &lb_4 = p4, &lb_5 = p6;

       auto mirror_coords = [this](const QPoint & p) {
          return QPoint(
             this->DesignedSize.width() - p.x(),
             p.y()
          );
       };

       auto fix_coords = [this](const QPoint & p) -> QPoint {
          return QPoint(
             p.x() * this->size().width() / this->DesignedSize.width(),
             p.y() * this->size().height() / this->DesignedSize.height()
             );
       };

       QPainterPath mouse;

       mouse.moveTo(fix_coords(p1));
       mouse.cubicTo(fix_coords(ctrl_p1), fix_coords(ctrl_p2), fix_coords(p2));
       mouse.cubicTo(fix_coords(ctrl_p3), fix_coords(ctrl_p4), fix_coords(p4));
       mouse.cubicTo(fix_coords(ctrl_p5), fix_coords(ctrl_p6), fix_coords(p6));
       mouse.cubicTo(fix_coords(ctrl_p7), fix_coords(ctrl_p8), fix_coords(p8));

       mouse.cubicTo(fix_coords(mirror_coords(ctrl_p8)), fix_coords(mirror_coords(ctrl_p7)), fix_coords(mirror_coords(p6)));
       mouse.cubicTo(fix_coords(mirror_coords(ctrl_p6)), fix_coords(mirror_coords(ctrl_p5)), fix_coords(mirror_coords(p4)));
       mouse.cubicTo(fix_coords(mirror_coords(ctrl_p4)), fix_coords(mirror_coords(ctrl_p3)), fix_coords(mirror_coords(p2)));
       mouse.cubicTo(fix_coords(mirror_coords(ctrl_p2)), fix_coords(mirror_coords(ctrl_p1)), fix_coords(mirror_coords(p1)));

       mouse.closeSubpath();

       painter.drawPath(mouse);

       QPainter button_painter(this);
       button_painter.setRenderHint(QPainter::Antialiasing, true);
       button_painter.setPen(
       QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
       );

        QLinearGradient linearGradient(0, 0, size().width() / 2, size().height() / 2);
        linearGradient.setColorAt(0.25, Qt::white);
        linearGradient.setColorAt(1.0, Qt::red);


       button_painter.setBrush(linearGradient);

       QPainterPath lb;

       lb.moveTo(fix_coords(lb_1));
       lb.lineTo(fix_coords(lb_2));
       lb.lineTo(fix_coords(lb_3));
       lb.cubicTo(fix_coords(ctrl_p3), fix_coords(ctrl_p4), fix_coords(lb_4));
       lb.cubicTo(fix_coords(ctrl_p5), fix_coords(ctrl_p6), fix_coords(lb_5));
       button_painter.drawPath(lb);
    }



};
