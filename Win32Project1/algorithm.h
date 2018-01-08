#pragma once
#include "tree.h"
#include "file.h"
#include "data.h"
#include "constants.h"

using namespace std;

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm();
	CGeneticAlgorithm(CString sSourceFile, string sResultFile, int iPopulationSize, int iIterations, 
		int iCrossingChance, int iMutationChance, int iNumOfVars = DEFAULT_NUM_OF_VARIABLES);
	~CGeneticAlgorithm();

	void vFind();
	void vTest();

private:
	CString s_source_file;
	string s_destination_file;
	int i_population_size;
	int i_iterations;
	int i_crossing_chance;
	int i_mutation_chance;
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
};

