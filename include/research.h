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

#include "common.h"

using namespace std;


/********** �n�̌W���E���͏����i�s�ρj**********/
#define S0 (1./(2.*M_PI))	// whitenoise�̃p���[�X�y�N�g��
#define EPSILON 0.3			// ����`���̋���
#define ZETA 0.05			// �����萔

/********** �v�Z���� **********/
#define SAMPLE_LENGTH 131072	// 131072,65536
#define NUM_OF_SAMPLES 200		// ���͂̕W�{��
#define dt 0.01		// ���ԍ��ݕ�
#define dx 0.1		// pdf�̉����̍��ݕ�


class Simulation
{
private:
	double _lambda, _beta2, _alpha, _sigma;
	// pdf�̉����̍ŏ��l�C�ő�l
	double _y1min, _y1max, _y2max, _y2min;
	// �����Q�N�b�^�Ŏg��
	static double _f1(double force, double y1, double y2);
	static double _f2(double force, double y1, double y2);

public:
	Simulation(double lambda, double beta2, double alpha);
	void createExcitation(std::vector<double> &, std::vector<double> &);
	void culcRungeKutta(std::vector<double> &, std::vector< std::vector<double> > &, std::vector< std::vector<double> > &, const std::vector<double> &);
	void createDispPdf(const std::vector< std::vector<double> > &, std::vector<double> &, std::vector<double> &);
	void createVelPdf(const std::vector< std::vector<double> > &, std::vector<double> &, std::vector<double> &);
	void exactSolutionOfGaussianWhiteNoise();
};

#endif // !__RESEARCH_H_INCLUDE_