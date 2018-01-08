#pragma once
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <atlstr.h>

#include "tree.h"
#include "constants.h"
#include "data.h"
#include "file.h"

using namespace std;


class  CPGAlg
{
public:
	bool bInitialize(CString  sTest);
	void vRunIteration();
	CString  sGetCurrentBestTree();

private:
	string s_source_file;
	string s_destination_file;
	int i_population_size;
	double d_crossing_chance;
	double d_mutation_chance;
	int i_number_of_variables;
	vector<CTree *> *pv_population;
	vector<CRecord *> v_data;
	CFileReader c_file_manager;

	void v_initialization();
	void v_evaluation();
	void v_selection();
	void v_crossing();
	void v_mutation();

	size_t i_find_best();
};//class  CPGAlg