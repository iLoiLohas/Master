#include "GA.h"


GA::GA()
{
}

GA::~GA()
{
}

void GA::initGene()
{
	// �J�E���g�ϐ�
	size_t tmp, tmp_column, tmp_row;

	int duplicateFlg;

	// [0.0, 1.0]�̃����_���l���쐬
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> randomValue(0.0, 1.0);

	// @TODO:�q���̐�����������H
	for (tmp = 0; tmp < population; tmp++)
		fitnessIndex[tmp] = 1;

	do {
		duplicateFlg = 0;
		for (tmp_column = 0; tmp_column < this->population; tmp_column++)
		{
			// ��`�q�̍쐬
			for (tmp_row = 0; tmp_row < this->geneLength; tmp_row++)
			{
				allIndividual[tmp_column][tmp_row] = (randomValue(mt) > 0.5) ? 1 : 0;
			}

			if (this->_isDuplicatedGene(allIndividual, tmp_column))
				duplicateFlg = 1;
		}
	} while (duplicateFlg = 1)
}

/*
@param gene �̏W�c��2�����z��
*/
bool GA::_isDuplicatedGene(unsigned int **gene, int column)
{
	// �J�E���g�ϐ�
	size_t tmp_column, tmp_row;

	int duplicateFlg;
	bool result = false;

	for (tmp_column = 0; tmp_column < column; tmp_column++)
	{
		duplicateFlg = 1;
		for (tmp_row = 0; tmp_row < this->geneLength; tmp_row++)
		{
			// 1�ł���`�q���قȂ�΃t���O�����
			if (gene[column][tmp_row] != gene[tmp_column][tmp_row])
				duplicateFlg = 0;
		}

		if (duplicateFlg = 1)
			result = true;
	}

	return result;
}

void GA::culcFitness()
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	double x;

	for (tmp_column = 0; tmp_column < this->population; tmp_column++)
	{
		switch (this->fitnessIndex[tmp_column])
		{
		case 0:
			// @TODO:max��mean�̓K���x�����߂�

			break;
		case 1:
			x = this->_binary2Decimal(allIndividual[tmp_column]) * 1.0 / (pow(2.0, (double)this->geneLength) - 1.0);
			this->fitness[tmp_column] = this->_getObjectiveFunc(x);
			this->fitnessIndex[tmp_column] = ;
			break;
		default:
			break;
		}
	}
}

double GA::_binary2Decimal(int* binary)
{
	// �J�E���g�ϐ�
	size_t tmp;

	double decimal;
	int place,  numRow;

	numRow	= ARRAY_LENGTH(binary);
	decimal		= 0.0;
	place	= 0;
	for (tmp = numRow; tmp >= 0; tmp--)
	{
		x = 0.0;
		place = 0.0;
		if (binary[tmp] > 0)
			decimal += pow(2.0, (double)place);
		place += 1;
	}

	return decimal;
}

double GA::_getObjectiveFunc(double x)
{
	return sin(3.0*x) + 0.5*sin(9.0*x) + sin(15.0*x + 50.0);
}

void GA::output(int generation)
{
	size_t tmp_column, tmp_row;

	// �y����z�o�͕��@���l����
	for (tmp_column = 0; tmp_column < this->geneLength; tmp_column++)
	{

	}
}