#pragma once
#include "constants.h"
#include "data.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <atlstr.h>

using namespace std;

class CFileReader
{
public:
	CFileReader();
	~CFileReader();

	vector<CRecord *> vReadData(CString sFileName = DEF_SOURCE_FILE, int iNumOfVariables = DEFAULT_NUM_OF_VARIABLES);
	void vSaveToFile(string sText, string sFileName = DEF_RESULT_FILE);

private:
	//string s_file_name;
	//int i_num_of_variables;
	FILE *pf_file;
};

