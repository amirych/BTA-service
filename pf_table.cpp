#include "pf_table.h"

#include<QPainter>
#include<QPen>
#include<QFont>
#include<QFontMetrics>
#include<QString>

#include<cmath>


            /*    Constructors    */

PF_table::PF_table(const double init_pos, QWidget *parent):
    QWidget(parent), currentPos(init_pos), newPos(init_pos)
{
    startAngle = __scale_start_angle__ - 90;
    stopAngle = __scale_stop_angle__ -90;
    spanAngle = startAngle - stopAngle;
}

PF_table::PF_table(QWidget *parent) : PF_table(220.0,parent)
{

}



            /*    Public slots    */

void PF_table::setNewPos(const double new_pos)
{
    newPos = new_pos;
    this->update();
}


void PF_table::changePos()
{
    if ( currentPos == newPos ) return;


}


void PF_table::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QPen pen;

    int side;

    this->setStyleSheet("QWidget { background-color: white }");

    if ( this->width() > this->height() ) {
        side = this->width();
    } else {
        side = this->height();
    }

    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2, this->height()/2); // move origin to the center of the widget
    painter.scale(side/__widget_size__,side/__widget_size__);

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
