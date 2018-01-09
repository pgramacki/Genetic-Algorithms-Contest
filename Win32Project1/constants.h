#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once

// node type
#define VARIABLE 0	// variable
#define CONSTANT 1	// constant
#define ADD 2	// +
#define SUB 3	// -
#define MUL 4	// *
#define DIV 7	// /
#define SIN 5	// sin
#define COS 6	// cos

#define MAX_CONSTANT_VALUE 10000
#define MAX_TREE_DEPTH 5
#define DEFAULT_NUM_OF_VARIABLES 2
#define FIRST_OP_CODE 2
#define LAST_OP_CODE 7
#define BUFFOR_SIZE 1023

#define VERY_BIG_NUMBER 999999999

#define DEF_SOURCE_FILE "data.txt"
#define DEF_RESULT_FILE "result.txt"

#define DEF_POPULATION_SIZE 500
#define DEF_ITERATIONS_NUM 50
#define DEF_CROSSING_CHANCE 50
#define DEF_MUTATION_CHANCE 20

#define X_VAR "x"
#define Y_VAR "y"

// operations
#define S_ADD "+"
#define S_SUB "-"
#define S_MUL "*"
#define S_DIV "/"
#define S_SIN "sin"
#define S_COS "cos"

#define LEFT 0
#define RIGHT 1

// number of arguments
#define VAR_CONST_ARG_NUM 0
#define ADD_SUB_MUL_DIV_ARG_NUM 2
#define SIN_COS_ARG_NUM 1

// repairing expression
#define DEFAULT_CONSTANT "1"
#define DEFAULT_OPERATION ADD

// ASCII
#define SPACE ' '
#define S_SPACE " "
#define EMPTY_STRING ""
#define NEW_LINE "\n"
#define ASCII_MINUS 45
#define ASCII_FIRST_DIGIT 48
#define ASCII_LAST_DIGIT 57
#define ASCII_FIRST_CAPITAL 65
#define ASCII_LAST_CAPITAL 90
#define ASCII_FIRST_SMALL 97
#define ASCII_LAST_SMALL 122
#define DECIMAL_SYSTEM_BASE 10
#define ARROW " -> "
#define SEMICOLON ';'

// errors
#define NO_ERROR 0
#define WRONG_EXPRESSION 1
#define WRONG_VALUES 2
#define DIVIDE_BY_ZERO 3
#define WRONG_INSTRUCTION 4
#define WRONG_ARGUMENTS 5
#define NO_TREE_CREATED 6
#define NEGATIVE_DEPTH 7

// errors' messages
#define WRONG_EXPRESSION_MESSAGE " <- expression corrected due to mistake"
#define WRONG_VALUES_MESSAGE "wrong values"
#define DIVIDE_BY_ZERO_MESSAGE "algebraic error - division by 0"
#define WRONG_INSTRUCTION_MESSAGE "wrong instruction"
#define WRONG_ARGUMENTS_MESSAGE "wrong arguments"
#define NO_TREE_CREATED_MESSAGE "no tree created"

// interface
#define S_ENTER "enter"
#define S_VARS "vars"
#define S_PRINT "print"
#define S_COMPUTE "comp"
#define S_JOIN "join"
#define S_QUIT "quit"
#define S_PRINT_LEAF "printLeaf"

#define I_ENTER 11
#define I_VARS 12
#define I_PRINT 13
#define I_COMPUTE 14
#define I_JOIN 15
#define I_QUIT 16
#define I_PRINT_LEAF 17

#define CONST_TO_STR(x) #x

#endif //CONSTANTS_H