/***********
	GA.cpp

	�K��i2016/05/09�j
		- std::vector< std::vector<int> >�̕ϐ�����~Population
************/

#include "../include/Ga.h"
#include "../include/GaCommon.h"


GA::GA(int numVariable)
{
	std::cout << "Calls constructor." << std::endl;

	int geneLength	= 20;	// ��`�q��
	int population		= 120;	// �̐�

	this->_population	= population;
	this->_geneLength	= geneLength;
	this->_numVariable	= numVariable;

	this->_searchPopulation	= std::vector< std::vector<int> >(population, std::vector<int>(geneLength));
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
	@param &binary 1�̂̈�`�q
	@param &phenotype ��`�q�^��\���^�ɕϊ���������
*/
void GA::_convertPhenotype(const std::vector<int> &binary, std::vector<double> &phenotype)
{
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);

	for (int numVar = 0; numVar < this->_numVariable; ++numVar)
	{
		for (int numBinary = 0; numBinary < this->_geneLength; ++numBinary)
		{
			tmpGene.push_back(binary[numVar*this->_geneLength + numBinary]);
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
	@param &variable �ϐ�
	@param &objectiveValue �ړI�֐��̒l�i�ړI�֐��̐������̈���m�ۂ��Ă����j
*/
void GA::_getObjectiveFunc(const std::vector<double> &variable, std::vector<double> &objectiveValue)
{
	objectiveValue.at(0)	= this->_f1(variable);
	objectiveValue.at(1)	= this->_f2(variable);
}

double GA::_f1(const std::vector<double> &variable)
{
	return 1. - exp(-4.*variable[0])*pow(sin(6.*M_PI*variable[0]),6);
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
	int generation = 0;
	int maxGeneration	= 500;
	std::vector<std::vector<int> > margedPopulation, nextRankPopulation, crowdingSortedPopulation;
	std::vector<std::vector<std::vector<int> > > archivePopulation(maxGeneration), searchPopulation(maxGeneration), classifiedByRankGene;

	std::copy(this->_archivePopulation.begin(), this->_archivePopulation.end(), std::back_inserter(archivePopulation[0]));
	std::copy(this->_searchPopulation.begin(), this->_searchPopulation.end(), std::back_inserter(searchPopulation[0]));

	/*** Step1 ***/
	this->initGene();

	while(1)
	{
		/*** Step2 ***/
		// TODO:�]�����@�̊m��
		this->_outputObjectiveValue(searchPopulation[generation], generation);

		/*** Step3 ***/
		GaCommon::joinPopulation(archivePopulation[generation], searchPopulation[generation], margedPopulation);
		this->_nonSuperioritySort(margedPopulation, classifiedByRankGene);
		margedPopulation.clear();

		/*** Step4 ***/
		this->_updateArchivePopulation(classifiedByRankGene, archivePopulation[generation+1], nextRankPopulation);

		/*** Step5 ***/
		this->_crowdingSort(nextRankPopulation, crowdingSortedPopulation);
		this->_insertIndividuals(archivePopulation[generation+1], nextRankPopulation);
		nextRankPopulation.clear();
		crowdingSortedPopulation.clear();

		/*** Step6 ***/
		if (generation < maxGeneration)
			break;

		/*** Step7 ***/
		// �I���ƌ����𓯎��ɍs���Ă���
		this->_crowdedTournamentSelection(archivePopulation[generation+1], searchPopulation[generation+1], classifiedByRankGene);
		classifiedByRankGene.clear();

		/*** Step8 ***/
		this->_mutationGene(searchPopulation[generation+1], 1/this->_geneLength*this->_numVariable);

		generation	+= 1;
	}
}

/*
	��W�c�ɑ΂��Ĕ�D�z�\�[�g���s��
	@param gene �\�[�g���s����W�c
	@param &classifiedByRankGene �����N���ƂɃN���X���������̏W�c 
*/
void GA::_nonSuperioritySort(
	const std::vector <std::vector<int> > &targetPopulation,
	std::vector<std::vector<std::vector<int> > > &classifiedByRankGene)
{
	// �J�E���g�ϐ�
	int tmp, tmp_column;

	int numGene, numObj, superiorityFlg;
	std::vector<double> targetObject(2), comparedObject(2);	// �ړI�֐��̐�
	std::vector<std::vector<int> > sortingPopulation, tmpRankedGene;

	// �̂Ƀ����N��t���Ă����C�����N�t�����ꂽ�̂͏���
	std::copy(targetPopulation.begin(), targetPopulation.end(), std::back_inserter(sortingPopulation));
	while(sortingPopulation.size() > 0)
	{
		numGene	= 0;
		while(numGene < sortingPopulation.size())
		{
			superiorityFlg	= 1;
			this->_binary2ObjectiveFunc(sortingPopulation[numGene], targetObject);

			for (tmp_column = 0; tmp_column < sortingPopulation.size(); ++tmp_column)
			{
				if (numGene == tmp_column)
					continue;
				this->_binary2ObjectiveFunc(sortingPopulation[tmp_column], comparedObject);

				for (numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
					if (targetObject[numObj] < comparedObject[numObj])
						superiorityFlg	= 0;
			}

			if (superiorityFlg == 1)
				tmpRankedGene.push_back(sortingPopulation[numGene]);
			targetObject.clear();
			numGene += 1;
		}

		// �����N���ƂɌ̂�ۑ����C�̌Q���X�V
		classifiedByRankGene.push_back(tmpRankedGene);
		for (tmp = 0; tmp < tmpRankedGene.size(); ++tmp)
			GaCommonTemp<int>::removeElement(sortingPopulation, tmpRankedGene[tmp]);
		tmpRankedGene.clear();
	}
}

/*
	�V���ȃA�[�J�C�u�W�c�𐶐�
	@param &classifiedByRankGene �����N���Ƃ̌�
	@param &newArchivePopulation �����N��ʂ���擾�����X�V�p�A�[�J�C�u��W�c
	@param &nextRankPopulation �A�[�J�C�u��W�c�ɓ��肫��Ȃ������ō������N�̌̏W�c
*/
void GA::_updateArchivePopulation(
	const std::vector<std::vector<std::vector<int> > > &classifiedByRankGene,
	std::vector<std::vector<int> > &newArchivePopulation,
	std::vector<std::vector<int> > &nextRankPopulation)
{
	int rank;

	for (rank = 0; rank < classifiedByRankGene.size(); ++rank)
	{
		if (this->_population - newArchivePopulation.size() > classifiedByRankGene[rank].size())
			GaCommon::pushBackAll(newArchivePopulation, classifiedByRankGene[rank]);
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
void GA::_crowdingSort(
	const std::vector<std::vector<int > > &certainRankPopulation,
	std::vector<std::vector<int> > &crowdingSortedPopulation)
{
	// �J�E���g�ϐ�
	int tmp1, tmp2;

	int numObj;
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);
	std::vector<double> tmpObjectFunc(2);	// �ړI�֐��̐�
	std::vector<std::vector<std::vector<int> > > objectiveSortedGene(2);	// �ړI�֐��̐�	

	// �ړI�֐����ƂɖړI�֐��l���������ɕ��ׂ�
	this->_putObjectiveSortedGeneEveryObjectiveFunc(certainRankPopulation, objectiveSortedGene);

	// ���G�x���傫�����Ɍ̂��\�[�g
	std::copy(certainRankPopulation.begin(), certainRankPopulation.end(), std::back_inserter(crowdingSortedPopulation));
	double distance1 = 0., distance2 = 0.;
	for (tmp1 = 1; tmp1 < certainRankPopulation.size()-1; ++tmp1)
	{
		for (tmp2 = certainRankPopulation.size()-2;  tmp2 > tmp1; --tmp2)
		{
			distance1	= this->_culcCrowdingDistanseForIndividual(objectiveSortedGene, crowdingSortedPopulation[tmp2]);
			distance2	= this->_culcCrowdingDistanseForIndividual(objectiveSortedGene, crowdingSortedPopulation[tmp2-1]);
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
	�ړI�֐����ƂɖړI�֐��l���������ɕ��ׂ�
	@param &targetPopulation �\�[�g�������̌Q
	@param &objectiveSortedGene �ړI�֐����ƂɃ\�[�g���ꂽ�́i�ړI�֐��̐������̈���m�ۂ��Ă����j
*/
void GA::_putObjectiveSortedGeneEveryObjectiveFunc(
	const std::vector<std::vector<int> > &targetPopulation,
	std::vector<std::vector<std::vector<int> > > &objectiveSortedGene)
{
	int numObj;
	for (numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
	{
		this->_sortByObjectiveValue(targetPopulation, objectiveSortedGene[numObj], numObj);
		std::reverse(objectiveSortedGene[numObj].begin(), objectiveSortedGene[numObj].end());
	}
}

/*
	�w�肵���ړI�֐��l�����������Ɍ̂��o�u���\�[�g����
	@param &targetGene �\�[�g�������̌Q
	@param &objectiveSortedGene �\�[�g��̌̌Q
	@param num �ΏۂƂ���ړI�֐��̔ԍ�
*/
void GA::_sortByObjectiveValue(
	const std::vector<std::vector<int> > &targetGene,
	std::vector<std::vector<int> > &objectiveSortedPopulation,
	int num)
{
	// �J�E���g�ϐ�
	int tmp1, tmp2;

	std::vector<double> tmpObject1(2);	// �ړI�֐��̐�
	std::vector<double> tmpObject2(2);	// �ړI�֐��̐�
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);

	std::copy(targetGene.begin(), targetGene.end(), std::back_inserter(objectiveSortedPopulation));

	for (tmp1 = 0; tmp1 < targetGene.size(); ++tmp1)
	{
		for (tmp2 = targetGene.size() - 1; tmp2 > tmp1; --tmp2)
		{
			this->_binary2ObjectiveFunc(targetGene[tmp2], tmpObject1);
			this->_binary2ObjectiveFunc(targetGene[tmp2-1], tmpObject2);
			if (tmpObject1[num] < tmpObject2[num])
			{
				tmpGene	= objectiveSortedPopulation[tmp2];
				objectiveSortedPopulation[tmp2]	= objectiveSortedPopulation[tmp2-1];
				objectiveSortedPopulation[tmp2-1]	= tmpGene;
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
double GA::_culcCrowdingDistanse(
	const std::vector<std::vector<std::vector<int> > > &objectiveSortedGene,
	int numGene,
	int numObj)
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
	@param &objectiveSortedGene �ړI�֐����ƂɖړI�֐��n���������Ƀ\�[�g���ꂽ�̌Q
	@param &individual �̂̈�`�q
*/
double GA::_culcCrowdingDistanseForIndividual(
	const std::vector<std::vector<std::vector<int> > > &objectiveSortedGene,
	const std::vector<int> &individual)
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
void GA::_insertIndividuals(
	std::vector<std::vector<int> > &insertedPopulation,
	const std::vector<std::vector<int> > &insertPopulation)
{
	if (insertedPopulation.size() >= this->_population)
	{
		std::cout << "ERROR:Excess individual!!" << std::endl;
		return;
	}

	for (int tmp = 0; insertedPopulation.size() < this->_population; ++tmp)
		insertedPopulation.push_back(insertPopulation[tmp]);
}

/*
	���G�x�g�[�i�����g�I��
	@param &selectedPopulation �I�������̏W�c
	@param &newSearchPopulation �V���ȒT����W�c
	@param &classifiedByRankGene �����N���ƂɃN���X�������ꂽ��
*/
void GA::_crowdedTournamentSelection(
	const std::vector<std::vector<int> > &selectedPopulation,
	std::vector<std::vector<int> > &newSearchPopulation,
	const std::vector<std::vector<std::vector<int> > > &classifiedByRankGene)
{
	std::vector<int> parentGene1, parentGene2, childGene1, childGene2;
	std::vector<std::vector<int> > tmpSelectionPopulation, highRankPopulation;

	// �̐���N�ɂȂ�܂őI�������s
	// �e�̂������_���ɑI�����C��l���������s
	// �e�~2�Ǝq�~2�̂��������N����ʂ̌̂�1�I�����C�V���ȒT����W�c�ɒǉ�����
	for (int numGene = 0; newSearchPopulation.size() < this->_population; ++numGene)
	{
		this->_select2GenesFromPopulation(selectedPopulation, parentGene1, parentGene2);
		this->_uniformCrossover(parentGene1, parentGene2, childGene1, childGene2);

		tmpSelectionPopulation.push_back(parentGene1);
		tmpSelectionPopulation.push_back(parentGene2);
		tmpSelectionPopulation.push_back(childGene1);
		tmpSelectionPopulation.push_back(childGene2);

		this->_highRankGeneSelection(classifiedByRankGene, tmpSelectionPopulation, highRankPopulation, 1);
		newSearchPopulation.push_back(highRankPopulation[1]);
	}
}

/*
	�̂̃����N��Ԃ�
	@param &classifiedByRankGene �����N���ƂɃN���X�������ꂽ��
	@param &targetGene �����N��m�肽����
*/
int GA::_returnGeneRank(
	const std::vector<std::vector<std::vector<int> > > &classifiedByRankGene,
	const std::vector<int> &targetGene)
{
	int rank, numGene;

	for (rank = 0; rank < classifiedByRankGene.size(); ++rank)
	{
		for (numGene = 0; numGene < classifiedByRankGene[rank].size(); ++numGene)
		{
			if (classifiedByRankGene[rank][numGene] == targetGene)
				return rank;
		}
	}
}

/*
	�̌Q���烉���_����2�̂�I������
	@param &targetPopulation �I������̌Q
	@param &gene1 �I�����ꂽ��1
	@param &gene2 �I�����ꂽ��2
*/
void GA::_select2GenesFromPopulation(
	const std::vector<std::vector<int> > &targetPopulation,
	std::vector<int> &gene1,
	std::vector<int> &gene2)
{
	int geneNum1, geneNum2;

	// �̂�I�����邽�߂̃����_���l�𐶐�
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_int_distribution<int> randomValue(0, targetPopulation.size()-1);

	// �����_���Ɍ̂�I��
	do
	{
		geneNum1	= randomValue(mt);
		geneNum2	= randomValue(mt);
	} while (geneNum1 == geneNum2);

	gene1	= targetPopulation[geneNum1];
	gene2	= targetPopulation[geneNum2];
}

/*
	��l�����������Ȃ�
	@param &parentGene1 �e��1
	@param &parentGene2 �e��2
	@param &childGene1 �q��1
	@param &childGene2 �q��2
*/
void GA::_uniformCrossover(
	const std::vector<int> &parentGene1,
	const std::vector<int> &parentGene2,
	std::vector<int> &childGene1,
	std::vector<int> &childGene2)
{
	// �J�E���g�ϐ�
	int tmp;

	// �}�X�N�p�^�[���p�̃����_���l������
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_int_distribution<int> randomValue(0, 1);

	// �}�X�N�p�^�[���𐶐�
	std::vector<int> maskPattern(this->_geneLength*this->_numVariable);
	for (tmp = 0; tmp < this->_geneLength; tmp++)
		maskPattern.push_back(randomValue(mt));

	// ����
	for (tmp = 0; tmp < this->_geneLength*this->_numVariable; tmp++)
	{
		if (maskPattern[tmp] == 0)
		{
			childGene1.push_back(parentGene1[tmp]);
			childGene2.push_back(parentGene2[tmp]);
		}
		else if (maskPattern[tmp] == 1)
		{
			childGene1.push_back(parentGene2[tmp]);
			childGene2.push_back(parentGene1[tmp]);
		}
	}
}

/*
	�w�肵���̂����ʃ����N�̂�I��
	@param &classifiedByRankGene �����N���ƂɃN���X�������ꂽ��
	@param &targetPopulation �I������̌Q
	@param &highRankPopulation ��ʃ����N�̌̌Q
	@param num �I������̐�
*/
void GA::_highRankGeneSelection(
	const std::vector<std::vector<std::vector<int> > > &classifiedByRankGene,
	const std::vector<std::vector<int> > &targetPopulation,
	std::vector<std::vector<int> > &highRankPopulation,
	int num)
{
	int geneRank;
	double geneDistance, longestDistance = 0.;
	std::vector<int> tmpHighRankGene(this->_geneLength*this->_numVariable);
	std::vector<std::vector<std::vector<int> > > objectiveSortedGene(2);	// �ړI�֐��̐�	

	// �Ώۂ̌̌Q�������N���Ƃɕ�����
	std::vector<std::vector<std::vector<int> > > tmpClassifiedByRankGene;
	this->_nonSuperioritySort(targetPopulation, tmpClassifiedByRankGene);

	// ��ʃ����N�̌̂�num�I������
	for (int rank = 0; highRankPopulation.size() < num; ++rank)
	{
		if (tmpClassifiedByRankGene[rank].size() == 1)
			highRankPopulation.push_back(tmpClassifiedByRankGene[rank][0]);
		else if (tmpClassifiedByRankGene.size() > 1)
		{
			// �������N�ɕ����̂����݂����ꍇ
			// �ł����G�����������̂�I�����Ēǉ�����
			for (int tmp = 0; tmp < tmpClassifiedByRankGene[rank].size(); ++tmp)
			{
				// �S�̒��̃����N���獬�G�x���������߂�
				geneRank	= this->_returnGeneRank(classifiedByRankGene, tmpClassifiedByRankGene[rank][tmp]);
				this->_putObjectiveSortedGeneEveryObjectiveFunc(classifiedByRankGene[geneRank], objectiveSortedGene);
				geneDistance	= this->_culcCrowdingDistanseForIndividual(objectiveSortedGene, tmpClassifiedByRankGene[rank][tmp]);
				if (longestDistance < geneDistance)
				{
					longestDistance = geneDistance;
					tmpHighRankGene	= tmpClassifiedByRankGene[rank][tmp];
				}
			}

			highRankPopulation.push_back(tmpHighRankGene);
		}
	}
}

/*
	5�Ԗڂ̌̂ɓˑR�ψق��s���D
	�K���x�v�Z�O�̌̂̈�`�q(0 or 1)�������_���ɓ���ւ���
	@param &targetPopulation �Ώۂ̌̌Q
	@param mutationRate �ˑR�ψٗ�
*/
void GA::_mutationGene(
	std::vector<std::vector<int> > &targetPopulation,
	double mutationRate)
{
	int geneNum = 5;

	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_real_distribution<double> randomValue(0.0, 1.0);

	for (int tmp = 0; tmp < this->_geneLength*this->_numVariable; tmp++)
	{
		if (mutationRate > randomValue(mt))
		{
			switch (targetPopulation[geneNum][tmp])
			{
			case 0:
				targetPopulation[geneNum][tmp] = 1;
			case 1:
				targetPopulation[geneNum][tmp] = 0;
			default:
				break;
			}
		}
	}
}

/*
	�w�肵������̌̏W�c��x�ƓK���x��\������
	@param &targetPopulation �Ώۂ̌̌S
	@param generation �����_�ł̐���
*/
void GA::_outputObjectiveValue(
	std::vector<std::vector<int> > targetPopulation,
	int generation)
{
	double x;
	std::vector<std::vector<double> > objectiveParameter(targetPopulation.size(), std::vector<double>(2));
	std::vector<double> objectiveValue(2);

	std::cout << generation << "-generation" << std::endl;
	for (int numGene = 0; numGene < targetPopulation.size(); ++numGene)
	{
		this->_binary2ObjectiveFunc(targetPopulation[numGene], objectiveParameter[numGene]);
		this->_getObjectiveFunc(objectiveParameter[numGene], objectiveValue);

		for (int numVar = 0; numVar < this->_numVariable; ++numVar)
		{
			std::cout << objectiveParameter[numGene][numVar] << ",";
		}
		std::cout << "\t";
		for (int numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
		{
			std::cout << objectiveValue[numObj] << ",";
		}
	}
	std::cout << std::endl;
}