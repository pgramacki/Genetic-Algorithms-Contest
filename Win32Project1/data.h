#pragma once
#include "constants.h"
#include <vector>

using namespace std;

class CRecord
{
public:
	CRecord(vector<double> vArgs, double dResult, int iNumOfArgs = DEFAULT_NUM_OF_VARIABLES);
	~CRecord();

	vector<double> vGetArguments() { return v_arguments; }
	double dGetResult() { return d_result; }

private:
	int i_num_of_arguments;
	vector<double> v_arguments;
	double d_result;
};

