#pragma once
#include "vector"
#include "list"
#include "iostream"
#include "input.h"
#include "algorithms.h"
#include "minimum_spanning_tree.h"
using namespace std;

class solutions
{
public:
	solutions(void);
	~solutions(void);
	void calc_shortest_path(input & allies, algorithms & nerd);
	void min_cost(input &, algorithms &);
	void max_flow(input &, algorithms &);
	void min_tree(minimum_spanning_tree & tree);
private:
	vector <list<int>> shortest_path;
};

