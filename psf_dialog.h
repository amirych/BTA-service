#ifndef PSF_DIALOG_H
#define PSF_DIALOG_H

#include <QWidget>
#include <QString>
#include <QRectF>
#include <QPushButton>

#include "qcustomplot.h"

class psf_dialog : public QDialog
{
public:
    explicit psf_dialog(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    psf_dialog(QString fits_filename, QRectF region, QWidget *parent = nullptr, Qt::WindowFlags f = 0);

    void plot_psf(QString fits_filename, QRectF region);

private:
    void setupUi();

    QCustomPlot *customPlot;
    QCPGraph *x_graph_data;
    QCPGraph *y_graph_data;
    QCPGraph *x_graph_fit;
    QCPGraph *y_graph_fit;


    QPushButton *close_button;
};

#endif // PSF_DIALOG_H
