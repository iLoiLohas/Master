#ifndef __GA_H_INCLUDE__
#define __GA_H_INCLUDE__

#include <stdlib.h>
#include <random>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

class GA
{
public:
	GA(int numVariable);
	~GA();
	void initGene();
	void culcFitness();
	void outputGeneration(int generation);
	void uniformCrossover();
	int selectIndividual();
	void mutation(double mutationRate);
	void selectRanking();

private:
	std::vector<std::vector<int> > allIndividual;
	std::vector<double> fitness;
	double _meanFitness;	// �g���ĂȂ�
	double _maxFitness;	// �g���ĂȂ�
	int _maxFitnessNumber;	// �g���ĂȂ�
	int _numVariable;
	int _population;
	int _geneLength;

	void _setPopulation(int population);
	void _setGeneLength(int length);
	void _setNumVariable(int num);
	double _getObjectiveFunc(const std::vector<double> &var, int num);	// 1�ϐ��̏ꍇ
	double _binary2Phenotype(const std::vector<int>);
	bool _isDuplicatedGene(std::vector <std::vector<int> > gene, int column);
	void _outputIndividuals(std::vector <std::vector<int> > individuals);
	void _outputIndividual(std::vector<int> individual);

	/*** NSGA-2�p ***/
	std::vector< std::vector<int> > _searchPopulation;
	std::vector< std::vector<int> > _archivePopulation;
	void _initSearchPopulation();
	void _joinGene(const std::vector <std::vector<int> >, const std::vector <std::vector<int> >, std::vector <std::vector<int> >);

};

#endif // !__GA_H_INCLUDE__