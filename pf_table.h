#ifndef PF_TABLE_H
#define PF_TABLE_H

#include <QWidget>

#define __widget_size__  200
#define __scale_pan__  35
#define __scale_step__  20
#define __tick_len__  5
#define __hand_pan__  3

#define __scale_start_angle__  90
#define __scale_stop_angle__  370



class PF_table : public QWidget
{
    Q_OBJECT
public:
    explicit PF_table(QWidget *parent = nullptr);
    PF_table(const double init_pos, QWidget *parent = nullptr);

signals:

public slots:
    void setNewPos(const double new_pos);
    void changePos();

    void paintEvent(QPaintEvent *event);

private:
    void drawTable();

    double currentPos, newPos;

    int startAngle, stopAngle, spanAngle;
};

#endif // PF_TABLE_H
