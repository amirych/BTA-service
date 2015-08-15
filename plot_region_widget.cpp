#include "plot_region_widget.h"

#include <QVBoxLayout>
#include <QVector>
#include <QDebug>

#include "fitsio.h"

plot_region_widget::plot_region_widget(QString fits_filename, QRectF region, QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    setupUI();
    connect(close_button,SIGNAL(clicked()),this,SLOT(deleteLater()));

    if ( fits_filename.isEmpty() ) return;

    plotRegion(fits_filename,region);
}


plot_region_widget::plot_region_widget(QWidget *parent, Qt::WindowFlags f): plot_region_widget("",QRectF(),parent,f)
{

}


void plot_region_widget::setupUI()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    customPlot = new QCustomPlot(this);

    close_button = new QPushButton("Close",this);

    main_layout->addWidget(customPlot);
    main_layout->addWidget(close_button,0,Qt::AlignRight);

    customPlot->addGraph();
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("mean counts, ADU");
    customPlot->xAxis->setRange(0,1);
    customPlot->yAxis->setRange(0,1);
    customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);

    customPlot->replot();

//    customPlot->resize(200,100);
}



void plot_region_widget::plotRegion(QString fits_filename, QRectF region)
{
    fitsfile *file;
    int fits_status = 0;

    double *data = nullptr;
    double h,w;
    QVector<double> xdata,ydata;

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
        data = new double[nelem];

        fits_open_image(&file,filename.toLocal8Bit().data(),READONLY,&fits_status);
        if ( fits_status ) throw fits_status;

        fits_read_pix(file,TDOUBLE,fpixel,nelem,NULL,data,NULL,&fits_status);
        if ( fits_status ) throw fits_status;

        fits_close_file(file,&fits_status);

        // averaging
        if (region.width() >= region.height() ) { // along Y-axis
            customPlot->xAxis->setLabel("X, pixel");
            for ( double x = xl; x <= xr; ++x ) xdata.append(x);
            ydata.resize(xdata.size());

            for ( LONGLONG col = 0; col < Ncols; ++col ) {
                ydata[col] = 0.0;
                for ( LONGLONG row = 0; row < Nrows; ++row ) {
                    ydata[col] += data[Ncols*row + col];
                }
                ydata[col] /= Nrows;
            }
        } else { // along X-axis
            customPlot->xAxis->setLabel("Y, pixel");
            for ( double y = yl; y <= yr; ++y ) xdata.append(y);
            ydata.resize(xdata.size());

            for ( LONGLONG row = 0; row < Nrows; ++row ) {
                ydata[row] = 0.0;
                for ( LONGLONG col = 0; col < Ncols; ++col ) {
                    ydata[row] += data[Ncols*row + col];
                }
                ydata[row] /= Ncols;
            }
        }

        delete[] data;

        customPlot->graph(0)->setData(xdata,ydata);
        customPlot->xAxis->rescale();
        customPlot->yAxis->rescale();
        customPlot->replot();

    } catch (int err ) {
        fits_close_file(file,&fits_status);
        delete[] data;
    } catch (std::bad_alloc &ex) {
    }

}
