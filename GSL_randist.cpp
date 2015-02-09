#include <cmath>
#include "GSL_rng.h"

double gsl_ran_flat (const gsl_rng * r, const double a, const double b)
{
	double u = gsl_rng_uniform (r);

	/* A uniform distribution over [a,b) */

	return a * (1 - u) + b * u;
}


double gsl_ran_gaussian (const gsl_rng * r, const double sigma)
{
	double x, y, r2;

	do
	{
		/* choose x,y in uniform square (-1,-1) to (+1,+1) */
		x = -1 + 2 * gsl_rng_uniform_pos (r);
		y = -1 + 2 * gsl_rng_uniform_pos (r);

		/* see if it is in the unit circle */
		r2 = x * x + y * y;
	}
	while (r2 > 1.0 || r2 == 0);

	/* Box-Muller transform */
	return sigma * y * sqrt (-2.0 * log (r2) / r2);
}