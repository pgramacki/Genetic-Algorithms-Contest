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
	CPGAlg();
	~CPGAlg();

	bool bInitialize(CString  sTest);
	void vRunIteration();
	CString  sGetCurrentBestTree();

private:
	CTree *pc_best;
	int i_population_size;
	int i_iterations;
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
	void v_cut();

	size_t i_find_best();
	void v_clear();
};//class  CPGAlg