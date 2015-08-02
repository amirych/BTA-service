#include "pf_table.h"

#include<QPainter>
#include<QPen>
#include<QFont>
#include<QFontMetrics>
#include<QString>
#include<QVBoxLayout>
#include<QEvent>
#include<QRect>

#include<cmath>

#include <QDebug>

            /*    Constructors    */

PF_table::PF_table(const double init_pos, QWidget *parent):
    QWidget(parent), currentPos(init_pos), newPos(init_pos)
{
//    QVBoxLayout *widget_layout = new QVBoxLayout(this);
//    widget_layout->setMargin(0);
//    main_widget = new QWidget(this);

//    widget_layout->addWidget(main_widget);

//    main_widget->installEventFilter(this);

    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);

    startAngle = __scale_start_angle__ - 90;
    stopAngle = __scale_stop_angle__ -90;
    spanAngle = startAngle - stopAngle;

}


PF_table::PF_table(QWidget *parent) : PF_table(220.0,parent)
{

}



//bool PF_table::hasHeightForWidth() const
//{
//    return true;
//}


int PF_table::heightForWidth(int w) const
{
    return w;
}


//void PF_table::resizeEvent(QResizeEvent *event)
//{

//    QSize main_widget_geometry = this->size();

//    if ( main_widget_geometry.height() > main_widget_geometry.width() ) {
//        main_widget_geometry.setHeight(main_widget_geometry.width());
//    } else {
//        main_widget_geometry.setWidth(main_widget_geometry.height());
//    }

////    main_widget->resize(main_widget_geometry);
//    this->resize(main_widget_geometry);
////    update();
//}


QSize PF_table::sizeHint() const
{
    return QSize(__widget_size__/2,__widget_size__/2);
//    QSize sz = this->parentWidget()->size();
//    int side;
//    if ( sz.width() > sz.height() ) {
//        side = sz.height();
//    } else {
//        side = sz.width();
//    }

//    qDebug() << side;
//    return QSize(side, side);
}


QSizePolicy PF_table::sizePolicy() const
{
//    return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    return QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}


            /*    Public slots    */

void PF_table::setNewPos(const double new_pos)
{
    newPos = new_pos;
    this->update();
}


void PF_table::changePos()
{
    if ( currentPos == newPos ) {
        emit newPosHasReached(newPos);
        return;
    }

    emit tableIsRotating();

    /*
            TODO: hardware-dependant part ...
    */


    currentPos = newPos;
    emit newPosHasReached(currentPos);
}


void PF_table::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QPen pen;

    int side;

//    this->setStyleSheet("QWidget { background-color: white }");
    this->setStyleSheet("background-color: white");

    if ( this->width() < this->height() ) {
        side = this->width();
    } else {
        side = this->height();
    }

    painter.begin(this);
//    painter.begin(main_widget);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2, this->height()/2); // move origin to the center of the widget
    painter.scale(1.0*side/__widget_size__,1.0*side/__widget_size__);

    // draw table

    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(1.2);
    painter.setPen(pen);

    QRectF rect = QRectF(-__widget_size__/2+__scale_pan__,-__widget_size__/2+__scale_pan__,
                         __widget_size__-2*__scale_pan__,__widget_size__-2*__scale_pan__);

    painter.drawArc(rect,-startAngle*16,spanAngle*16);

    // markers
    painter.save();
    for ( int ang = startAngle; ang < stopAngle+__scale_step__; ang += __scale_step__) {
      if ( ang > stopAngle ) break;
      painter.drawLine(__widget_size__/2-__scale_pan__,0,__widget_size__/2-__scale_pan__+__tick_len__,0);
      painter.rotate(__scale_step__);
    }
    painter.restore();

    // marker text
    pen.setWidth(1);
    painter.setPen(pen);

    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    QFontMetrics fm = QFontMetrics(font);
    int h = fm.height();
    int w = fm.width("999");

    int ang_mark = __scale_start_angle__;
    int radius = __widget_size__/2-__scale_pan__+w;
    QString str_mark;

    for ( int ang  = startAngle; ang < stopAngle+__scale_step__; ang += __scale_step__){
        if ( ang > stopAngle ) break;
        double a = ang*M_PI/180.0;
        double x = radius*cos(a);
        double y = radius*sin(a);
        str_mark.setNum(ang_mark);
        painter.drawText(x-w/2,y+h/2,str_mark);
        ang_mark += __scale_step__;
    }

    // new position hand
    painter.save();
    painter.rotate(newPos-90);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1.2);
    painter.setPen(pen);
    painter.drawLine(0,0,__widget_size__/2-__scale_pan__-__hand_pan__,0);
    painter.restore();


    // current position hand
    painter.save();
    painter.rotate(currentPos-90);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(0,0,__widget_size__/2-__scale_pan__-__hand_pan__,0);
    painter.restore();

    painter.end();
}


//bool PF_table::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == main_widget && event->type() == QEvent::Paint) {
//        QPainter painter;
//        QPen pen;

//        int side;

//        main_widget->setStyleSheet("QWidget { background-color: white }");

//        if ( main_widget->width() < main_widget->height() ) {
//            side = main_widget->width();
//        } else {
//            side = main_widget->height();
//        }

//        painter.begin(main_widget);

//        painter.setRenderHint(QPainter::Antialiasing);
//        painter.translate(main_widget->width()/2, main_widget->height()/2); // move origin to the center of the widget
//    //    painter.translate(this->width()/2, this->height()/2); // move origin to the center of the widget
//        painter.scale(1.0*side/__widget_size__,1.0*side/__widget_size__);

//        // draw table

//        pen.setStyle(Qt::SolidLine);
//        pen.setWidthF(1.2);
//        painter.setPen(pen);

//        QRectF rect = QRectF(-__widget_size__/2+__scale_pan__,-__widget_size__/2+__scale_pan__,
//                             __widget_size__-2*__scale_pan__,__widget_size__-2*__scale_pan__);

//        painter.drawArc(rect,-startAngle*16,spanAngle*16);

//        // markers
//        painter.save();
//        for ( int ang = startAngle; ang < stopAngle+__scale_step__; ang += __scale_step__) {
//          if ( ang > stopAngle ) break;
//          painter.drawLine(__widget_size__/2-__scale_pan__,0,__widget_size__/2-__scale_pan__+__tick_len__,0);
//          painter.rotate(__scale_step__);
//        }
//        painter.restore();

//        // marker text
//        pen.setWidth(1);
//        painter.setPen(pen);

//        QFont font = painter.font();
//        font.setPointSize(8);
//        painter.setFont(font);
//        QFontMetrics fm = QFontMetrics(font);
//        int h = fm.height();
//        int w = fm.width("999");

//        int ang_mark = __scale_start_angle__;
//        int radius = __widget_size__/2-__scale_pan__+w;
//        QString str_mark;

//        for ( int ang  = startAngle; ang < stopAngle+__scale_step__; ang += __scale_step__){
//            if ( ang > stopAngle ) break;
//            double a = ang*M_PI/180.0;
//            double x = radius*cos(a);
//            double y = radius*sin(a);
//            str_mark.setNum(ang_mark);
//            painter.drawText(x-w/2,y+h/2,str_mark);
//            ang_mark += __scale_step__;
//        }

//        // new position hand
//        painter.save();
//        painter.rotate(newPos-90);
//        pen.setStyle(Qt::DashLine);
//        pen.setWidth(1.2);
//        painter.setPen(pen);
//        painter.drawLine(0,0,__widget_size__/2-__scale_pan__-__hand_pan__,0);
//        painter.restore();


//        // current position hand
//        painter.save();
//        painter.rotate(currentPos-90);
//        pen.setStyle(Qt::SolidLine);
//        pen.setWidth(2);
//        painter.setPen(pen);
//        painter.drawLine(0,0,__widget_size__/2-__scale_pan__-__hand_pan__,0);
//        painter.restore();

//        painter.end();
//        return true;
//    }
//    return false;
//}
