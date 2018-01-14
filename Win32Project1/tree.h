#pragma once
#include "constants.h"
#include "data.h"

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

//class CNode;

class CTree
{
	friend class CNode;

public:
	CTree(int iNumOfVariables = DEFAULT_NUM_OF_VARIABLES);
	CTree(CTree &pcOther);
	~CTree();

	int iCreateFromString(string sExpression);
	void vCreateRandom();
	void vMutate();
	void vCrossWith(CTree *pcOther);
	int vCutTree(int iMaxLevel);
	int iCalculateAccuracy(vector<CRecord *> &vValues);
	void vToFile(vector<CRecord *> &vValues);
	double dGetAccuracy() { return d_accuracy; }
	string sPrintTree();
	vector <string> vsGetVariables();
	int iComputeExpression(vector <double> &vValues, double &dResult);
	void operator= (CTree &pcOther);
	CTree operator+ (CTree &pcRight);
	vector<string> vsPrintLeaf();

private:
	CNode *pc_root;		// pointer to tree's root
	int i_error_code;	// field will hold posible errors
	int i_num_of_vars;	// number of variables in tree
	double d_accuracy;
	bool b_evaluated;
	vector <string> v_variables;

	void v_copy_tree(CTree &pcOther);
	void v_join_with(CTree &pcOther);
};

class CNode
{
public:
	CNode();
	CNode(CTree *pcTree);
	CNode(CNode &pcOther, CTree *pcTree);
	~CNode();

	void vCreateFromString(string &sExpression);
	void vCreateRandom(int iMaxDepth);
	void vMutate();
	void vCut(int iCurrentLevel, int iMaxLevel);
	CNode** pcChooseNodeToCross();
	string sPrint();
	void vGetVariables();
	double dCompute(vector<double> &vValues);
	void vJoin(CNode *pcOther);
	vector<string> vsPrintLeaf();
	bool bIsLeaf();
	void vChangeTree(CTree *pcNewTree);

private:
	CTree *pc_tree;					// poiter to tree which part is this node
	int i_field_type;				// 0 -> variable, 1 -> constant, 2-7 -> operations
	string s_var_name;				// symbol of variable
	double d_constant_val;				// value of constant
	vector <CNode *> v_children;	// children of this node

	void v_print_leaf(string s_previous_nodes, vector<string> & v_result);
	void v_create_tree(string &s_expression, int &i_start_position);
	void v_create_children(int i_number, string s_expression, int & i_position);
	string s_get_substring(int &i_start, string s_line);
	void v_remove_wrong_symbols(string &s_part);
	bool b_correct_symbol(char c_symbol);
	bool b_is_number(string s_line);
	int i_parse_to_int(string s_number);
	void v_check_type(string &s_single);
	string s_to_string();
	string s_print_node();
	bool b_vector_contains_variable();
	void v_create_random(int i_current_depth, int i_max_depth);
	void v_random_leaf();
	void v_random_node(int i_current_depth, int i_max_depth);
	string s_random_variable();
};

