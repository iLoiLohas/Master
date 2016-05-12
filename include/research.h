#ifndef __RESEARCH_H_INCLUDE__
#define __RESEARCH_H_INCLUDE__

#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <map>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_sf.h>

using namespace std;


/********** �n�̌W���E���͏����i�s�ρj**********/
#define S0 (1./(2.*M_PI))	// whitenoise�̃p���[�X�y�N�g��
#define EPSILON 0.3			// ����`���̋���
#define ZETA 0.05			// �����萔

/********** �v�Z���� **********/
#define SAMPLE_LENGTH 131072	// 131072,65536
#define NUM_OF_SAMPLES 100		// ���͂̕W�{��
#define dt 0.01		// ���ԍ��ݕ�
#define dx 0.1		// pdf�̉����̍��ݕ�


class Simulation
{
private:
	double _lambda, _beta2, _alpha, _sigma;
	// ���͕W�{
	double _force[SAMPLE_LENGTH];
	// pdf�쐬���̃o�b�t�@�[
	double y1_buffer[SAMPLE_LENGTH][NUM_OF_SAMPLES];
	double y2_buffer[SAMPLE_LENGTH][NUM_OF_SAMPLES];
	// pdf�̉����̍ŏ��l�C�ő�l
	double y1min, y1max, y2max, y2min;
	void _createExcitation();
	// �����Q�N�b�^�Ŏg��
	static double _f1(double force, double y1, double y2);
	static double _f2(double force, double y1, double y2);

public:
	Simulation(double lambda, double beta2, double alpha);
	void culcRungeKutta();
	void culcDisplacementPdf();
	void culcVelocityPdf();
	void culcDispVariance();
	void culcVelVariance();
	void exactSolutionOfGaussianWhiteNoise();
};

#endif // !__RESEARCH_H_INCLUDE_