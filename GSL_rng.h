#ifndef __GSL_RNG_H__
#define __GSL_RNG_H__
#include <stdlib.h>

typedef struct
{
	size_t size;
	void (*set) (void *state, unsigned long int seed);
	unsigned long int (*get) (void *state);
	double (*get_double) (void *state);
}gsl_rng_type;

typedef struct
{
	const gsl_rng_type * type;
	void *state;
}gsl_rng;

extern const gsl_rng_type *gsl_rng_mt19937;

extern unsigned long int gsl_rng_default_seed;

gsl_rng *gsl_rng_alloc (const gsl_rng_type * T);

void gsl_rng_free (gsl_rng * r);

inline double gsl_rng_uniform (const gsl_rng * r);
inline double gsl_rng_uniform_pos (const gsl_rng * r);

inline unsigned long int gsl_rng_get (const gsl_rng * r)
{
	return (r->type->get) (r->state);
}

inline double gsl_rng_uniform (const gsl_rng * r)
{
	return (r->type->get_double) (r->state);
}

inline double gsl_rng_uniform_pos (const gsl_rng * r)
{
	double x ;
	do
	{
		x = (r->type->get_double) (r->state) ;
	}
	while (x == 0) ;

	return x ;
}

#endif /* __GSL_RNG_H__ */