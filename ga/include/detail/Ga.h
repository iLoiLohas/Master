#include "../GaCommon.h"
#include <vector>
#include <iostream>

/*
	�v�f�Ŏw�肵��2�����z��̗v�f���폜
	�폜���ꂽ�v�f�̃C���f�b�N�X�͋l�߂���
	�����v�f�͂��ׂč폜�����
*/
template<typename T>
void GaCommonTemp<T>::removeElement(std::vector<std::vector<T> > &population, const std::vector<T> &element)
{
	for (int tmp = 0; tmp < population.size(); ++tmp)
		if (population[tmp] == element) population.erase(population.begin() + tmp);
}

/*
	2�����z��̂��ׂĂ̗v�f��\������
	@param &individual �\������2�����z��
*/
template<typename T>
void GaCommonTemp<T>::outputAllElement(const std::vector<T> &individual)
{
	for (int tmp = 0; tmp < individual.size(); ++tmp)
	{
		std::cout << individual[tmp];
		if (tmp != individual.size()-1)
			std::cout << " ";
	}
	std::cout << std::endl;
}