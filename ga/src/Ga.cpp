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
	int population		= 50;	// �̐�

	this->_population	= population;
	this->_geneLength	= geneLength;
	this->_numVariable	= numVariable;
}

GA::~GA()
{
}

/*
	�T����W�c������������D
	@param &searchPopulation
*/
void GA::_initSearchPopulation(std::vector<std::vector<int> > &searchPopulation)
{
	while (searchPopulation.size() < this->_population)
	{
		std::vector<int> tmpGene(this->_geneLength*this->_numVariable);
		this->_createRandomlyIndividual(tmpGene);

		searchPopulation.reserve(this->_population);
		if (!this->_isDuplicatedGene(searchPopulation, tmpGene))
			searchPopulation.push_back(tmpGene);
	}
}

/*
	�̂������_����1��������
	@param &individual ���������
*/
void GA::_createRandomlyIndividual(std::vector<int> &individual)
{
	// [0.0, 1.0]�̃����_���l�𐶐�
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> randomValue(0.0, 1.0);

	for (int numBinary = 0; numBinary < this->_geneLength*this->_numVariable; ++numBinary)
		individual[numBinary]	= (randomValue(mt) > 0.5) ? 1 : 0;
}

/*
	�̂�\������
	@param &individual �\�������
*/
void GA::_outputGene(const std::vector<int> &individual)
{
	int numBinary;

	for (numBinary = 0; numBinary < this->_geneLength*this->_numVariable; ++numBinary)
		std::cout << individual[numBinary];

	std::cout << std::endl;
}

/*
	�̏W�c��\������
	@param &targetPopulation �\������̏W�c
*/
void GA::_outputPopulation(const std::vector<std::vector<int>> &targetPopulation)
{
	int numIndividual;

	for (numIndividual = 0; numIndividual < targetPopulation.size(); ++numIndividual)
		this->_outputGene(targetPopulation[numIndividual]);
}

/*
	�̂��̏W�c�̒��ŏd�����Ă��Ȃ������肷��
	@param &searchPopulation �̏W�c
	@param &targetGene ��
*/
bool GA::_isDuplicatedGene(std::vector<std::vector<int>> &searchPopulation, const std::vector<int> &targetGene)
{
	// �̏W�c�̗v�f��0��������d�����Ă��Ȃ�
	if (searchPopulation.size() == 0)
		return false;

	int numGene;
	for (numGene = 0; numGene < searchPopulation.size(); ++numGene)
		if (searchPopulation[numGene] == targetGene)
			return true;

	return false;
}

/*
	1��2�i���f�[�^��\���^�ɕϊ�
	@param &binary 1�̂̈�`�q
	@param &phenotype ��`�q�^��\���^�ɕϊ��������́i�̈���m�ۂ��Ă����j
*/
void GA::_convertPhenotype(const std::vector<int> &binary, std::vector<double> &phenotype)
{

	for (int numVar = 0; numVar < this->_numVariable; ++numVar)
	{
		std::vector<int> tmpGene(this->_geneLength);

		for (int numBinary = 0; numBinary < this->_geneLength; ++numBinary)
			tmpGene[numBinary]	= binary[numVar*this->_geneLength + numBinary];

		phenotype[numVar]	= this->_binary2Phenotype(tmpGene);
	}
}

/*
	1��`�q��2�i���f�[�^��\���^�ɕϊ�
	@param &binary 1��`�q���̒���������2�i���f�[�^
*/
double GA::_binary2Phenotype(const std::vector<int> &binary)
{
	int place		= 0;
	double decimal	= 0.;

	for (int numBinary = this->_geneLength - 1; numBinary >= 0 ; --numBinary)
	{
		if (binary[numBinary] == 1)
			decimal	+= pow(2.0, place);
		place	+= 1;
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
	objectiveValue[0]	= this->_f1(variable);
	objectiveValue[1]	= this->_f2(variable);
}

double GA::_f1(const std::vector<double> &variable)
{
	return 1. - exp(-4.*variable[0])*pow(sin(6.*M_PI*variable[0]),6);
	// return -(sin(3.*variable[0])+0.5*sin(9.*variable[0])+sin(15.*variable[0]+50.));
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
//	GaCommonTemp<double>::outputAllElement(obj);
}

/*
	NSGA2���s�p���\�b�h
*/
void GA::nsga2Run()
{
	int generation = 0;
	int maxGeneration	= 500;
	std::vector<std::vector<std::vector<int> > > archivePopulation(maxGeneration), searchPopulation(maxGeneration);

	/*** Step1 ***/
	std::cout << "Call Step1!" << std::endl;
	this->_initSearchPopulation(searchPopulation[generation]);

	while(1)
	{
		/*** Step2 ***/
		std::cout << "Call Step2!" << std::endl;
		// TODO:�]�����@�̊m��
//		this->_outputObjectiveValue(searchPopulation[generation], generation);

		/*** Step3 ***/
		std::cout << "Call Step3!" << std::endl;
		std::vector<std::vector<std::vector<int> > > classifiedByRankGene;
		std::vector<std::vector<int> > margedPopulation;
		GaCommon::joinPopulation(archivePopulation[generation], searchPopulation[generation], margedPopulation);
//		std::cout << margedPopulation.size() << std::endl;
		this->_nonSuperioritySort(margedPopulation, classifiedByRankGene);

//		std::cout << classifiedByRankGene[0].size() << std::endl;

		/*** Step4 ***/
		std::cout << "Call Step4!" << std::endl;
		std::vector<std::vector<int> > nextRankPopulation;
		this->_updateArchivePopulation(classifiedByRankGene, archivePopulation[generation+1], nextRankPopulation);
//		std::cout << archivePopulation[generation+1].size() << std::endl;

		/*** Step5 ***/
		std::cout << "Call Step5!" << std::endl;
		std::vector<std::vector<int> > crowdingSortedPopulation;
		this->_crowdingSort(nextRankPopulation, crowdingSortedPopulation);
		this->_insertIndividuals(archivePopulation[generation+1], nextRankPopulation);
//		std::cout << archivePopulation[generation+1].size() << std::endl;

		/*** Step6 ***/
		std::cout << "Call Step6!" << std::endl;
		this->_outputObjectiveValue(searchPopulation[generation], generation);
		if  (generation == maxGeneration)
			break;

		/*** Step7 ***/
		std::cout << "Call Step7!" << std::endl;
		// �I���ƌ����𓯎��ɍs���Ă���
		this->_crowdedTournamentSelection(archivePopulation[generation+1], searchPopulation[generation+1], classifiedByRankGene);
//		std::cout << searchPopulation[generation+1].size() << std::endl;

		/*** Step8 ***/
		std::cout << "Call Step8!" << std::endl << std::endl;
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
	int tmp, numGene;
	std::vector<std::vector<int> > sortingPopulation;

	// �̂Ƀ����N��t���Ă����C�����N�t�����ꂽ�̂͏���
	std::copy(targetPopulation.begin(), targetPopulation.end(), std::back_inserter(sortingPopulation));
	while (sortingPopulation.size() > 0)
	{
		std::vector<std::vector<int> > tmpRankedGene;

		for (numGene = 0; numGene < sortingPopulation.size(); ++numGene)
		{
//			std::cout << this->_isSuperior(sortingPopulation[numGene], sortingPopulation) << std::endl;
			if (this->_isSuperior(sortingPopulation[numGene], sortingPopulation))
				tmpRankedGene.push_back(sortingPopulation[numGene]);			
		}

		// �����N���ƂɌ̂�ۑ����C�̌Q���X�V
		classifiedByRankGene.push_back(tmpRankedGene);
		for (tmp = 0; tmp < tmpRankedGene.size(); ++tmp)
			GaCommonTemp<int>::removeElement(sortingPopulation, tmpRankedGene[tmp]);
	}
}

/*
	�̂��̏W�c�̒��ŗD�z���Ă��邩�𔻒肷��
	@param &targetGene ���肷���
	@param &comparedPopulation targetGene��������̏W�c
*/
bool GA::_isSuperior(
	const std::vector<int> &targetGene,
	const std::vector<std::vector<int> > &comparedPopulation)
{
	int numGene, numSuperior;
	bool isSuperior;
	std::vector<double> targetObjeciveValue(2), comparedObjectiveValue(2);	// �ړI�֐��̐�

	isSuperior	= true;
	this->_binary2ObjectiveFunc(targetGene, targetObjeciveValue);
//	GaCommonTemp<double>::outputAllElement(targetObjeciveValue);

	for (numGene = 0; numGene < comparedPopulation.size(); ++numGene)
	{
		if (comparedPopulation[numGene] == targetGene)
			continue;

		this->_binary2ObjectiveFunc(comparedPopulation[numGene], comparedObjectiveValue);

		// �ړI�֐��l���r����1���������Ȃ��������D�z�ő̂łȂ�
		numSuperior	= this->_numOfSuperior(targetObjeciveValue, comparedObjectiveValue);
		if (numSuperior == 0)
			isSuperior	= false;
	}

	return isSuperior;
}

/*
	�ړI�֐��̂����C�D�z���Ă������Ԃ�
	@param &targetObjectiveValue ��ׂ�ő̖̂ړI�֐��l
	@param &comparedObjectiveValue ��ׂ���ő̖̂ړI�֐��l
*/
int GA::_numOfSuperior(
	const std::vector<double> &targetObjeciveValue,
	const std::vector<double> &comparedObjectiveValue)
{
	int num = 0;

	for (int numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
		if (targetObjeciveValue[numObj] < comparedObjectiveValue[numObj])
			num	+= 1;

	return num;
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
//		std::cout << classifiedByRankGene[rank].size() << std::endl;
		if (this->_population - newArchivePopulation.size() > classifiedByRankGene[rank].size())
			GaCommon::pushBackAll(newArchivePopulation, classifiedByRankGene[rank]);
		else
			break;
	}

	// �A�[�J�C�u��W�c�ɓ��肫��Ȃ������ō������N�̌̏W�c���i�[
	nextRankPopulation.reserve(classifiedByRankGene[rank].size());
	nextRankPopulation	= classifiedByRankGene[rank];
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
	std::vector<std::vector<std::vector<int> > > objectiveSortedGene(2);	// �ړI�֐��̐�

	// �ړI�֐����ƂɖړI�֐��l���������ɕ��ׂ�
	this->_putObjectiveSortedGeneEveryObjectiveFunc(certainRankPopulation, objectiveSortedGene);

	// ���G�x���傫�����Ɍ̂��\�[�g
	crowdingSortedPopulation.reserve(certainRankPopulation.size());
	crowdingSortedPopulation	= certainRankPopulation;
	this->_orderByCrowdingDistanceUsingBubbleSort(crowdingSortedPopulation, objectiveSortedGene);
}

/*
	���G�x���傫�����Ɍ̌Q���o�u���\�[�g
	@param &sortingPopulation �\�[�g�Ώۂ̌̌Q
	@param &objectiveSortedGene �ړI�֐����ƂɖړI�֐��l���������ɕ��񂾌̌Q
*/
void GA::_orderByCrowdingDistanceUsingBubbleSort(
	std::vector<std::vector<int> > &sortingPopulation,
	const std::vector<std::vector<std::vector<int> > > &objectiveSortedGene)
{
	double distance1, distance2;
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);

	for (int tmp1 = 1; tmp1 < sortingPopulation.size(); ++tmp1)
	{
		for (int tmp2 = sortingPopulation.size()-1; tmp2 > tmp1 ; --tmp2)
		{
			distance1	= this->_culcCrowdingDistanseForIndividual(objectiveSortedGene, sortingPopulation[tmp2]);
			distance2	= this->_culcCrowdingDistanseForIndividual(objectiveSortedGene, sortingPopulation[tmp2-1]);
			if (distance1 > distance2)
			{
				tmpGene	= sortingPopulation[tmp2];
				sortingPopulation[tmp2]	= sortingPopulation[tmp2-1];
				sortingPopulation[tmp2-1]	= tmpGene;
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
	for (int numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
	{
		this->_orderBySmallObjectiveValueUsingBubbleSort(targetPopulation, objectiveSortedGene[numObj], numObj);
		std::reverse(objectiveSortedGene[numObj].begin(), objectiveSortedGene[numObj].end());
	}
}

/*
	�w�肵���ړI�֐��l�����������Ɍ̂��o�u���\�[�g����
	@param &targetGene �\�[�g�������̌Q
	@param &objectiveSortedGene �\�[�g��̌̌Q
	@param num �ΏۂƂ���ړI�֐��̔ԍ�
*/
void GA::_orderBySmallObjectiveValueUsingBubbleSort(
	const std::vector<std::vector<int> > &targetPopulation,
	std::vector<std::vector<int> > &objectiveSortedPopulation,
	int num)
{
	std::vector<double> tmpObject1(2), tmpObject2(2);	// �ړI�֐��̐�
	std::vector<int> tmpGene(this->_geneLength*this->_numVariable);

	// �\�[�g����̌Q���R�s�[
	objectiveSortedPopulation.reserve(targetPopulation.size());
	objectiveSortedPopulation	= targetPopulation;

	for (int tmp1 = 0; tmp1 < targetPopulation.size(); ++tmp1)
	{
		for (int tmp2 = targetPopulation.size() - 1; tmp2 > tmp1; --tmp2)
		{
			this->_binary2ObjectiveFunc(targetPopulation[tmp2], tmpObject1);
			this->_binary2ObjectiveFunc(targetPopulation[tmp2-1], tmpObject2);
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
	std::vector<double> tmpObjRight(2);		// �ړI�֐��̐�
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
		resultDistanse	+= distance[numObj];

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

	insertedPopulation.reserve(this->_population);
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
	std::vector<int> parentGene1(this->_geneLength*this->_numVariable), parentGene2(this->_geneLength*this->_numVariable);
	std::vector<int> childGene1(this->_geneLength*this->_numVariable), childGene2(this->_geneLength*this->_numVariable);

	// �̐���N�ɂȂ�܂őI�������s
	// �e�̂������_���ɑI�����C��l���������s
	// �e�~2�Ǝq�~2�̂��������N����ʂ̌̂�1�I�����C�V���ȒT����W�c�ɒǉ�����
	newSearchPopulation.reserve(this->_population);
	for (int numGene = 0; newSearchPopulation.size() < this->_population; ++numGene)
	{
		std::vector<std::vector<int> > tmpSelectionPopulation(4), highRankPopulation;

		this->_select2GenesFromPopulation(selectedPopulation, parentGene1, parentGene2);
		this->_uniformCrossover(parentGene1, parentGene2, childGene1, childGene2);

		tmpSelectionPopulation[0]	= parentGene1;
		tmpSelectionPopulation[1]	= parentGene2;
		tmpSelectionPopulation[2]	= childGene1;
		tmpSelectionPopulation[3]	= childGene2;

		this->_highRankGeneSelection(classifiedByRankGene, tmpSelectionPopulation, highRankPopulation, 1);
		newSearchPopulation.push_back(highRankPopulation[0]);
//		std::cout << newSearchPopulation.size() << std::endl;
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
	@param &childGene1 �q��1�i�̈�m�ۍς݁j
	@param &childGene2 �q��2�i�̈�m�ۍς݁j
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
	for (tmp = 0; tmp < this->_geneLength*this->_numVariable; tmp++)
		maskPattern[tmp]	= randomValue(mt);

	// ����
	for (tmp = 0; tmp < this->_geneLength*this->_numVariable; tmp++)
	{
		if (maskPattern[tmp] == 0)
		{
			childGene1[tmp]	= parentGene1[tmp];
			childGene2[tmp]	= parentGene2[tmp];
		}
		else if (maskPattern[tmp] == 1)
		{
			childGene1[tmp]	= parentGene2[tmp];
			childGene2[tmp]	= parentGene1[tmp];
		}
	}
}

/*
	2�_�������s��
	@param &parentGene1 �e��1
	@param &parentGene2 �e��2
	@param &childGene1 �q��1�i�̈�m�ۍς݁j
	@param &childGene2 �q��2�i�̈�m�ۍς݁j
*/
void GA::_2pointCrossover(
	const std::vector<int> &parentGene1,
	const std::vector<int> &parentGene2,
	std::vector<int> &childGene1,
	std::vector<int> &childGene2)
{
	// �����_���l������
	std::random_device seedGen;
	std::mt19937 mt(seedGen());
	std::uniform_int_distribution<int> randomValue(0, this->_geneLength-1);

	int point1, point2;
	do
	{
		point1	= randomValue(mt);
		point2	= randomValue(mt);
	} while (point1 == point2);

	// �����_�����߂�
	int start, end;
	if (point1 < point2)
	{
		start	= point1;
		end	= point2;
	} else if (point1 > point2)
	{
		start	= point2;
		end	= point1;
	} else
	{
		std::cout << "ERROR:No crossover point!";
		return;
	}

	childGene1	= parentGene1;
	childGene2	= parentGene2;
	// this->_copyGene(parentGene1, childGene1);
	// this->_copyGene(parentGene2, childGene2);
	for (int tmp = start; tmp <= end; ++tmp)
	{
		childGene1[tmp]	= parentGene2[tmp];
		childGene2[tmp]	= parentGene1[tmp];
	}
}

/*
	��`�q���R�s�[����
	@param &targetGene �R�s�[�����`�q
	@param &copiedGene �R�s�[������`�q�i�̈�m�ۍς݁j
*/
void GA::_copyGene(
	const std::vector<int> &targetGene,
	std::vector<int> &copiedGene)
{
	for (int tmp = 0; tmp < targetGene.size(); ++tmp)
		copiedGene[tmp]	= targetGene[tmp];
}

/*
	�w�肵���̌Q�����ʃ����N�̂�I��
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

	// �Ώۂ̌̌Q�������N���Ƃɕ�����
	std::vector<std::vector<std::vector<int> > > tmpClassifiedByRankGene;
	this->_nonSuperioritySort(targetPopulation, tmpClassifiedByRankGene);

	// ��ʃ����N�̌̂�num�I������
	highRankPopulation.reserve(num);
	for (int rank = 0; highRankPopulation.size() < num; ++rank)
	{
		if (tmpClassifiedByRankGene[rank].size() == 1)
			highRankPopulation.push_back(tmpClassifiedByRankGene[rank].front());
		else if (tmpClassifiedByRankGene[rank].size() > 1)
		{
			// �������N�ɕ����̂����݂����ꍇ
			// �ł����G�����������̂�I�����Ēǉ�����
			// TODO:�����N�������ق����D�悳��邩�畡���������獬�G�����̏��ɂ��ׂĂ̌̂��i�[
			for (int tmp = 0; tmp < tmpClassifiedByRankGene[rank].size(); ++tmp)
			{
				// �S�̒��̃����N���獬�G�x���������߂�
				std::vector<std::vector<std::vector<int> > > objectiveSortedGene(2);	// �ړI�֐��̐�
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
	�w�肵������̌̏W�c�̖ړI�֐��l��\������
	@param &targetPopulation �Ώۂ̌̌S
	@param generation �����_�ł̐���
*/
void GA::_outputObjectiveValue(
	std::vector<std::vector<int> > targetPopulation,
	int generation)
{
	double x;
	std::vector<double> objectiveValue(2);

	double tmp;
	std::cout << generation << "-generation" << std::endl;
	for (int numGene = 0; numGene < targetPopulation.size(); ++numGene)
	{
		this->_binary2ObjectiveFunc(targetPopulation[numGene], objectiveValue);

		// tmp	= 1. - pow(objectiveValue[0],2);
		// std::cout << objectiveValue[0] << " " << tmp << std::endl;

		for (int numObj = 0; numObj < 2; ++numObj)	// �ړI�֐��̐�
		{
			std::cout << objectiveValue[numObj];
			if (numObj != 1)	// �ړI�֐��̐� - 1
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/*
	�w�肵������̌̏W�c��x��\������
	@param &targetPopulation �Ώۂ̌̌Q
	@param generation �����_�ł̐���
*/
// void GA::_outputParameterValue(
// 	const std::vector<std::vector<int> > &targetPopulation,
// 	int generation)
// {
// 	for (int numGene = 0; numGene < targetPopulation.size(); ++numGene)
// 	{
// 		this->_binary2ObjectiveFunc
// 	}
// }