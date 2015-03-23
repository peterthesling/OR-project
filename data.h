#ifndef DATA_H
#define DATA_H
#pragma once
#include <vector>
#include "string"
#include "iostream"
#include "fstream"
#include "list"
#include "algorithms.h"
using namespace std;

class algorithms;
class data
{
public:
struct edge
	{
		double distance_time;
		double upper_bound;
		double cost_per_ton;
		double flow;
		double residual;
	};
	data(void);
	~data(void);
	bool read_trans(string);
	bool read_dist(string);	
	bool read_cost(string);
	bool read_supply(string, algorithms &);
	bool read_upperbound(string);
	int get_nr_of_cities();
	vector <list<int>> adj_list;
	double get_dist(int i, int j);
	int get_node(string);
	vector <vector<edge>> edge_matrix;

private:
	vector <vector<int>> transport;
	int nr_of_cities;
	double km_to_miles;
};

#endif
