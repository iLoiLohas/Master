#ifndef __GA_H_INCLUDE__
#define __GA_H_INCLUDE__

#include <stdlib.h>
#include <random>
#include <math.h>
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

class GA
{
public:
	GA();
	~GA();
	void initGene();
	void culcFitness();
	void output(int generation);

private:
	/*
	�K���x�̌v�Z�w�W
		0:���g�p
		1:�K���x�v�Z�O�i�ˑR�ψق͂��̌̂����ɓK���j
		2:�K���x�v�Z�ς݁i�������ɐe�Ƃ݂Ȃ��j
	*/
	int* fitnessIndex;
	double* fitness;
	double meanFitness;
	double maxFitness;
	int population;
	int geneLength;
	unsigned int **allIndividual;
	double _binary2Decimal(int* binary);
	bool _isDuplicatedGene(unsigned int **gene);
	// @TODO:setObjectiveFunc��ǉ�������
	double _getObjectiveFunc(double x);	// 1�ϐ��̏ꍇ
};

#endif // !__GA_H_INCLUDE__