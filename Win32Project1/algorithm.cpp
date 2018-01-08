#include "algorithm.h"



CGeneticAlgorithm::CGeneticAlgorithm()
{
	s_source_file = DEF_SOURCE_FILE;
	s_destination_file = DEF_RESULT_FILE;
	i_population_size = DEF_POPULATION_SIZE;
	i_iterations = DEF_ITERATIONS_NUM;
	i_crossing_chance = DEF_CROSSING_CHANCE;
	i_mutation_chance = DEF_MUTATION_CHANCE;
	i_number_of_variables = DEFAULT_NUM_OF_VARIABLES;

	if (i_population_size % 2 != 0) i_population_size++;

	pv_population = new vector<CTree *>(i_population_size);
}

CGeneticAlgorithm::CGeneticAlgorithm(CString sSourceFile, string sResultFile, int iPopulationSize, int iIterations, int iCrossingChance, 
	int iMutationChance, int iNumOfVars /*= DEFAULT_NUM_OF_VARIABLES*/)
{
	s_source_file = sSourceFile;
	s_destination_file = sResultFile;
	i_population_size = iPopulationSize;
	i_iterations = iIterations;
	i_crossing_chance = iCrossingChance;
	i_mutation_chance = iMutationChance;
	i_number_of_variables = iNumOfVars;

	if (i_population_size % 2 != 0) i_population_size++;

	pv_population = new vector<CTree *>(i_population_size);
}


CGeneticAlgorithm::~CGeneticAlgorithm()
{
	if (pv_population != NULL)
	{
		for (size_t ii = 0; ii < pv_population->size(); ii++)
			delete (*pv_population)[ii];

		delete pv_population;
	}

	for (size_t ii = 0; ii < v_data.size(); ii++)
		delete v_data[ii];
}

void CGeneticAlgorithm::vFind()
{
	v_data = c_file_manager.vReadData(s_source_file, i_number_of_variables);

	v_initialization();
	v_evaluation();

	while (i_iterations-- > 0)
	{
		cout << i_iterations << endl;

		v_selection();
		v_crossing();
		v_mutation();
		v_evaluation();
	}

	size_t i_best_tree = i_find_best();

	c_file_manager.vSaveToFile((*pv_population)[i_best_tree]->sPrintTree() + NEW_LINE 
		+ to_string((*pv_population)[i_best_tree]->dGetAccuracy()), s_destination_file);
}

// test method
//void CGeneticAlgorithm::vTest()
//{
//	CTree a;
//	cout << a.iCreateFromString("+ sin x0 x1") << endl;
//
//	v_data = c_file_manager.vReadData();
//
//	cout << a.iCalculateAccuracy(v_data) << endl;
//
//	cout << a.dGetAccuracy();
//}

void CGeneticAlgorithm::v_initialization()
{
	CTree *pc_tmp;

	for (int ii = 0; ii < i_population_size; ii++)
	{
		pc_tmp = new CTree(i_number_of_variables);
		pc_tmp->vCreateRandom();
		(*pv_population)[ii] = pc_tmp;
	}
}

void CGeneticAlgorithm::v_evaluation()
{
	for (size_t ii = 0; ii < pv_population->size(); ii++)
	{
		if ((*pv_population)[ii]->iCalculateAccuracy(v_data) != NO_ERROR)
		{
			//cout << (*pv_population)[ii]->sPrintTree() << endl;
			delete (*pv_population)[ii];
			(*pv_population)[ii] = new CTree(i_number_of_variables);
			(*pv_population)[ii]->vCreateRandom();
			ii--;
		}
	}
}

void CGeneticAlgorithm::v_selection()
{
	vector<CTree *> *pv_parents = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;

	(*pv_parents)[0] = new CTree(*(*pv_population)[i_find_best()]);

	for (int ii = 1; ii < i_population_size; ii++)
	{
		i_first = rand() % i_population_size;
		do
			i_second = rand() % i_population_size;
		while (i_first == i_second);

		if ((*pv_population)[i_first]->dGetAccuracy() < (*pv_population)[i_second]->dGetAccuracy())
			(*pv_parents)[ii] = new CTree(*(*pv_population)[i_first]);
		else (*pv_parents)[ii] = new CTree(*(*pv_population)[i_second]);
	}

	for (size_t ii = 0; ii < pv_population->size(); ii++)
		delete (*pv_population)[ii];
	delete pv_population;

	pv_population = pv_parents;
}

void CGeneticAlgorithm::v_crossing()
{
	vector<CTree *> *pv_children = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;
	CTree *pc_first;
	CTree *pc_second;

	//(*pv_children)[0] = new CTree(*(*pv_population)[i_find_best()]);
	//(*pv_children)[1] = new CTree(*(*pv_population)[i_find_best()]);

	for (int ii = 0; ii < i_population_size; ii += 2)
	{
		i_first = rand() % i_population_size;
		do
			i_second = rand() % i_population_size;
		while (i_first == i_second);

		pc_first = new CTree(*(*pv_population)[i_first]);
		pc_second = new CTree(*(*pv_population)[i_second]);

		if (rand() % 100 < i_crossing_chance)
			pc_first->vCrossWith(pc_second);

		(*pv_children)[ii] = pc_first;
		(*pv_children)[ii + 1] = pc_second;
	}

	for (size_t ii = 0; ii < pv_population->size(); ii++)
		delete (*pv_population)[ii];
	delete pv_population;

	pv_population = pv_children;
}

void CGeneticAlgorithm::v_mutation()
{
	for (size_t ii = 0; ii < pv_population->size(); ii++)
		if (rand() % 100 < i_mutation_chance)
			(*pv_population)[ii]->vMutate();
}

size_t CGeneticAlgorithm::i_find_best()
{
	size_t i_best_tree = 0;

	for (size_t ii = 1; ii < pv_population->size(); ii++)
	{
		//cout << (*pv_population)[ii]->sPrintTree() << " " << (*pv_population)[ii]->dGetAccuracy() << endl;

		if ((*pv_population)[ii]->dGetAccuracy() < (*pv_population)[i_best_tree]->dGetAccuracy())
			i_best_tree = ii;
	}
	return i_best_tree;
}
