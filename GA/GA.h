#ifndef __GA_H_INCLUDE__
#define __GA_H_INCLUDE__

#include <stdlib.h>
#include <random>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

class GA
{
public:
	GA();
	~GA();
	void initGene();
	void culcFitness();
	void output(int generation);
	void uniformCrossover();
	int selectIndividual();

private:
	/*
		�K���x�̌v�Z�w�W
			0:���g�p
			1:�K���x�v�Z�O�i�ˑR�ψق͂��̌̂����ɓK���j
			2:�K���x�v�Z�ς݁i�������ɐe�Ƃ݂Ȃ��j
		@TODO:enum��p�ӂ���
	*/
	std::vector<int> fitnessIndex;
	std::vector<double> fitness;
	double meanFitness;
	double maxFitness;
	int maxFitnessNumber;
	int population;
	int geneLength;
	std::vector<std::vector<int>> allIndividual;
	double _binary2Phenotype(int* binary);
	bool _isDuplicatedGene(unsigned int **gene);
	// @TODO:setObjectiveFunc��ǉ�������
	double _getObjectiveFunc(double x);	// 1�ϐ��̏ꍇ
};

#endif // !__GA_H_INCLUDE__