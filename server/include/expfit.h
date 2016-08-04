#ifndef __EXPFIT_H_INCLUDE_
#define __EXPFIT_H_INCLUDE_

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

using namespace std;

class MomentEq
{
public:
	static int expb_f(const gsl_vector *x, void *params, gsl_vector *f);
	static int expb_df(const gsl_vector * x, void *params, gsl_matrix *J);
	static int expb_fdf(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *J);
	static void getMomentFromParameter(const std::vector<double> &, std::vector<double> &);
	// static double limitingConditionFnc(const gsl_vector* x);
};

#endif // !__EXPFIT_H_INCLUDE_