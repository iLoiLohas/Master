#ifndef __RESEARCH_H_INCLUDE__
#define __RESEARCH_H_INCLUDE__

#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_sf.h>

using namespace std;

class Simulation
{
private:
	/********** vZð **********/
	static const std::size_t SAMPLE_LENGTH;
	static const std::size_t NUM_OF_SAMPLES;	// üÍÌW{
	static const double dt;	// ÔÝ
	static const double dx;	// pdfÌ¡²ÌÝ

	double _lambda, _beta2, _alpha, _sigma;
	// pdfÌ¡²ÌÅ¬lCÅål
	double _y1min, _y1max, _y2max, _y2min;
	// QNb^Åg¤
	static double _f1(double force, double y1, double y2);
	static double _f2(double force, double y1, double y2);
	void _createExcitation(std::vector< std::vector<double> > &);

public:
	Simulation(double lambda, double beta2, double alpha);
	void culcRungeKutta(std::vector<double> &, std::vector< std::vector<double> > &, std::vector< std::vector<double> > &, std::vector< std::vector<double> > &);
	void createDispPdf(const std::vector< std::vector<double> > &, std::vector<double> &, std::vector<double> &);
	void createVelPdf(const std::vector< std::vector<double> > &, std::vector<double> &, std::vector<double> &);
	void exactSolutionOfGaussianWhiteNoise(std::vector<double> &, std::vector<double> &);
};

#endif // !__RESEARCH_H_INCLUDE_