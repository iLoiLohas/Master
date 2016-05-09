/***********
	GA.cpp

	�K��i2016/05/09�j
		- std::vector< std::vector<int> >�̕ϐ�����~Population
************/

#include "Ga.h"
#include "GaCommon.h"


GA::GA(int numVariable)
{
	std::cout << "Calls constructor." << std::endl;

	int geneLength	= 20;	// ��`�q��
	int population	= 120;	// �̐�

	this->_population	= population;
	this->_geneLength	= geneLength;
	this->_numVariable	= numVariable;

	this->_searchPopulation	= std::vector< std::vector<int> >(population, std::vector<int>(geneLength));
	this->fitness			= std::vector<int>(population);
}

GA::~GA()
{
}

/*
	����������
*/
void GA::initGene()
{
	this->_initSearchPopulation();
	this->_archivePopulation	= std::vector< std::vector<int> >(this->_population, std::vector<int>(this->_geneLength*this->_numVariable));
}

/*
	�T����W�c������������D
*/
void GA::_initSearchPopulation()
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

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
			for (tmp_row = 0; tmp_row < this->_geneLength*this->_numVariable; tmp_row++)
			{
				this->_searchPopulation.at(tmp_column).at(tmp_row) = (randomValue(mt) > 0.5) ? 1 : 0;
			}

			if (this->_isDuplicatedGene(this->_searchPopulation, tmp_column))
				duplicateFlg = 1;
		}
	} while (duplicateFlg == 1);	
}

/*
	�̏W�c��\������D
	�e�X�g�p
*/
void GA::_outputIndividuals(std::vector<std::vector<int>> &individuals)
{
	// �J�E���g�ϐ�
	int tmp_column, tmp_row;

	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		for (tmp_row = 0; tmp_row < this->_geneLength*this->_numVariable; tmp_row++)
		{
			std::cout << individuals[tmp_column][tmp_row];
		}
		std::cout << std::endl;
	}
}

/*
	@param gene �̏W�c��2�����z��
*/
bool GA::_isDuplicatedGene(std::vector<std::vector<int>> &gene, int column)
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

/*
	1�̂�2�i���f�[�^��\���^�ɕϊ�
	2�ڂ̈����Ɍ��ʂ��i�[
	@param &binary 1�̂̈�`�q
	@param &phenotype ��`�q�^��\���^�ɕϊ���������
*/
void GA::_convertPhenotype(const std::vector<int> &binary, std::vector<double> &phenotype)
{
	// �J�E���g�ϐ�
	int tmp_var, tmp_cnt;

	std::vector<int> tmpGene(this->geneLength);

	for (tmp_cnt = 0; tmp_cnt < this->_numVariable; ++tmp_cnt)
	{
		for (tmp_var = 0; tmp_var < this->_geneLength; ++tmp_var)
		{
			tmpGene.push_back(binary[tmp_column][tmp_cnt*this->_geneLength + tmp_var]);
		}
		phenotype.push_back(this->_binary2Phenotype(tmpGene));
		tmpGene.clear();
	}
}

/*
	1��`�q��2�i���f�[�^��\���^�ɕϊ�
	@param &binary 1��`�q���̒���������2�i���f�[�^
*/
double GA::_binary2Phenotype(const std::vector<int> &binary)
{
	// �J�E���g�ϐ�
	int tmp;

	double decimal	= 0.;
	int place	= 0;

	for (tmp = this->_geneLength - 1; tmp >= 0 ; --tmp)
	{
		if (binary[tmp] == 1)
		{
			decimal	+= pow(2.0, (double)place);
			place	+= 1;
		}
	}

	return decimal / (pow(2.0, (double)this->_geneLength) - 1.0);
}

/*
	�ړI�֐����v�Z
	@param &var �ϐ�
	@param &obj �ړI�֐��̒l�i�ړI�֐��̐������̈���m�ۂ��Ă����j
*/
void GA::_getObjectiveFunc(const std::vector<double> &var, std::vector<double> &obj)
{
	obj.at(0)	= this->_f1(var);
	obj.at(1)	= this->_f2(var);
}

double GA::_f1(const std::vector<double> &var)
{
	return 1. - exp(-4.*var[0])*pow(sin(6.*M_PI*var[0]),6);
}

double GA::_f2(const std::vector<double> &var)
{
	double g, h;

	g = 1. + 9.*pow((var[1]+var[2]+var[3]+var[4]+var[5]+var[6]+var[7]+var[8]+var[9])/(9.),0.25);
	h = 1. - pow(this->_f1(var)/g,2);

	return g*h;
}

/*
	1�̂���ړI�֐��l���v�Z
	@param &binary 1�̂̈�`�q
	@param &obj �ړI�֐��̒l�i�ړI�֐��̐������̈���m�ۂ��Ă����j
*/
void GA::_binary2ObjectiveFunc(const std::vector<int> &binary, std::vector<double> &obj)
{
	// �\���^���ꎞ�I�ɕۑ�
	std::vector<double> tmpPhenotype(this->_numVariable);

	this->_convertPhenotype(binary, tmpPhenotype);
	this->_getObjectiveFunc(tmpPhenotype, obj);
}

/***************
	���e�X�g�֐�
***************/

/*
	NSGA2���s�p���\�b�h
*/
void GA::nsga2Run()
{
	/*** Step1 ***/
	this->_initGene();

	/*** Step2 ***/
	// TODO:�]�����@�̊m��

	/*** Step3 ***/
	std::vector<std::vector<int> > margedPopulation;
	GaCommon::joinPopulation(this->_archivePopulation, this->_searchPopulation, margedPopulation);
	std::vector<std::vector<std::vector<int> > > classifiedByRankGene;
	this->_nonSuperioritySort(margedPopulation, classifiedByRankGene);

	/*** Step4 ***/
	std::vector<std::vector<int> > newArchivePopulation, nextRankPopulation;
	this->_updateArchivePopulation(classifiedByRankGene, newArchivePopulation, nextRankPopulation);

	/*** Step5 ***/
	std::vector<std::vector<int> > crowdingSortedPopulation;
	this->_crowdingSort(nextRankPopulation, crowdingSortedPopulation);
	this->_insertIndividuals(newArchivePopulation, nextRankPopulation);

	/*** Step6 ***/
	// TODO:�I��������ݒ�

	/*** Step7 ***/
	
	/*** Step8 ***/
}

/*
	��W�c�ɑ΂��Ĕ�D�z�\�[�g���s��
	@param gene �\�[�g���s����W�c
	@param &classifiedByRankGene �����N���ƂɃN���X���������̏W�c 
*/
void GA::_nonSuperioritySort(const std::vector <std::vector<int> > &targetPopulation, std::vector<std::vector<std::vector<int> > > &classifiedByRankGene)
{
	// �J�E���g�ϐ�
	int tmp, tmp_column;

	int num, numObj, superiorityFlg;
	std::vector<double> targetObject(2), comparedObject(2);	// �ړI�֐��̐�
	std::vector<std::vector<int> > sortingPopulation, tmpRankedGene;

	// �̂Ƀ����N��t���Ă����C�����N�t�����ꂽ�̂͏���
	std::copy(targetPopulation.begin(), targetPopulation.end(), std::back_inserter(sortingPopulation));

	while(sortingPopulation.size() > 0)
	{
		num	= 0;
		while(num < sortingPopulation.size())
		{
			superiorityFlg	= 1;
			this->_binary2ObjectiveFunc(sortingPopulation[num], targetObject);

			for (tmp_column = 0; tmp_column < objValue.size(); ++tmp_column)
			{
				if (num == tmp_column)
					continue;
				this->_binary2ObjectiveFunc(sortingPopulation[tmp_column], comparedObject);

				for (numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
					if (targetObject[numObj] < comparedObject[numObj])
						superiorityFlg	= 0;
			}

			if (superiorityFlg == 1)
				tmpRankedGene.push_back(sortingPopulation[num]);
			targetObject.clear();
			num += 1;
		}

		// �����N���ƂɌ̂�ۑ����C�̌Q���X�V
		classifiedByRankGene.push_back(tmpRankedGene);
		for (tmp = 0; tmp < tmpRankedGene.size(); ++tmp)
			GaCommon::removeElement(sortingPopulation, tmpRankedGene[tmp]);
		tmpRankedGene.clear();
	}
}

/*
	�V���ȃA�[�J�C�u�W�c�𐶐�
	@param &classifiedByRankGene �����N���Ƃ̌�
	@param &newArchivePopulation �����N��ʂ���擾�����X�V�p�A�[�J�C�u��W�c
	@param &nextRankPopulation �A�[�J�C�u��W�c�ɓ��肫��Ȃ������ō������N�̌̏W�c
*/
void GA::_updateArchivePopulation(const std::vector<std::vector<std::vector<int> > > &classifiedByRankGene, std::vector<std::vector<int> > &newArchivePopulation, std::vector<std::vector<int> > &nextRankPopulation)
{
	for (int rank = 0; rank < classifiedByRankGene.size(); ++rank)
	{
		if (this->_population - newArchivePopulation.size() > classifiedByRankGene[rank].size())
			newArchivePopulation.push_back(classifiedByRankGene[rank]);
		else
			break;
	}

	// �A�[�J�C�u��W�c�ɓ��肫��Ȃ������ō������N�̌̏W�c���i�[
	std::copy(classifiedByRankGene[rank].begin(), classifiedByRankGene[rank].end(), std::back_inserter(nextRankPopulation));
}

/*
	���G�x�\�[�g
	@param &certainRankPopulation ���郉���N�̌̌Q
	@param &crowdingSortedPopulation ���G�x���ƂɃ\�[�g���ꂽ�̌Q
*/
void GA::_crowdingSort(const std::vector<std::vector<int > > &certainRankPopulation, std::vector<std::vector<int> > &crowdingSortedPopulation)
{
	// �J�E���g�ϐ�
	int tmp1, tmp2;

	int numObj;
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);
	std::vector<double> tmpObjectFunc(2);	// �ړI�֐��̐�
	std::vector<std::vector<std::vector<int> > > sortedObjectiveFunc(2);	// �ړI�֐��̐�	

	// �ړI�֐����ƂɖړI�֐��l���������ɕ��ׂ�
	for (numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
	{
		this->_sortRegardingObjective(certainRankPopulation, sortedObjectiveFunc[numObj], numObj);
		std::reverse(sortedObjectiveFunc[numObj].begin(), sortedObjectiveFunc[numObj].end());
	}

	// ���G�x���傫�����Ɍő̂��\�[�g
	std::copy(certainRankPopulation.begin(), certainRankPopulation.end(), std::back_inserter(crowdingSortedPopulation));
	double distance1 = 0., distance2 = 0.;
	for (tmp1 = 1; tmp1 < certainRankPopulation.size()-1; ++tmp1)
	{
		for (tmp2 = certainRankPopulation.size()-2;  tmp2 > tmp1; --tmp2)
		{
			distance1	= this->_culcCrowdingDistanseForIndividual(sortedObjectiveFunc, crowdingSortedPopulation[tmp2]);
			distance2	= this->_culcCrowdingDistanseForIndividual(sortedObjectiveFunc, crowdingSortedPopulation[tmp2-1]);
			if (distance1 > distance2)
			{
				tmpGene	= crowdingSortedPopulation[tmp2];
				crowdingSortedPopulation[tmp2]	= crowdingSortedPopulation[tmp2-1];
				crowdingSortedPopulation[tmp2-1]	= tmpGene;
			}
		}
	}
}

/*
	�w�肵���ړI�֐��l�����������Ɍ̂��o�u���\�[�g����
	@param &targetGene �\�[�g�������̌Q
	@param &sortedGene �\�[�g��̌̌Q
	@param num �ΏۂƂ���ړI�֐��̔ԍ�
*/
void GA::_sortRegardingObjective(const std::vector<std::vector<int> > &targetGene, std::vector<std::vector<int> > &sortedGene, int num)
{
	// �J�E���g�ϐ�
	int tmp1, tmp2;

	std::vector<double> tmpObject1(2);	// �ړI�֐��̐�
	std::vector<double> tmpObject2(2);	// �ړI�֐��̐�
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);

	std::copy(targetGene.begin(), targetGene.end(), std::back_inserter(sortedGene));

	for (tmp1 = 0; tmp1 < targetGene.size(); ++tmp1)
	{
		for (tmp2 = targetGene.size() - 1; tmp2 > tmp1; --tmp2)
		{
			this->_binary2ObjectiveFunc(targetGene[tmp2], tmpObject1);
			this->_binary2ObjectiveFunc(targetGene[tmp2-1], tmpObject2);
			if (tmpObject1[num] < tmpObject2[num])
			{
				tmpGene	= sortedGene[tmp2];
				sortedGene[tmp2]	= sortedGene[tmp2-1];
				sortedGene[tmp2-1]	= tmpGene;
			}
		}
	}
}

/*
	�w�肵���ړI�֐��ɑ΂��č��G�x���v�Z����
	@param &objectiveSortedGene �ړI�֐����ƂɃ\�[�g���ꂽ�̌Q
	@param numGene �̂̔ԍ��i���E�̂͑I���ł��Ȃ��j
	@param numObj �ړI�֐��̔ԍ�
*/
double GA::_culcCrowdingDistanse(const std::vector<std::vector<std::vector<int> > > &objectiveSortedGene, int numGene, int numObj)
{
	std::vector<double> tmpObjLeft(2);		// �ړI�֐��̐�
	std::vector<double> tmpObjRight(2);	// �ړI�֐��̐�
	std::vector<double> tmpObjMax(2);		// �ړI�֐��̐�
	std::vector<double> tmpObjMin(2);		// �ړI�֐��̐�

	// ���E�̂������Č̂�I��
	double distance	= 0.;
	this->_binary2ObjectiveFunc(objectiveSortedGene[numObj][numGene-1], tmpObjLeft);
	this->_binary2ObjectiveFunc(objectiveSortedGene[numObj][numGene+1], tmpObjRight);
	this->_binary2ObjectiveFunc(objectiveSortedGene[numObj][0], tmpObjMax);
	this->_binary2ObjectiveFunc(objectiveSortedGene[numObj][objectiveSortedGene.front().size()-1], tmpObjMin);
	distance	= (tmpObjLeft[numObj] - tmpObjRight[numObj])/(tmpObjMax[numObj] - tmpObjMin[numObj]);

	return distance;
}

/*
	�w�肵���̂̑����G�x���v�Z����
	@param &objectiveSortedGene �ړI�֐����ƂɃ\�[�g���ꂽ�̌Q
	@param &individual �̂̈�`�q
*/
double GA::_culcCrowdingDistanseForIndividual(const std::vector<std::vector<std::vector<int> > > &objectiveSortedGene, const std::vector<int> &individual)
{
	int numObj, numGene;
	std::vector<double> distance(2);	// �ړI�֐��̐�

	for (numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
	{
		for (numGene = 1; numGene < objectiveSortedGene.front().size()-1; ++numGene)
		{
			if (objectiveSortedGene[numObj][numGene] == individual)
			{
				distance[numObj]	= this->_culcCrowdingDistanse(objectiveSortedGene, numGene, numObj);
				break;
			}
		}
	}

	double resultDistanse	= 0.;
	for (numObj = 0; numObj < distance.size(); ++numObj)
	{
		resultDistanse	+= distance[numObj];
	}

	return  resultDistanse;
}

/*
	�̐���N�ɂȂ�܂Ō̂�ǉ�����
	@param &insertedPopulation �̂�ǉ������̏W�c�i�̐���N�ȉ��j
	@param &insertPopulation �ǉ�����̂��܂ތ̏W�c
*/
void GA::_insertIndividuals(std::vector<std::vector<int> > &insertedPopulation, const std::vector<std::vector<int> > &insertPopulation)
{
	if (insertedPopulation.size() >= this->_population)
	{
		std::cout << "ERROR:Excess individual!!" << std::endl;
		return;
	}

	for (int tmp = 0; insertedPopulation.size() < this->_population; ++tmp)
	{
		insertedPopulation.push_back(insertPopulation[tmp]);
	}
}




/***************
	�Â��֐�
***************/

/*
	�w�肵������̌̏W�c��x�ƓK���x��\������B
*/
void GA::outputGeneration(int generation)
{
	size_t tmp_column;

	double x;

	std::cout << generation << "-generation" << std::endl;
	std::cout << "number\tx\tfitness" << std::endl;
	for (tmp_column = 0; tmp_column < this->_geneLength*this->_numVariable; tmp_column++)
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
	int tmp_column;

	std::vector<double> shuffledFitness, fitnessRatio, selectedIndividual;

	// �ő̏W�c���V���b�t�����ď��Ԃ�ς����ɓK���x�����߂�
	double sumFitness = 0.0, x;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(this->allIndividual.begin(), this->allIndividual.end(), std::mt19937(seed));
	for (tmp_column = 0; tmp_column < this->_population; tmp_column++)
	{
		x = this->_binary2Phenotype(this->allIndividual[tmp_column]);
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
	int selectedFlg = 0, individualNum = 0;
	do
	{
		for (individualNum = 0; individualNum < fitnessRatio.size(); individualNum++)
		{
			if (fitnessRatio[individualNum] > randomValue(mt))	// ���񃉃��_���l�����������	
			{
				return individualNum;
			}
		}
	} while (selectedFlg == 0);

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
