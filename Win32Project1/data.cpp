#include "data.h"


CRecord::CRecord(vector<double> vArgs, double dResult, int iNumOfArgs /*= DEFAULT_NUM_OF_VARIABLES*/)
{
	i_num_of_arguments = iNumOfArgs;
	v_arguments = vArgs;
	d_result = dResult;
}

CRecord::~CRecord()
{
}
