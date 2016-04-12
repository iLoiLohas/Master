#ifndef __GA_H_INCLUDE__
#define __GA_H_INCLUDE__

#include <stdlib.h>
#include <random>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

class GA
{
public:
	GA(int population, int geneLength);
	~GA();
	void initGene();
	void culcFitness();
	void outputGeneration(int generation);
	void uniformCrossover();
	int selectIndividual();
	void mutation(double mutationRate);
	void selectRanking();

private:
	std::vector<std::vector<int>> allIndividual;
	std::vector<double> fitness;
	double meanFitness;	// �g���ĂȂ�
	double maxFitness;	// �g���ĂȂ�
	int maxFitnessNumber;	// �g���ĂȂ�
	int _population;
	int _geneLength;

	void _setPopulation(int population);
	void _setGeneLength(int length);
	int _getPopulation();
	int _getGeneLength();
	// @TODO:setObjectiveFunc��ǉ�������
	double _getObjectiveFunc(double x);	// 1�ϐ��̏ꍇ
	double _binary2Phenotype(std::vector<int>);
	bool _isDuplicatedGene(std::vector<std::vector<int>> gene, int column);
	void _outputIndividuals(std::vector<std::vector<int>> individuals);
	void _outputIndividual(std::vector<int> individual);
};

#endif // !__GA_H_INCLUDE__