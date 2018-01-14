#include  "PGAlg.h"




CPGAlg::CPGAlg()
{
	i_population_size = DEF_POPULATION_SIZE;
	d_crossing_chance = DEF_CROSSING_CHANCE;
	d_mutation_chance = DEF_MUTATION_CHANCE;
	i_number_of_variables = DEFAULT_NUM_OF_VARIABLES;

	pv_population = new vector<CTree *>(i_population_size);
	pc_best = NULL;
}

CPGAlg::~CPGAlg()
{
	if (pc_best != NULL)
		delete pc_best;

	if (pv_population != NULL)
	{
		for (size_t ii = 0; ii < pv_population->size(); ii++)
			delete pv_population->at(ii);
		delete pv_population;
	}

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

	cout << "iteracja... " << i_iterations << endl;

	v_selection();
	v_crossing();
	v_mutation();
	v_evaluation();
}

CString  CPGAlg::sGetCurrentBestTree()
{
	size_t st_best = i_find_best();
	CTree *pc_iter_best = (*pv_population)[st_best];

	if (pc_best == NULL)
		pc_best = new CTree(*pc_iter_best);
	else if (pc_best->dGetAccuracy() > pc_iter_best->dGetAccuracy())
	{
		delete pc_best;
		pc_best = new CTree(*pc_iter_best);
	}

	string s_result = pc_best->sPrintTree();
	cout << s_result << endl << pc_best->dGetAccuracy() << endl;
	return (s_result.c_str());
}

void CPGAlg::v_initialization()
{
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
	for (size_t ii = 0; ii < pv_population->size(); ii++)
	{
		if ((*pv_population)[ii]->iCalculateAccuracy(v_data) != NO_ERROR)
		{
			delete (*pv_population)[ii];
			(*pv_population)[ii] = new CTree(i_number_of_variables);
			(*pv_population)[ii]->vCreateRandom();
			ii--;
		}
	}
}

void CPGAlg::v_selection()
{

	vector<CTree *> *pv_parents = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;
	int ii = 0;

	if (pc_best != NULL)
	{
		(*pv_parents)[0] = new CTree(*pc_best);
		ii++;
	}

	for (; ii < i_population_size; ii++)
	{
		i_first = rand() % i_population_size;
		do
		{
			i_second = rand() % i_population_size;
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

	vector<CTree *> *pv_children = new vector<CTree *>(i_population_size);
	int i_first;
	int i_second;
	CTree *pc_first;
	CTree *pc_second;

	for (int ii = 0; ii < i_population_size; ii += 2)
	{
		i_first = rand() % i_population_size;
		do
		{
			i_second = rand() % i_population_size;
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
		if ((rand() % 100) < d_mutation_chance)
			(*pv_population)[ii]->vMutate();
}

void CPGAlg::v_cut()
{
	for (size_t ii = 0; ii < pv_population->size(); ii++)
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
