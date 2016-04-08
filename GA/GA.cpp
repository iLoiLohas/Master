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
bool GA::_isDuplicatedGene(std::vector<std::vector<int>> gene, int column)
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

	this->meanFitness = 0.0;
	this->maxFitness = 0.0;
	this->maxFitnessNumber = 0;
	for (tmp_column = 0; tmp_column < this->population; tmp_column++)
	{
		switch (this->fitnessIndex[tmp_column])
		{
		case 0:
			// �����s��Ȃ�
			break;
		case 1:
			x = this->_binary2Phenotype(allIndividual[tmp_column]);
			this->fitness[tmp_column] = this->_getObjectiveFunc(x);
			this->fitnessIndex[tmp_column] = ;
			break;
		case 2:
			this->meanFitness += this->fitness[tmp_column];
			if (this->fitness[tmp_column] > this->maxFitness)
			{
				this->maxFitness = this->fitness[tmp_column];
				this->maxFitnessNumber = tmp_column;
			}
			break;
		default:
			break;
		}
	}
}

/*
	2�i���f�[�^��\���^�ɕϊ�
*/
double GA::_binary2Phenotype(std::vector<int> binary)
{
	// �J�E���g�ϐ�
	size_t tmp;

	double decimal;
	int place,  numRow;

	decimal		= 0.0;
	place	= 0;
	for (tmp = binary.size(); tmp >= 0; tmp--)
	{
		x = 0.0;
		place = 0.0;
		if (binary[tmp] > 0)
			decimal += pow(2.0, (double)place);
		place += 1;
	}

	return decimal * 1.0 / (pow(2.0, (double)this->geneLength) - 1.0);
}

double GA::_getObjectiveFunc(double x)
{
	return sin(3.0*x) + 0.5*sin(9.0*x) + sin(15.0*x + 50.0);
}

void GA::output(int generation)
{
	size_t tmp_column;

	std::cout << "��" << generation << "����" << std::endl;
	for (tmp_column = 0; tmp_column < this->geneLength; tmp_column++)
	{
		x = this->_binary2Phenotype(allIndividual[tmp_column]);
		std::cout << tmp_column << "\t" << x << "\t" << this->_getObjectiveFunc(x) << std::endl;
	}
}

void GA::uniformCrossover()
{
	// �J�E���g�ϐ�
	size_t tmp;

	int parent1, parent2;
	std::vector<int> maskPattern, parent1Individual, parent2Individual, childIndividual;
	
	// �e��I��
	do
	{
		parent1 = this->selectIndividual();
		parent2 = this->selectIndividual();
	} while (parent1 == parent2);
	parent1Individual.push_back(this->allIndividual[parent1]);
	parent2Individual.push_back(this->allIndividual[parent2]);

	// �}�X�N�p�^�[���𐶐�
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_real_distribution<int> randomValue(0, 1);
	for (tmp = 0; tmp < this->geneLength; tmp++)
	{
		maskPattern.push_back(randomValue(mt));
	}

	// ����
	for (tmp = 0; tmp < this->geneLength; tmp++)
	{
		if (maskPattern[tmp] == 0)
		{
			childIndividual.push_back(parent1Individual[tmp]);
		}
		else if (maskPattern[tmp] == 1)
		{
			childIndividual.push_back(parent2Individual[tmp]);
		}
	}

	// �̏W�c�Ɏq����ǉ�
	this->allIndividual.push_back(childIndividual);
	this->fitnessIndex.push_back(1);
}

/*
	�K���x���헪����ő̂�I�����C���̔ԍ���Ԃ��D
	���s�����ꍇ��-1��Ԃ��D
*/
int GA::selectIndividual()
{
	// �J�E���g�ϐ�
	size_t tmp_column;

	std::vector<std::vector<int>> evaluatedIndividual;
	std::vector<double> evaluatedFitness, fitnessRatio, selectedIndividual;
	double sumFitness = 0.0, x;
	int individualNum;

	// �K���x���]���ς݂̌̏W�c�����
	for (tmp_column = 0; tmp_column < this->population; tmp_column++)
	{
		if (this->fitnessIndex[tmp_column] == 2)
		{
			evaluatedIndividual.push_back(this->allIndividual[tmp_column]);
		}
	}

	// �ő̏W�c���V���b�t�����ď��Ԃ�ς����ɓK���x�����߂�
	// @MEMO:�Q�d�z��̂Ƃ���size()�̋���
	std::shuffle(evaluatedIndividual.begin(), evaluatedIndividual.end(), std::mt19937());
	for (tmp_column = 0; tmp_column < evaluatedIndividual.size(); tmp_column++)
	{
		x = this->_binary2Phenotype(evaluatedIndividual[tmp_column]);
		evaluatedFitness.push_back(this->_getObjectiveFunc(x));
		sumFitness += this->_getObjectiveFunc(x);
	}

	// �K���x�̔�
	for (tmp_column = 0; tmp_column < evaluatedFitness.size(); tmp_column++)
	{
		fitnessRatio.push_back(evaluatedFitness[tmp_column] / sumFitness);
	}

	// �K���x����m���I�Ɍő̂�I��
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_real_distribution<double> randomValue(0.0, 1.0);
	int selectedFlg = 0;
	do
	{
		for (tmp_column = 0; tmp_column < fitnessRatio.size(); tmp_column++)
		{
			if (fitnessRatio[tmp_column] > randomValue(mt))	// ���񃉃��_���l�����������	
			{
				selectedIndividual.push_back(evaluatedIndividual[tmp_column]);
				selectedFlg = 1;
				break;
			}
		}
	} while (selectedFlg == 0);

	// �I�΂ꂽ�̂̔ԍ���Ԃ�
	bool correspond = false;
	for (individualNum = 0; individualNum < this->population; individualNum++)
	{
		correspond = std::equal(this->allIndividual[tmp_column].cbegin(), this->allIndividual[tmp_column].cend(), selectedIndividual.cbegin());
		if (correspond)
		{
			return individualNum;
		}
	}

	return -1;	// �ő̂����݂��Ȃ������ꍇ
}

void GA::mutation(double mutationRate)
{
	// �J�E���g�ϐ�
	size_t tmp_column, tmp_row;

	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_real_distribution<double> randomValue(0.0, 1.0);

	for (tmp_column = 0; tmp_column < this->population; tmp_column++)
	{
		if (this->fitnessIndex[tmp_column] == 1)
		{
			for (tmp_row = 0; tmp_row < this->geneLength; tmp_row++)
			{
				if (mutationRate > randomValue(mt))
				{
					switch (this->allIndividual[tmp_column][tmp_row])
					{
					case 0:
						this->allIndividual[tmp_column][tmp_row] = 1;
					case 1:
						this->allIndividual[tmp_column][tmp_row] = 0;
					default:
						break;
					}
				}
			}
		}
	}
}