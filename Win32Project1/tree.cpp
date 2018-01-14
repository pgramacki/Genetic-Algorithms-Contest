#include "tree.h"
#include <fstream>
#include <iostream>
using namespace std;

CTree::CTree(int iNumOfVariables /*= DEFAULT_NUM_OF_VARIABLES*/)
{
	i_error_code = NO_ERROR;
	i_num_of_vars = iNumOfVariables;
	pc_root = NULL;
	b_evaluated = false;
	d_accuracy = VERY_BIG_NUMBER;
}

CTree::CTree(CTree & pcOther)
{
	pc_root = NULL;
	v_copy_tree(pcOther);
}

CTree::~CTree()
{
	delete pc_root;
}

int CTree::iCreateFromString(string sExpression)
{
	pc_root = new CNode(this);

	pc_root->vCreateFromString(sExpression);

	return i_error_code;
}

void CTree::vCreateRandom()
{
	if (pc_root != NULL)
		delete pc_root;

	v_variables.push_back(X_VAR);
	v_variables.push_back(Y_VAR);

	pc_root = new CNode(this);

	pc_root->vCreateRandom(rand() % MAX_TREE_DEPTH);

	b_evaluated = false;
}

void CTree::vMutate()
{

	if (!pc_root->bIsLeaf())
		pc_root->vMutate();
	else
		vCreateRandom();

	b_evaluated = false;
}

void CTree::vCrossWith(CTree * pcOther)
{
	if (!pc_root->bIsLeaf() && !pcOther->pc_root->bIsLeaf())
	{
		CNode **pc_this_node = pc_root->pcChooseNodeToCross();
		CNode **pc_other_node = pcOther->pc_root->pcChooseNodeToCross();

		(*pc_this_node)->vChangeTree(pcOther);
		(*pc_other_node)->vChangeTree(this);

		CNode *pc_tmp = *pc_this_node;
		*pc_this_node = *pc_other_node;
		*pc_other_node = pc_tmp;

		b_evaluated = false;
		pcOther->b_evaluated = false;
	}
}

int CTree::vCutTree(int iMaxLevel)
{
	if (iMaxLevel < 0)
		return NEGATIVE_DEPTH;
	else pc_root->vCut(0, iMaxLevel);

	return NO_ERROR;

}

int CTree::iCalculateAccuracy(vector<CRecord *> &vValues)
{
	if (!b_evaluated)
	{
		d_accuracy = 0;
		double d_value_from_tree;
		int i_error_code = NO_ERROR;

		for (size_t ii = 0; ii < vValues.size(); ii++)
		{
			i_error_code = iComputeExpression(vValues[ii]->vGetArguments(), d_value_from_tree);

			if (i_error_code != NO_ERROR) {
				d_accuracy = VERY_BIG_NUMBER;
				return i_error_code;
			}

			d_accuracy += (d_value_from_tree - vValues[ii]->dGetResult())*(d_value_from_tree - vValues[ii]->dGetResult());
		}

		b_evaluated = true;
	}

	return NO_ERROR;
}

void CTree::vToFile(vector<CRecord*>& vValues)
{
	fstream f_file;
	f_file.open("results.txt", ios::out);
	double d_value_from_tree;

	for (size_t ii = 0; ii < vValues.size(); ii++)
	{
		iComputeExpression(vValues[ii]->vGetArguments(), d_value_from_tree);
		f_file << vValues[ii]->vGetArguments()[0] << "; " << vValues[ii]->vGetArguments()[1] << "; " << d_value_from_tree << endl;
	}

	f_file.close();
}

string CTree::sPrintTree()
{
	return pc_root->sPrint();
}

vector<string> CTree::vsGetVariables()
{
	if (v_variables.empty())
		pc_root->vGetVariables();

	return v_variables;
}

int CTree::iComputeExpression(vector<double> &vValues, double & dResult)
{
	i_error_code = NO_ERROR;

	if (v_variables.empty())		// chceck if there is a list of variables stored
		pc_root->vGetVariables();	// if not - create it

	//if (v_variables.size() != vValues.size())	// chceck if number of arguments is ok
	//	return WRONG_VALUES;

	dResult = pc_root->dCompute(vValues);	// compute expression
	return i_error_code;
}

void CTree::operator=(CTree & pcOther)
{
	v_copy_tree(pcOther);
}

CTree CTree::operator+(CTree & pcRight)
{
	CTree c_result(*this);

	//c_result.pc_root;

	c_result.v_join_with(pcRight);

	return c_result;
}

vector<string> CTree::vsPrintLeaf()
{
	return pc_root->vsPrintLeaf();
}

void CTree::v_copy_tree(CTree & pcOther)
{
	if (pc_root != NULL)
		delete pc_root;

	pc_root = new CNode(*pcOther.pc_root, this);
	i_error_code = pcOther.i_error_code;
	v_variables = pcOther.v_variables;
	i_num_of_vars = pcOther.i_num_of_vars;
	b_evaluated = pcOther.b_evaluated;
	d_accuracy = pcOther.d_accuracy;
}

void CTree::v_join_with(CTree &pcOther)
{
	if (pc_root == NULL || pc_root->bIsLeaf())
	{
		if (pc_root != NULL) delete pc_root;
		pc_root = new CNode(*pcOther.pc_root, this);
	}
	else pc_root->vJoin(pcOther.pc_root);

	v_variables.clear();
}

//////////////////////////////////////////////////////////////////////////////////////

CNode::CNode()
{

}

CNode::CNode(CTree * pcTree)
{
	pc_tree = pcTree;
}

CNode::CNode(CNode & pcOther, CTree *pcTree)
{
	pc_tree = pcTree;

	i_field_type = pcOther.i_field_type;
	s_var_name = pcOther.s_var_name;
	d_constant_val = pcOther.d_constant_val;

	for (size_t ii = 0; ii < pcOther.v_children.size(); ii++)
		v_children.push_back(new CNode(*pcOther.v_children[ii], pc_tree));
}


CNode::~CNode()
{
	for (size_t ii = 0; ii < v_children.size(); ii++)	// delete all children
		delete v_children[ii];
}

void CNode::vCreateFromString(string & sExpression)
{
	int i_position = 0;

	v_create_tree(sExpression, i_position);

	if (i_position < sExpression.size())
		pc_tree->i_error_code = WRONG_EXPRESSION;
}

void CNode::vCreateRandom(int iMaxDepth)
{
	v_create_random(0, iMaxDepth);
}

void CNode::vMutate()
{
	int i_child_to_mutate = rand() % v_children.size();

	if (!v_children[i_child_to_mutate]->bIsLeaf() && rand() % 2) // now it's 50% chance no to mutate a child
		v_children[i_child_to_mutate]->vMutate();
	else
	{
		delete v_children[i_child_to_mutate];
		v_children[i_child_to_mutate] = new CNode(pc_tree);
		v_children[i_child_to_mutate]->vCreateRandom(rand() % MAX_TREE_DEPTH);
	}
}

void CNode::vCut(int iCurrentLevel, int iMaxLevel)
{
	if (iCurrentLevel == iMaxLevel)
		if (!bIsLeaf())
		{
			v_random_leaf();
			pc_tree->b_evaluated = false;
		}
		else;
	else
		for (size_t ii = 0; ii < v_children.size(); ii++)
			v_children[ii]->vCut(iCurrentLevel + 1, iMaxLevel);
}

CNode ** CNode::pcChooseNodeToCross()
{
	int i_child_to_mutate = rand() % v_children.size();

	if (!v_children[i_child_to_mutate]->bIsLeaf() && rand() % 2)
		return v_children[i_child_to_mutate]->pcChooseNodeToCross();
	else
		return &v_children[i_child_to_mutate];
}

string CNode::sPrint()
{
	string s_result;

	s_result = s_to_string();

	return s_result.substr(0, s_result.size() - 1);	// remove last space
}

void CNode::vGetVariables()
{
	if (i_field_type == VARIABLE && !b_vector_contains_variable())
		pc_tree->v_variables.push_back(s_var_name);

	for (size_t ii = 0; ii < v_children.size(); ii++)
		v_children[ii]->vGetVariables();
}

double CNode::dCompute(vector<double> &vValues)
{
	if (pc_tree->i_error_code != NO_ERROR)
		return 0;
	else {
		double right;

		switch (i_field_type)
		{
		case CONSTANT:
			return d_constant_val;
			break;
		case VARIABLE:
			double d_val;
			for (size_t ii = 0; ii < vValues.size(); ii++)
				if (pc_tree->v_variables[ii] == s_var_name)
					d_val = vValues[ii];
			return d_val;
			break;
		case ADD:
			return v_children[LEFT]->dCompute(vValues) + v_children[RIGHT]->dCompute(vValues);
			break;
		case SUB:
			return v_children[LEFT]->dCompute(vValues) - v_children[RIGHT]->dCompute(vValues);
			break;
		case MUL:
			return v_children[LEFT]->dCompute(vValues) * v_children[RIGHT]->dCompute(vValues);
			break;
		case DIV:
			right = v_children[RIGHT]->dCompute(vValues);
			if (right == 0) {
				pc_tree->i_error_code = DIVIDE_BY_ZERO;
				return 0;
			}
			else return v_children[LEFT]->dCompute(vValues) / right;
			break;
		case SIN:
			return sin(v_children[0]->dCompute(vValues));
			break;
		case COS:
			return cos(v_children[0]->dCompute(vValues));
			break;
		default:
			return 0;
			break;
		}
	}
}

void CNode::vJoin(CNode * pcOther)
{
	//srand(time(0));

	int i_index = rand() % v_children.size();

	if (v_children[i_index]->bIsLeaf()) {
		delete v_children[i_index];
		v_children[i_index] = new CNode(*pcOther, pc_tree);
	}
	else v_children[i_index]->vJoin(pcOther);
}

vector<string> CNode::vsPrintLeaf()
{
	vector<string> v_result;
	v_print_leaf(EMPTY_STRING, v_result);

	return v_result;
}

bool CNode::bIsLeaf()
{
	return i_field_type == VARIABLE || i_field_type == CONSTANT;
}

void CNode::vChangeTree(CTree * pcNewTree)
{
	pc_tree = pcNewTree;
	for (size_t ii = 0; ii < v_children.size(); ii++)
		v_children[ii]->vChangeTree(pcNewTree);
}

void CNode::v_print_leaf(string s_previous_nodes, vector<string> & v_result)
{
	if (i_field_type == VARIABLE || i_field_type == CONSTANT)
		v_result.push_back(s_previous_nodes + ARROW + s_print_node());
	else
	{
		if (this != pc_tree->pc_root) s_previous_nodes += ARROW;
		for (size_t ii = 0; ii < v_children.size(); ii++)
			v_children[ii]->v_print_leaf(s_previous_nodes + s_print_node(), v_result);
	}
}

/* creates node and correct number of children */
void CNode::v_create_tree(string & s_expression, int & i_start_position)
{
	string s_part;
	bool b_continiue = false;
	do {
		s_part = s_get_substring(i_start_position, s_expression);	// get one element of expression
		if (s_part == EMPTY_STRING) {		// if there is nothing left in expression - use default value "1"
			s_part = DEFAULT_CONSTANT;
			pc_tree->i_error_code = WRONG_EXPRESSION;
		}
		v_check_type(s_part);	// and use it to chceck type of node
		b_continiue = i_field_type == VARIABLE && s_var_name.size() == 0;  // do this to avoid situation when there id %% in expression and it will be stored as ""
	} while (b_continiue);

	if (i_field_type != VARIABLE && i_field_type != CONSTANT)	// chceck if there will be children
		if (i_field_type == SIN || i_field_type == COS)		// chceck how many children
			v_create_children(SIN_COS_ARG_NUM, s_expression, i_start_position);
		else v_create_children(ADD_SUB_MUL_DIV_ARG_NUM, s_expression, i_start_position);

		/* this part repairs expression if no operator is given
		else if (this == pc_tree->pc_root)
		{
		i_start_position = 0;
		i_field_type = DEFAULT_OPERATION;

		v_create_children(ADD_SUB_MUL_DIV_ARG_NUM, s_expression, i_start_position);
		}
		*/
}

void CNode::v_create_children(int i_number, string s_expression, int & i_start_position)
{
	for (int ii = 0; ii < i_number; ii++)
	{
		CNode *c_child = new CNode(pc_tree);		// create child
		c_child->v_create_tree(s_expression, i_start_position);	// and make it read from expression
		v_children.push_back(c_child);	// add it to vector
	}
}

/* returns one element form expression starting at i_start */
string CNode::s_get_substring(int & i_start, string s_line)
{
	string s_result;

	while (i_start < s_line.length() && s_line.at(i_start) == SPACE)
		i_start++;

	while (i_start < s_line.length() && s_line.at(i_start) != SPACE)
		s_result += s_line.at(i_start++);

	return s_result;
}

/* removes wrong symbols from variable name */
void CNode::v_remove_wrong_symbols(string &s_part)
{
	string s_result;

	for (unsigned int ii = 0; ii < s_part.size(); ii++)
		if (b_correct_symbol(s_part[ii]))
			s_result += s_part[ii];
		else pc_tree->i_error_code = WRONG_EXPRESSION;

		s_part = s_result;
}

/* chceck if char is correct element of variable name - digit or letter */
bool CNode::b_correct_symbol(char c_symbol)
{
	return (c_symbol >= ASCII_FIRST_DIGIT && c_symbol <= ASCII_LAST_DIGIT) ||
		(c_symbol >= ASCII_FIRST_CAPITAL && c_symbol <= ASCII_LAST_CAPITAL) ||
		(c_symbol >= ASCII_FIRST_SMALL && c_symbol <= ASCII_LAST_SMALL);
}

/* chcek if given string contains a number */
bool CNode::b_is_number(string s_line)
{
	bool b_is_number = s_line.length() != 0;

	for (int ii = s_line.at(0) == ASCII_MINUS ? 1 : 0; ii < s_line.length() && b_is_number; ii++) {
		char c_sign = s_line.at(ii);
		if ((int)c_sign < ASCII_FIRST_DIGIT || (int)c_sign > ASCII_LAST_DIGIT)
			b_is_number = false;
	}

	return b_is_number;

}

/* converts string to int */
int CNode::i_parse_to_int(string s_number)
{
	int i_result = 0;

	for (unsigned int ii = s_number.at(0) == ASCII_MINUS ? 1 : 0; ii < s_number.length(); ii++) {
		i_result *= DECIMAL_SYSTEM_BASE;
		i_result += (int)s_number.at(ii) - ASCII_FIRST_DIGIT;
	}

	return s_number.at(0) == ASCII_MINUS ? i_result*(-1) : i_result;

}

/* chceck the type of node regarding given part of expression */
void CNode::v_check_type(string &s_single)
{
	if (s_single == S_ADD)
		i_field_type = ADD;
	else if (s_single == S_SUB)
		i_field_type = SUB;
	else if (s_single == S_MUL)
		i_field_type = MUL;
	else if (s_single == S_DIV)
		i_field_type = DIV;
	else if (s_single == S_SIN)
		i_field_type = SIN;
	else if (s_single == S_COS)
		i_field_type = COS;
	else if (b_is_number(s_single))
	{
		i_field_type = CONSTANT;
		d_constant_val = i_parse_to_int(s_single);
	}
	else
	{
		i_field_type = VARIABLE;
		v_remove_wrong_symbols(s_single);
		s_var_name = s_single;
	}
}

/* return prefix representation of node and it's children */
string CNode::s_to_string()
{
	string s_result;

	if (i_field_type == CONSTANT)
		s_result = to_string((int)d_constant_val) + S_SPACE;
	else if (i_field_type == VARIABLE)
		s_result = s_var_name + S_SPACE;
	else
	{
		s_result = s_print_node() + S_SPACE;
		for (size_t ii = 0; ii < v_children.size(); ii++)
			s_result += v_children[ii]->s_to_string();
	}

	return s_result;
}

/* additional method to print operator */
string CNode::s_print_node()
{
	string s_result;

	switch (i_field_type)
	{
	case ADD:
		s_result = S_ADD;
		break;
	case SUB:
		s_result = S_SUB;
		break;
	case MUL:
		s_result = S_MUL;
		break;
	case DIV:
		s_result = S_DIV;
		break;
	case SIN:
		s_result = S_SIN;
		break;
	case VARIABLE:
		s_result = s_var_name;
		break;
	case CONSTANT:
		s_result = to_string(d_constant_val);
		break;
	default:
		s_result = S_COS;
		break;
	}

	return s_result;
}

/* chceck if given variable is in tree */
bool CNode::b_vector_contains_variable()
{
	bool b_result = false;

	for (size_t ii = 0; ii < pc_tree->v_variables.size() && !b_result; ii++)
		if (pc_tree->v_variables[ii] == s_var_name)
			b_result = true;

	return b_result;
}

void CNode::v_create_random(int i_current_depth, int i_max_depth)
{
	if (i_current_depth == i_max_depth || !(rand() % 3))
		v_random_leaf();
	else v_random_node(i_current_depth, i_max_depth);
}

void CNode::v_random_leaf()
{
	for (size_t ii = 0; ii < v_children.size(); ii++)
		delete v_children[ii];

	v_children.clear();

	if (rand() % 3)
	{
		i_field_type = VARIABLE;
		s_var_name = s_random_variable();
	}
	else
	{
		i_field_type = CONSTANT;
		d_constant_val = (rand() % (2 * MAX_CONSTANT_VALUE)) - MAX_CONSTANT_VALUE;
	}
}

void CNode::v_random_node(int i_current_depth, int i_max_depth)
{
	//srand(time(0));

	i_field_type = FIRST_OP_CODE + rand() % (LAST_OP_CODE + 1 - FIRST_OP_CODE);

	if (i_field_type != SIN && i_field_type != COS)
		v_children.push_back(new CNode(pc_tree));
	v_children.push_back(new CNode(pc_tree));

	for (size_t ii = 0; ii < v_children.size(); ii++)
		v_children[ii]->v_create_random(i_current_depth + 1, i_max_depth);
}

string CNode::s_random_variable()
{
	return rand() % 2 == 0 ? X_VAR : Y_VAR;
}


