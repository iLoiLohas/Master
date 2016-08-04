#ifndef __COMMON_H_INCLUDE__
#define __COMMON_H_INCLUDE__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <vector>

class Common
{
private:
public:
	Common();
	~Common();
	static void outputIntoFile(const std::string, const std::vector<double> &, const std::vector<double> &);
	static void resize2DemensionalVector(std::vector< std::vector< double > > &, unsigned int, unsigned int);
};

#endif // !__COMMON_H_INCLUDE_