#ifndef __GACOMMON_H_INCLUDE__
#define __GACOMMON_H_INCLUDE__

#include <vector>
#include <algorithm>

class GaCommon
{
public:
	static void joinPopulation(const std::vector<std::vector<int> >&, const std::vector<std::vector<int> >&, std::vector<std::vector<int> >&);
	static void pushBackAll(std::vector<std::vector<int> >&, const std::vector<std::vector<int> >&);
private:

};

template<typename T>
class GaCommonTemp
{
public:
	static void removeElement(std::vector<std::vector<T> >&, const std::vector<T>&);
private:

};


// テンプレート関数の実装を記述したファイルを読み込む
// link:http://qiita.com/MasayaMizuhara/items/37f8c2a5462a4f7f8ea0
#include "detail/Ga.h"

#endif // !__GACOMMON_H_INCLUDE__