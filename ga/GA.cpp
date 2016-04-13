#include "GA.h"


GA::GA(int population, int geneLength)
{
	std::cout << "Calls constructor." << std::endl;

	size_t tmp;

	this->_setPopulation(population);
	this->_setGeneLength(geneLength);

	// �̈�m��
	this->allIndividual.resize(population);
	for (tmp = 0; tmp < population; tmp++)
	{
		this->allIndividual[tmp].resize(geneLength);
	}

	// �̈�m��
	this->fitness.resize(population);
}

GA::~GA()
{
}

void GA::_setPopulation(int population)
{
	this->_population = population;
}

void GA::_setGeneLength(int length)
{
	this->_geneLength = length;
}

int GA::_getPopulation()
{
	return this->_population;
}

int GA::_getGeneLength()
{
	return this->_geneLength;
}

void GA::initGene()
{
	// �J�E���g�ϐ�
	int tmp, tmp_column, tmp_row;

	int duplicateFlg;

	// [0.0, 1.0]�̃����_���l���쐬
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> randomValue(0.0, 1.0);

	do {
		duplicateFlg = 0;
		for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
		{
			// ��`�q�̍쐬
			for (tmp_row = 0; tmp_row < this->_geneLength; tmp_row++)
			{
				this->allIndividual.at(tmp_column).at(tmp_row) = (randomValue(mt) > 0.5) ? 1 : 0;
			}

			if (this->_isDuplicatedGene(this->allIndividual, tmp_column))
				duplicateFlg = 1;
		}
	} while (duplicateFlg == 1);
}

/*
	�̏W�c��\������D
	�e�X�g�p
*/
void GA::_outputIndividuals(std::vector<std::vector<int>> individuals)
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		for (tmp_row = 0; tmp_row < this->_geneLength; tmp_row++)
		{
			std::cout << individuals[tmp_column][tmp_row];
		}
		std::cout << std::endl;
	}
}

/*
	@param gene �̏W�c��2�����z��
*/
bool GA::_isDuplicatedGene(std::vector<std::vector<int>> gene, int column)
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	int duplicateFlg;
	bool result = false;

	for (tmp_column = 0; tmp_column < column; tmp_column++)
	{
		duplicateFlg = 1;
		for (tmp_row = 0; tmp_row < this->_geneLength; tmp_row++)
		{
			// 1�ł���`�q���قȂ�΃t���O�����
			if (gene[column][tmp_row] != gene[tmp_column][tmp_row])
				duplicateFlg = 0;
		}

		if (duplicateFlg == 1)
			result = true;
	}

	return result;
}

void GA::culcFitness()
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	double x;
	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		x = this->_binary2Phenotype(allIndividual[tmp_column]);
		this->fitness[tmp_column] = this->_getObjectiveFunc(x);
	}
}

/*
	2�i���f�[�^��\���^�ɕϊ�
*/
double GA::_binary2Phenotype(std::vector<int> binary)
{
	// �J�E���g�ϐ�
	int tmp;

	double decimal;
	int place,  numRow;

	decimal		= 0.0;
	place	= 0;
	for (tmp = binary.size() - 1; tmp >= 0; --tmp)
	{
		if (binary[tmp] == 1)
			decimal += pow(2.0, (double)place);
		place += 1;
	}

	return decimal * 1.0 / (pow(2.0, (double)this->_geneLength) - 1.0);
}

double GA::_getObjectiveFunc(double x)
{
	return sin(3.0*x) + 0.5*sin(9.0*x) + sin(15.0*x + 50.0);
}

/*
	�w�肵������̌̏W�c��x�ƓK���x��\������B
*/
void GA::outputGeneration(int generation)
{
	size_t tmp_column;

	double x;

	std::cout << generation << "-generation" << std::endl;
	std::cout << "number\tx\tfitness" << std::endl;
	for (tmp_column = 0; tmp_column < this->_geneLength; tmp_column++)
	{
		x = this->_binary2Phenotype(allIndividual[tmp_column]);
		std::cout << tmp_column << "\t" << x << "\t" << this->_getObjectiveFunc(x) << std::endl;
	}
}

void GA::uniformCrossover()
{
	// �J�E���g�ϐ�
	int tmp;

	int parent1, parent2;
	std::vector<int> maskPattern, parent1Individual, parent2Individual, child1Individual, child2Individual;

	// �e��I��
	do
	{
		parent1 = this->selectIndividual();
		parent2 = this->selectIndividual();
		if (parent1 == -1 || parent2 == -1)
		{
			std::cout << "Couldn't select individual." << std::endl;
			return;
		}
	} while (parent1 == parent2);

	return;
	
	for (tmp = 0; tmp < this->_geneLength; tmp++)
	{
		parent1Individual.push_back(this->allIndividual[parent1][tmp]);
		parent2Individual.push_back(this->allIndividual[parent2][tmp]);
	}

	// �}�X�N�p�^�[���𐶐�
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_int_distribution<int> randomValue(0, 1);
	for (tmp = 0; tmp < this->_geneLength; tmp++)
	{
		maskPattern.push_back(randomValue(mt));
	}

	// ����
	for (tmp = 0; tmp < this->_geneLength; tmp++)
	{
		if (maskPattern[tmp] == 0)
		{
			child1Individual.push_back(parent1Individual[tmp]);
			child2Individual.push_back(parent2Individual[tmp]);
		}
		else if (maskPattern[tmp] == 1)
		{
			child1Individual.push_back(parent2Individual[tmp]);
			child2Individual.push_back(parent1Individual[tmp]);
		}
	}

	// ������
	this->allIndividual[parent1].swap(child1Individual);
	this->allIndividual[parent2].swap(child2Individual);
}

/*
	�K���x���헪����ő̂�I�����C���̔ԍ���Ԃ��D
	���s�����ꍇ��-1��Ԃ��D
*/
int GA::selectIndividual()
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	// �̏W�c�̃R�s�[���쐬
	std::vector<std::vector<int>> shuffledAllIndividual;
	std::copy(this->allIndividual.begin(), this->allIndividual.end(), back_inserter(shuffledAllIndividual));

	std::vector<double> shuffledFitness, fitnessRatio, selectedIndividual;

	// �ő̏W�c���V���b�t�����ď��Ԃ�ς����ɓK���x�����߂�
	double sumFitness = 0.0, x;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(shuffledAllIndividual.begin(), shuffledAllIndividual.end(), std::mt19937(seed));
	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		x = this->_binary2Phenotype(shuffledAllIndividual[tmp_column]);
		shuffledFitness.push_back(this->_getObjectiveFunc(x));
		sumFitness += this->_getObjectiveFunc(x);
	}

	// �K���x�̔�
	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		fitnessRatio.push_back(shuffledFitness[tmp_column] / sumFitness);
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
				for (tmp_row = 0; tmp_row < this->_geneLength; tmp_row++)
				{
					selectedIndividual.push_back(shuffledAllIndividual[tmp_column][tmp_row]);
				}
				selectedFlg = 1;
				break;
			}
		}
	} while (selectedFlg == 0);

	// �I�΂ꂽ�̂̔ԍ���Ԃ�
	int individualNum;
	bool correspond = false;
	for (individualNum = 0; individualNum < this->_population; individualNum++)
	{
		correspond = std::equal(shuffledAllIndividual[individualNum].cbegin(), shuffledAllIndividual[individualNum].cend(), selectedIndividual.cbegin());
		if (correspond)
		{
			return individualNum;
		}
	}

	return -1;	// �ő̂����݂��Ȃ������ꍇ
}

/*
	�����L���O�@�����ƂɑI�𓑑����s���D
*/
void GA::selectRanking()
{
	// �J�E���g�ϐ�
	int tmp, tmp1, tmp2;

	// �\�[�g�����L�����邽�߂̃L�[
	std::vector<int> key;
	key.resize(this->_population);
	for (tmp = 0; tmp < this->_population; tmp++)
		key[tmp] = tmp;

	// �K���x���~���Ƀ\�[�g�D���̏��Ԃ�key�ɕۑ��D
	double tmpVar;
	int tmpKey;
	for (tmp1 = 0; tmp1 < this->fitness.size(); ++tmp1)
	{
		for (tmp2 = tmp1+1; tmp2 < this->fitness.size(); ++tmp2)
		{
			if (this->fitness[tmp1] < this->fitness[tmp2])
			{
				tmpVar = this->fitness[tmp1];
				this->fitness[tmp1] = this->fitness[tmp2];
				this->fitness[tmp2] = tmpVar;

				tmpKey		= key[tmp1];
				key[tmp1]	= key[tmp2];
				key[tmp2]	= tmpKey;
			}
		}
	}

	// �̏W�c��K���x�̍~���ɕ��ׂ�
	for (tmp = 0; tmp < this->_geneLength; tmp++)
	{
		this->allIndividual[tmp] = this->allIndividual[key[tmp]];
	}

	// �K���x�����2�̌̂��R�s�[���C����2�𓑑�����D
	this->allIndividual[this->_geneLength - 1]	= this->allIndividual[0];
	this->allIndividual[this->_geneLength - 2]	= this->allIndividual[0];
}

/*
	5�Ԗڂ̌̂ɓˑR�ψق��s���D
	�K���x�v�Z�O�̌̂̈�`�q(0 or 1)�������_���ɓ���ւ���
	@param mutationRate �ˑR�ψٗ�
*/
void GA::mutation(double mutationRate)
{
	// �J�E���g�ϐ�
	size_t tmp_row;

	int column = 5;

	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_real_distribution<double> randomValue(0.0, 1.0);

	for (tmp_row = 0; tmp_row < this->_geneLength; tmp_row++)
	{
		if (mutationRate > randomValue(mt))
		{
			switch (this->allIndividual[column][tmp_row])
			{
			case 0:
				this->allIndividual[column][tmp_row] = 1;
			case 1:
				this->allIndividual[column][tmp_row] = 0;
			default:
				break;
			}
		}
	}
}