#include "StdAfx.h"
#include "minimum_spanning_tree.h"
#include "fstream"


minimum_spanning_tree::minimum_spanning_tree(void)
{
}

minimum_spanning_tree::~minimum_spanning_tree(void)
{
}

bool minimum_spanning_tree::read_dist(string filename_dist)																																									
{																																									
	ifstream in;																																								
	int start_node, end_node, elements_of_data, i=0;
	double cost;																																								
	in.open(filename_dist);																																								
	if (in.fail())																																								
		return false;																																							
	in >> nr_of_cities;

	in >> elements_of_data;
	LIST.resize(elements_of_data);

	while(!in.eof())																																								
	{																																								
		in >> start_node >> end_node >> cost;																																							
		LIST[i].begin = start_node -1; // other way? otherwise error in nodes
		LIST[i].end = end_node -1; // other way? otherwise error in nodes
		LIST[i].cost = cost;
		i++;
	}																																								
	in.close();																																								
	return true;																																								
}																																									

//correct
void minimum_spanning_tree::quick_sort(int begin, int end)
{
	struct edges temp;

	if (end > begin)
	{
		int a = begin;
		int b = end;
		double x = LIST[begin].cost;
		while (a != b)
		{
			if (LIST[a + 1].cost <= x)
			{
				temp = LIST[a];
				LIST[a] = LIST[a+1];
				LIST[a+1] = temp;
				a++;
			}
			else 
			{
				temp = LIST[a+1];
				LIST[a+1] = LIST[b];
				LIST[b] = temp;
				b--;
			}
		}
		quick_sort(begin, a - 1);
		quick_sort(b + 1, end);
	}
}

void minimum_spanning_tree::create()
{
	quick_sort(0,LIST.size()-1); // in solutions?? 

	tree.resize(nr_of_cities -1);
	nodes.resize(nr_of_cities);
	max_label =0;
	int x=0;

	for(int i=0; i< (int)LIST.size(); i++)
		{
			if(nodes[LIST[i].begin] == nodes[LIST[i].end] && nodes[LIST[i].begin]==0) // check if labels of the nodes are the same, i.e. cycles
			{	
				set_labels(LIST[i].begin, LIST[i].end); // update de nodes to labelled
				tree[x]=LIST[i];
				if (x!= tree.size()-1) 
					x++;
			}
			else if(nodes[LIST[i].begin] != nodes[LIST[i].end]) // check if labels of the nodes are the same, i.e. cycles
			{	
				set_labels(LIST[i].begin, LIST[i].end); // update de nodes to labelled
				tree[x]=LIST[i];
				if (x!= tree.size()-1) 
					x++;
			}
		}
}

int minimum_spanning_tree::get_nr_cities()																																									
{																																									
	return nr_of_cities;																																								
}	

void minimum_spanning_tree::set_labels (int start, int end)
{
	if (nodes[start] == 0 && nodes[end] == 0)
	{
		max_label++;
		nodes[start] = max_label;
		nodes[end] = max_label;
	}
	else if (nodes[start] == 0 && nodes[end] != 0)	
	{
		nodes[start] = nodes[end];
	}
	else if (nodes[end] == 0 && nodes[start] != 0)	
	{
		nodes[end] = nodes[start];
	}
	else if (nodes[start] != 0 && nodes[end] != 0)
	{	
		if (nodes[start] < nodes[end])
			nodes[end]=nodes[start];
		else
			nodes[start] = nodes[end];
	}
}

void minimum_spanning_tree::display()
{
	double total_cost=0;
	cout << "The minimum spanning tree consist out of the following arcs: " << endl;
	for (int i = 0; i <  (int)tree.size(); i++)
	{	
		cout << "There is a connection between " << tree[i].begin + 1 << " to city " << tree[i].end + 1 << " with costs " << tree[i].cost << ". " << endl;
		total_cost += tree[i].cost;
	}
	cout << "The total cost of the connection is " << total_cost <<  ". " << endl;
} // at the end of the algorithm he says; vector subscript out of range
