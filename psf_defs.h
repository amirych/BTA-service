#ifndef PSF_DEFS_H
#define PSF_DEFS_H

#define MOFFAT2D_NPARS 8
#define FITTING_MAX_ITER 100

typedef double moffat2d_params[MOFFAT2D_NPARS];
typedef double moffat2d_lower_bounds[MOFFAT2D_NPARS];
typedef double moffat2d_upper_bounds[MOFFAT2D_NPARS];


//moffat2d_lower_bounds moffat2d_default_lower_bounds = {0.0,-100.0,-100.0,1.0,1.0,1.0,0.0};
//moffat2d_upper_bounds moffat2d_default_upper_bounds = {1.0E4,5000.0,5000.0,20.0,20.0,10.0,360.0};


int fit_psf(double *data, std::size_t Ndata, double start_x, double start_y, std::size_t Nx, std::size_t Ny,
            moffat2d_lower_bounds lb,
            moffat2d_upper_bounds ub,
            moffat2d_params pars);

void comp_moffat2d(double start_x, double start_y, size_t Nx, size_t Ny, moffat2d_params pars, double *func);

#endif // PSF_DEFS_H

