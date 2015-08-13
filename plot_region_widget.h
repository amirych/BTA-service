#ifndef PLOT_REGION_WIDGET_H
#define PLOT_REGION_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QRectF>
#include <QPushButton>
#include <QString>

#include "qcustomplot.h"


class plot_region_widget : public QDialog
{
public:
    plot_region_widget(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    plot_region_widget(QString fits_filename, QRectF region, QWidget *parent = nullptr, Qt::WindowFlags f = 0);

    void plotRegion(QString fits_filename, QRectF region);

private:
    void setupUI();

    QCustomPlot *customPlot;
    QPushButton *close_button;
};

#endif // PLOT_REGION_WIDGET_H
