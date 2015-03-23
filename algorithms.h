#pragma once


#include "vector"
#include "iostream"
#include "input.h"
using namespace std;

class input;
class algorithms
{
public:
	struct struct_label
	{
		double value;
		int in_S;
		int in_LIST;	// labelling
		int pred;
		int supply;
	};
	algorithms(void);
	~algorithms(void);
	void min_cost (input &);
	bool floyd_warshall(input&, int&, int&);
	void Dijkstra(input &);
	void labelling (input &);
	void restart(input &);
	void start_residual (input &);
	void augment (input &);
	bool augment_cycle_F_W(vector <int> &, input &, int&, int&);
	vector <struct_label> labels;
	double total_flow;

	
};

