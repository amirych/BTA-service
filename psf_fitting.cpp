#include <cmath>
#include <cstdlib>
#include <psf_defs.h>
#include <new>

#include "levmar.h"

#include <QDebug>

using namespace std;


struct moffat2d_data {
    std::size_t Nx; // number of points along X-axis
    std::size_t Ny; // number of points along Y-axis
    double *x;
    double *y;
};

/*
  The Moffat's function:

               f(x,y) = A/(1+k*r(x,y)^2)^alpha + const, where

               A - amplitude of the function
               k = 4*(2^(1/alpha)-1) - norm. parameter:
               r(x,y) = r^2 = ((x-xc)/fwhmX)^2+((y-yc)/fwhmY)^2 - radius vector
                 xc and yc - x and y peak positions
                 fwhmX and fwhmY - full width at half maximum along X and Y axes, respectively
               alpha - slope of Moffat's function

             The present function realization allows to compute Moffat's function
             with its symmetry axes that are not parallel to coordinate axes.
             Following this, in the first the routine computes transformation to new "tilted"
             coordinate system:

               x' = (x-xc)*cos(phi) - (y-yc)*sin(phi)
               y' = (y-yc)*cos(phi) + (x-xc)*sin(phi), where

               phi - rotation angle (clock-wise from X-axis)


  pars[] = {A,xc,yc,fwhmX,fwhmY,alpha,phi,const}
    phi is in degrees


  data pointer is interpretated as a structure of type "moffat2d_data"

*/

void moffat2d(double *pars, double *func, int n_pars, int n_func, void* data)
{
    double k,r2,xp,yp;
    moffat2d_data *xy = (moffat2d_data*) data;

    if ( n_pars < MOFFAT2D_NPARS ) return;

    if ( xy->Nx*xy->Ny != n_func ) return;

    double phi = pars[6]/180.0*M_PI;

    k = 4.0*(pow(2,1.0/pars[5])-1.0);

    std::size_t l = 0;
    for ( std::size_t i = 0; i < xy->Ny; ++i ) {
        double y1 = (xy->y[i]-pars[2]);
        double y2 = y1*cos(phi);
        y1 *= sin(phi);
        for ( std::size_t j = 0; j < xy->Nx; ++j ) {
            double xx = (xy->x[j]-pars[1]);
            xp = xx*cos(phi) - y1;
            yp = y2 + xx*sin(phi);

            r2 = pow(xp/pars[3],2) + pow(yp/pars[4],2);

            func[l] = pars[0]*pow(1.0+k*r2,-pars[5]) + pars[7];
            ++l;
        }
    }

}

moffat2d_data* init_moffat2d_data(std::size_t Nx, std::size_t Ny)
{
    moffat2d_data *st = nullptr;

    st = new moffat2d_data;
    st->Nx = Nx;
    st->Ny = Ny;

    st->x = nullptr;
    st->y = nullptr;

    st->x = new double[Nx];
    st->y = new double[Ny];

    return st;
}


void free_moffat2d_data(moffat2d_data *data)
{
    if ( data == nullptr ) return;

    delete[] data->x;
    delete[] data->y;

    delete data;

    data = nullptr;
}


int fit_psf(double *data, std::size_t Ndata, double start_x, double start_y, std::size_t Nx, std::size_t Ny,
            moffat2d_lower_bounds lb,
            moffat2d_upper_bounds ub,
            double* pars)
//            moffat2d_params pars)
{
    moffat2d_data *md = nullptr;
    double *work_space = nullptr;
    double info[LM_INFO_SZ];

    int ret;

    try {
        work_space = new double[LM_BC_DIF_WORKSZ(MOFFAT2D_NPARS,Ndata)];
    } catch (std::bad_alloc &ex) {
        return -100;
    }

    try {
        md = init_moffat2d_data(Nx,Ny);

        for ( size_t i = 0; i < Nx; ++i ) {
            md->x[i] = start_x + i;
        }
        for ( size_t i = 0; i < Ny; ++i ) {
            md->y[i] = start_y + i;
        }

//        ret = dlevmar_bc_dif(moffat2d,pars,data,MOFFAT2D_NPARS,Ndata,lb,ub,NULL,FITTING_MAX_ITER,NULL,NULL,work_space,NULL,(void*)md);
        ret = dlevmar_bc_dif(moffat2d,pars,data,MOFFAT2D_NPARS,Ndata,lb,ub,NULL,FITTING_MAX_ITER,NULL,info,work_space,NULL,(void*)md);

        delete[] work_space;
        free_moffat2d_data(md);

        if ( info[6] == 7 ) return -200; // Nan or Inf func value!!!
        return ret;
    } catch (std::bad_alloc &ex) {
        delete[] work_space;
        return -100;
    }
}


// func must be allocated in caller (Nx*Ny array)!!!
void comp_moffat2d(double start_x, double start_y, size_t Nx, size_t Ny, moffat2d_params pars, double *func)
{
    moffat2d_data *md = nullptr;

    try {
        md = init_moffat2d_data(Nx,Ny);

        for ( size_t i = 0; i < Nx; ++i ) {
            md->x[i] = start_x + i;
        }
        for ( size_t i = 0; i < Ny; ++i ) {
            md->y[i] = start_y + i;
        }

        moffat2d(pars,func,MOFFAT2D_NPARS,Nx*Ny,(void*)md);

        free_moffat2d_data(md);
    } catch (std::bad_alloc &ex) {
    }
}
