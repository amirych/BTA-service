#include "psf_dialog.h"

#include <algorithm>
#include <QVBoxLayout>

#include "fitsio.h"
#include "levmar.h"

psf_dialog::psf_dialog(QString fits_filename, QRectF region, QWidget *parent, Qt::WindowFlags f): QDialog(parent,f)
{
    setupUi();

    connect(close_button,SIGNAL(clicked()),this,SLOT(deleteLater()));

    plot_psf(fits_filename,region);
}


psf_dialog::psf_dialog(QWidget *parent, Qt::WindowFlags f):psf_dialog("",QRectF(),parent,f)
{

}



void psf_dialog::plot_psf(QString fits_filename, QRectF region)
{
    if ( fits_filename.trimmed().isEmpty() ) return;

    fitsfile *file;
    int fits_status = 0;

    double *data = nullptr;
    double *fitted_data = nullptr;

    moffat2d_lower_bounds lc = {0.0,-100.0,-100.0,1.0,1.0,1.0,0.0,0.0};
    moffat2d_upper_bounds uc = {1.0E14,5000.0,5000.0,20.0,20.0,10.0,360.0,1.0E14};
//    moffat2d_params pars;

    double h,w;
    QVector<double> xdataX,ydataX,yfitX;
    QVector<double> xdataY,ydataY,yfitY;

    long fpixel[2] = {1,1};

    int xl,xr,yl,yr;

    // region is in FITS pixel notation!!! (starting from [1,1])

    xl = qRound(region.x());
    yl = qRound(region.y());

    // check for just pixel-wide region
    w = (region.width() < 1) ? 1.0 : region.width();
    h = (region.height() < 1) ? 1.0: region.height();

    xr = qRound(region.x() + w - 1);
    yr = qRound(region.y() + h - 1);

    LONGLONG Ncols = (xr-xl+1);
    LONGLONG Nrows = (yr-yl+1);

    LONGLONG nelem = Ncols*Nrows;
    if ( nelem <= 0 ) return;


    QString filename = fits_filename.trimmed();

    filename += "[" + QString::number(xl) + ":" + QString::number(xr) + "," + QString::number(yl) + ":" + QString::number(yr) + "]";

    try {
        fitted_data = new double[nelem];
    } catch (std::bad_alloc &ex) {
        return;
    }

    try {
        xdataX.resize(Ncols);
        ydataX.resize(Ncols);
        yfitX.resize(Ncols);

        xdataY.resize(Nrows);
        ydataY.resize(Nrows);
        yfitY.resize(Nrows);
    } catch (std::bad_alloc &ex) {
        return;
    }

    try {
        data = new double[nelem];

        fits_open_image(&file,filename.toLocal8Bit().data(),READONLY,&fits_status);
        if ( fits_status ) throw fits_status;

        fits_read_pix(file,TDOUBLE,fpixel,nelem,NULL,data,NULL,&fits_status);
        if ( fits_status ) throw fits_status;

        fits_close_file(file,&fits_status);


        auto min_el = std::min_element(data,data+nelem);
        auto max_el = std::max_element(data,data+nelem);
        fit_pars[0] = *max_el - *min_el;
        fit_pars[7] = (*min_el >= 0) ? *min_el : 0.0;
        fit_pars[1] = ((max_el - data) % Ncols) + xl;
        fit_pars[2] = ((max_el - data) / Ncols) + yl;
        fit_pars[3] = fit_pars[4] = 3.0;
        fit_pars[5] = 2.5;
        fit_pars[6] = 0.0;

        qDebug() << "PEAK: " << fit_pars[1] << fit_pars[2];

        int ret = fit_psf(data,nelem,xl,yl,Ncols,Nrows,lc,uc,fit_pars);

        qDebug() << "FIT: " << ret;

        if ( ret > 0 ) {
//            emit psfComputed(pars);

            comp_moffat2d(xl,yl,Ncols,Nrows,fit_pars,fitted_data);

            // get slicers
            int xc = qRound(fit_pars[1]-xl);
            if ( xc < 0 ) xc = 0;
            if ( xc >= Ncols ) xc = Ncols-1;

            int yc = qRound(fit_pars[2]-yl);
            if ( yc < 0 ) yc = 0;
            if ( yc >= Nrows ) yc = Nrows-1;

            // along x
            for ( size_t i = 0; i < Ncols; ++i ) {
                double x = xl + i;
                xdataX[i] = x;
                double y = data[yc*Ncols + i];
                ydataX[i] = y;
                y = fitted_data[yc*Ncols + i];
                yfitX[i] = y;
            }
            for ( int i = 0; i < MOFFAT2D_NPARS; ++i ) qDebug() << fit_pars[i];
            x_graph_data->setData(xdataX,ydataX);
            x_graph_fit->setData(xdataX,yfitX);
            x_graph_data->rescaleAxes();
            x_graph_fit->rescaleAxes();

            // along y

            for ( size_t i = 0; i < Nrows; ++i ) {
                double x = yl + i;
                xdataY[i] = x;
                double y = data[i*Ncols + xc];
                ydataY[i] = y;
                y = fitted_data[i*Ncols + xc];
                yfitY[i] = y;
            }
            y_graph_data->setData(xdataY,ydataY);
            y_graph_data->rescaleAxes();
            y_graph_fit->setData(xdataY,yfitY);
            y_graph_fit->rescaleAxes();

            QString str = formatResults(fit_pars);
            result_label->setText(str);
        } else {
            result_label->setText("Cannot fit the data!");
        }


        delete[] fitted_data;
        delete[] data;
    } catch (int err ) {
        fits_close_file(file,&fits_status);
        delete[] fitted_data;
        delete[] data;
    } catch (std::bad_alloc &ex) {
        delete[] fitted_data;
    }
}


void psf_dialog::getFitParams(moffat2d_params pars)
{
    for (int i = 0; i < MOFFAT2D_NPARS; ++i ) pars[i] = fit_pars[i];
}

            /*      Private methods     */

void psf_dialog::setupUi()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    customPlot = new QCustomPlot(this);

    result_gbox = new QGroupBox("Fitting result: ",this);
    QVBoxLayout *result_layout = new QVBoxLayout(result_gbox);
    result_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    result_label = new QLabel("",result_gbox);
    result_layout->addWidget(result_label);

    close_button = new QPushButton("Close",this);


    customPlot->plotLayout()->clear();

    QCPAxisRect *xAxisRect = new QCPAxisRect(customPlot,false);
    QCPAxisRect *yAxisRect = new QCPAxisRect(customPlot,false);

    xAxisRect->setupFullAxesBox(true);
    yAxisRect->setupFullAxesBox(true);

    customPlot->plotLayout()->addElement(0, 0, xAxisRect);
    customPlot->plotLayout()->addElement(0, 1, yAxisRect);

//    xAxisRect->addAxes(QCPAxis::atLeft | QCPAxis::atBottom );
//    yAxisRect->addAxes(QCPAxis::atLeft | QCPAxis::atBottom );
//    xAxisRect->addAxes(QCPAxis::atLeft | QCPAxis::atBottom | QCPAxis::atRight | QCPAxis::atTop);
//    yAxisRect->addAxes(QCPAxis::atLeft | QCPAxis::atBottom | QCPAxis::atRight | QCPAxis::atTop);

    x_graph_data = customPlot->addGraph(xAxisRect->axis(QCPAxis::atBottom),xAxisRect->axis(QCPAxis::atLeft));
    y_graph_data = customPlot->addGraph(yAxisRect->axis(QCPAxis::atBottom),yAxisRect->axis(QCPAxis::atLeft));

    x_graph_data->setPen(QPen(QColor("black"),1));
    x_graph_data->setLineStyle(QCPGraph::lsStepCenter);
    y_graph_data->setPen(QPen(QColor("black"),1));
    y_graph_data->setLineStyle(QCPGraph::lsStepCenter);

    x_graph_fit = customPlot->addGraph(xAxisRect->axis(QCPAxis::atBottom),xAxisRect->axis(QCPAxis::atLeft));
    y_graph_fit = customPlot->addGraph(yAxisRect->axis(QCPAxis::atBottom),yAxisRect->axis(QCPAxis::atLeft));

    x_graph_fit->setPen(QPen(QColor("red"),1));
    x_graph_fit->setLineStyle(QCPGraph::lsLine);
    y_graph_fit->setPen(QPen(QColor("red"),1));
    y_graph_fit->setLineStyle(QCPGraph::lsLine);


    xAxisRect->axis(QCPAxis::atBottom)->setLabel("X, pixel");
    xAxisRect->axis(QCPAxis::atLeft)->setLabel("counts, ADU");
    yAxisRect->axis(QCPAxis::atBottom)->setLabel("Y, pixel");
    yAxisRect->axis(QCPAxis::atLeft)->setLabel("counts, ADU");

    main_layout->addWidget(customPlot,4);
    main_layout->addWidget(result_gbox,1);
    main_layout->addWidget(close_button,1,Qt::AlignRight);

    this->setWindowTitle("Moffat PSF fitting");
}


QString psf_dialog::formatResults(moffat2d_params fit_pars)
{
    QString str = "Peak value: " + QString::number(fit_pars[0]) + "\n" +
                  "Center: [" + QString::number(fit_pars[1]) + ", " + QString::number(fit_pars[2]) + "]\n" +
                  "FWHM: [" + QString::number(fit_pars[3]) + ", " + QString::number(fit_pars[4]) + "]\n" +
                  "Alpha: " + QString::number(fit_pars[5]) + "\n" +
                  "Axes angle: " + QString::number(fit_pars[6]) + " degrees\n" +
                  "Background: " + QString::number(fit_pars[7]);
    return str;
}
