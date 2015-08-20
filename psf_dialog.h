#ifndef PSF_DIALOG_H
#define PSF_DIALOG_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QRectF>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "qcustomplot.h"
#include "psf_defs.h"

class psf_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit psf_dialog(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    psf_dialog(QString fits_filename, QRectF region, QWidget *parent = nullptr, Qt::WindowFlags f = 0);

    void plot_psf(QString fits_filename, QRectF region);

signals:
    void psfComputed(moffat2d_params pars);

private:
    void setupUi();
    QString formatResults(moffat2d_params fit_pars);

    QCustomPlot *customPlot;
    QCPGraph *x_graph_data;
    QCPGraph *y_graph_data;
    QCPGraph *x_graph_fit;
    QCPGraph *y_graph_fit;


    QGroupBox *result_gbox;
    QLabel *result_label;
    QPushButton *close_button;
};

#endif // PSF_DIALOG_H
