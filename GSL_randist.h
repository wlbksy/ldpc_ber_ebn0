#ifndef __GSL_RANDIST_H__
#define __GSL_RANDIST_H__
#include "GSL_rng.h"

double gsl_ran_flat (const gsl_rng * r, const double a, const double b);

double gsl_ran_gaussian (const gsl_rng * r, const double sigma);

#endif /* __GSL_RANDIST_H__ */