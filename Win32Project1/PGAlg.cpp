#include  "PGAlg.h"




CPGAlg::CPGAlg()
{
	s_source_file = DEF_SOURCE_FILE;
	s_destination_file = DEF_RESULT_FILE;
	i_population_size = DEF_POPULATION_SIZE;
	d_crossing_chance = DEF_CROSSING_CHANCE;
	d_mutation_chance = DEF_MUTATION_CHANCE;
	i_number_of_variables = DEFAULT_NUM_OF_VARIABLES;

	pv_population = new vector<CTree *>(i_population_size);
}

bool CPGAlg::bInitialize(CString  sTest)
{
	srand(time(0));
	i_iterations = 0;

	v_data = c_file_manager.vReadData(sTest);

	v_initialization();
	v_evaluation();

	return(true);
}

void CPGAlg::vRunIteration()
{
	i_iterations++;

	if (i_iterations % CUT_FREQUENCY == 0)
		v_cut();

	cout << "iteracja... " << endl;

	v_selection();
	v_crossing();
	v_mutation();
	v_evaluation();
}

CString  CPGAlg::sGetCurrentBestTree()
{
	size_t st_best = i_find_best();
	string s_result = (*pv_population)[st_best]->sPrintTree();
	cout << s_result << endl << (*pv_population)[st_best]->dGetAccuracy() << endl;
	return (s_result.c_str());
}

void CPGAlg::v_initialization()
{
	//cout << "init" << endl;

	CTree *pc_tmp;

	for (int ii = 0; ii < i_population_size; ii++)
	{
		pc_tmp = new CTree(i_number_of_variables);
		pc_tmp->vCreateRandom();
		(*pv_population)[ii] = pc_tmp;
	}
}
void CPGAlg::v_evaluation()
{
	//cout << "eval" << endl;

	size_t st_current_best = 0;

	for (size_t ii = 0; ii < pv_population->size(); ii++)
	{
		if ((*pv_population)[ii]->iCalculateAccuracy(v_data) != NO_ERROR)
		{
			delete (*pv_population)[ii];
			(*pv_population)[ii] = new CTree(i_number_of_variables);
			(*pv_population)[ii]->vCreateRandom();
			ii--;
		}

		if ((*pv_population)[ii]->dGetAccuracy() < (*pv_population)[st_current_best]->dGetAccuracy())
			st_current_best = ii;
	}

	d_current_best_accuracy = (*pv_population)[st_current_best]->dGetAccuracy();
}

void CPGAlg::v_selection()
{
	//cout << "select" << endl;

	vector<CTree *> *pv_parents = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;

	(*pv_parents)[0] = new CTree(*(*pv_population)[i_find_best()]);

	for (int ii = 1; ii < i_population_size; ii++)
	{
		i_first = rand() % i_population_size;	// STH WRONG HERE!!!
		//cout << "i_first = " << i_first << endl;
		do
		{
			i_second = rand() % i_population_size;
			//cout << "i_second = " << i_second << endl;
		} while (i_first == i_second);		// i_second changes

		if ((*pv_population)[i_first]->dGetAccuracy() < (*pv_population)[i_second]->dGetAccuracy())
			(*pv_parents)[ii] = new CTree(*(*pv_population)[i_first]);
		else (*pv_parents)[ii] = new CTree(*(*pv_population)[i_second]);
	}

	for (size_t ii = 0; ii < pv_population->size(); ii++)
		delete (*pv_population)[ii];
	delete pv_population;

	pv_population = pv_parents;
}

void CPGAlg::v_crossing()
{
	//cout << "cross" << endl;

	vector<CTree *> *pv_children = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;
	CTree *pc_first;
	CTree *pc_second;

	for (int ii = 0; ii < i_population_size; ii += 2)
	{
		i_first = rand() % i_population_size;
		//cout << "i_first = " << i_first << endl;
		do
		{
			i_second = rand() % i_population_size;
			//cout << "i_second = " << i_second << endl;
		} while (i_first == i_second);

		pc_first = new CTree(*(*pv_population)[i_first]);
		pc_second = new CTree(*(*pv_population)[i_second]);

		if ((rand() % 100) < d_crossing_chance)
			pc_first->vCrossWith(pc_second);

		(*pv_children)[ii] = pc_first;
		(*pv_children)[ii + 1] = pc_second;
	}

	for (size_t ii = 0; ii < pv_population->size(); ii++)
		delete (*pv_population)[ii];
	delete pv_population;

	pv_population = pv_children;
}

void CPGAlg::v_mutation()
{
	for (size_t ii = 0; ii < pv_population->size(); ii++)
		if (rand() % 100 < d_mutation_chance)
			(*pv_population)[ii]->vMutate();
}

void CPGAlg::v_cut()
{
	for (size_t ii = 0; ii < pv_population->size(); ii++)
		if ((*pv_population)[ii]->dGetAccuracy() / d_current_best_accuracy > CUT_CONDITION)
			(*pv_population)[ii]->vCutTree(CUT_DEPTH);

	v_evaluation();
}

size_t CPGAlg::i_find_best()
{
	size_t i_best_tree = 0;

	for (size_t ii = 1; ii < pv_population->size(); ii++)
		if ((*pv_population)[ii]->dGetAccuracy() < (*pv_population)[i_best_tree]->dGetAccuracy())
			i_best_tree = ii;

	return i_best_tree;
}
