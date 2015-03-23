// Aiding Allies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "algorithms.h"
#include "solutions.h" 
#include "iostream"
#include "input.h"
#include "minimum_spanning_tree.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	input allies;
	algorithms nerd;
	solutions yeah;
	minimum_spanning_tree tree;
	//yeah.calc_shortest_path(allies, nerd);
	//yeah.min_cost(allies, nerd);
	//yeah.max_flow(allies, nerd);
	yeah.min_tree(tree);
	nerd.labelling(allies);
	system("pause");
	return 0;
}

