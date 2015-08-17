#include "psf_dialog.h"

#include <algorithm>
#include <QVBoxLayout>

#include "fitsio.h"
#include "psf_defs.h"
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
    moffat2d_upper_bounds uc = {1.0E4,5000.0,5000.0,20.0,20.0,10.0,360.0,10000.0};
    moffat2d_params pars;

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


        auto minmax_data = std::minmax_element(data,data+nelem);
        pars[0] = *minmax_data.second;
        pars[7] = *minmax_data.first;
        pars[1] = (xr+xl)/2.0;
        pars[2] = (yr+yl)/2.0;
        pars[3] = pars[4] = 3.0;
        pars[5] = 2.5;
        pars[6] = 0.0;

        int ret = fit_psf(data,nelem,xl,yl,Ncols,Nrows,lc,uc,pars);

        qDebug() << "FIT: " << ret;

        if ( ret > 0 ) {
            comp_moffat2d(xl,yl,Ncols,Nrows,pars,fitted_data);

            // get slicers
            int xc = qRound(pars[1]-xl);
            if ( xc < 0 ) xc = 0;
            if ( xc >= Ncols ) xc = Ncols-1;

            int yc = qRound(pars[2]-yl);
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
            for ( int i = 0; i < MOFFAT2D_NPARS; ++i ) qDebug() << pars[i];
            x_graph_data->setData(xdataX,ydataX);
            x_graph_fit->setData(xdataX,yfitX);
            x_graph_data->rescaleAxes();
//            x_graph_fit->rescaleAxes();

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
//            y_graph_fit->setData(xdataY,yfitY);
        } else {

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


            /*      Private methods     */

void psf_dialog::setupUi()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    customPlot = new QCustomPlot(this);

    close_button = new QPushButton("Close",this);

    main_layout->addWidget(customPlot);
    main_layout->addWidget(close_button,0,Qt::AlignRight);

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

//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

//    QVector<double> x1a(20), y1a(20);
//    for (int i=0; i<x1a.size(); ++i)
//    {
//      x1a[i] = i/(double)(x1a.size()-1)*10-5.0;
//      y1a[i] = qCos(x1a[i]);
//    }

//    x_graph->setData(x1a,y1a);
//    y_graph->setData(x1a,y1a);

//    x_graph->rescaleAxes();
//    y_graph->rescaleAxes();
}
