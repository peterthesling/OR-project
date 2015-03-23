#pragma once
#include "list"
#include "iostream"
#include "vector"
using namespace std;

class minimum_spanning_tree
{


public:
	minimum_spanning_tree(void);
	~minimum_spanning_tree(void);
	
	struct edges
	{
		int begin;
		int end;
		double cost;
	};

	bool read_dist(string);			
	void quick_sort(int, int);
	void create();
	int get_nr_cities();
	void set_labels (int, int);
	void display();

private:
	int nr_of_cities;
	vector <edges> LIST; // the original list with edges
	vector <edges> tree; // making the tree
	vector <int> nodes; // checking of nodes are labeled
	int max_label; 
};

