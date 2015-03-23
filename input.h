#ifndef Input_H
#define Input_H
#pragma once
#include <vector>
#include "string"
#include "iostream"
#include "fstream"
#include "list"
#include "algorithms.h"
using namespace std;

class algorithms;
class input
{
public:
struct edge
	{
		double distance_time;
		double upper_bound;
		double cost_per_ton;
		double cost_per_ton_r; // costs residual for min cost
		double flow;
		double residual;
		double dist_label_F_W; // distance label floyd warshall algorithm
		int pred_F_W; // predecessor for floyd warshall alg.
	};

	input(void);
	~input(void);
	bool read_trans(string);
	bool read_dist(string, int);	
	bool read_cost(string);
	bool read_supply(string, algorithms &);
	bool read_upperbound(string filename_upperbound)	; // _M all arcs in the graph are infinity, some have restrictions
	bool read_upperbound_max(string); //_M alleen ships arcs are infinity
	void restart();
	int get_nr_of_cities();
	vector <list<int>> adj_list;
	double get_dist(int i, int j);
	bool read_d (string filename_d);
	vector <vector<edge>> edge_matrix;

private:
	vector <vector<int>> transport;
	int nr_of_cities;
	double km_to_miles;
};

#endif
