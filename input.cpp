#include "StdAfx.h"																																									
#include "input.h"																																									
#include "iostream"																																									
using namespace std;																																									
input::input(void)																																									
{																																									
	km_to_miles = 0.62137;																																								
}																																									
																																									
input::~input(void)																																									
{																																									
	edge_matrix.clear();																																								
	transport.clear();																																								
}																																									
																																									
bool input::read_trans(string filename_transport)																																									
{																																									
	ifstream in;																																								
	in.open(filename_transport);																																								
	if (in.fail())																																								
		return false;																																							
	transport.resize(4); // change																																								
	for (int i = 0; i < 4; i++)																																								
	{																																								
		transport[i].resize(2); // change																																							
		for (int j = 0; j < 2; j++)																																							
			in >> transport[i][j];																																						
	}																																								
	in.close();																																								
	return true;																																								
}																																									
																																									
bool input::read_dist(string filename_dist, int a)																																									
{																																									
	ifstream in;																																								
	int start_node, end_node, temp, type;																																								
	in.open(filename_dist);																																								
	if (in.fail())																																								
		return false;																																							
	in >> nr_of_cities;
																														
	edge_matrix.resize(get_nr_of_cities()); // 11 cities + supersink + superstart																																								
	for (int i = 0; i < get_nr_of_cities(); i++)																																								
		edge_matrix[i].resize(get_nr_of_cities());
	if (a = 0)
		restart(); //_M


	adj_list.resize(get_nr_of_cities()); 																														
	while(!in.eof())																																								
	{																																								
		in >> start_node >> end_node >> temp >> type;																																							
		if (adj_list[start_node].empty() || adj_list[start_node].back() != end_node)																												
				adj_list[start_node].push_back(end_node);																																						
		edge_matrix[start_node][end_node].distance_time = temp*km_to_miles/(transport[type-1][1]); // distances* number to convert to miles / speed to get to time																																							

																			// speed saved in second column of transport																				
	}																																								
	in.close();																																								
	return true;																																								
}																																									
																																									
bool input::read_cost(string filename_cost)																																									
{																																									
	int start_node, end_node, temp, type;																																								
	ifstream in;																																								
	in.open(filename_cost);																																								
	if (in.fail())																																								
		return false;																																							
	while(!in.eof())																																								
	{																																								
		in >> start_node >> end_node >> temp >> type;																																							
		edge_matrix[start_node][end_node].cost_per_ton = temp/transport[type-1][0];																																							
	}																																								
	in.close();																																								
	return true;																																								
}																																									

bool input::read_upperbound(string filename_upperbound)																																									
{																																									
	int start, end, temp, type;																																								
	ifstream in;																																								
	in.open(filename_upperbound);																																								
	if (in.fail())																																								
		return false;																																							
	
	list <int>::iterator it;									
	for (int i=0; i<get_nr_of_cities(); i++)	
		for (it=adj_list[i].begin(); it != adj_list[i].end(); it++)	
			edge_matrix[i][*it].upper_bound = 1000000;

	while(!in.eof())																																								
	{																																								
		in >> start >> end >> temp >> type;																																							
		edge_matrix[start][end].upper_bound = temp*transport[type-1][0];
	}																																								
																																									
	in.close();																																								
	return true;																																								
}					

bool input::read_upperbound_max(string filename_upperbound)																																									
{																																									
	int start, end, temp, type;																																								
	ifstream in;																																								
	in.open(filename_upperbound);																																								
	if (in.fail())																																								
		return false;																																							
	
	edge_matrix[0][1].upper_bound = 1000000;			
	edge_matrix[0][2].upper_bound = 1000000;	
	edge_matrix[9][12].upper_bound = 1000000;	
	edge_matrix[10][12].upper_bound = 1000000;	
	edge_matrix[11][12].upper_bound = 1000000;	

	while(!in.eof())																																								
	{																																								
		in >> start >> end >> temp >> type;																																							
		edge_matrix[start][end].upper_bound = temp*transport[type-1][0];

		if (type == 2)
			edge_matrix[start][end].upper_bound = 1000000;
	}																																								
																																									
	in.close();																																								
	return true;																																								
}																																									
																																									
bool input::read_supply(string filename_supply, algorithms & nerd)																																									
{																																									
	ifstream in;																																								
	nerd.labels.resize(get_nr_of_cities());																																								
	int city, temp;																																								
	in.open(filename_supply);																																								
	if (in.fail())																																								
		return false;																																							
	while(!in.eof())																																								
	{																																								
		in >> city >> temp;																																							
		nerd.labels[city].supply = temp;																																							
	}																																								
																																									
	return true;																																								
}																																									
																																									
bool input::read_d (string filename_d)																																									
{																																									
	ifstream in;																																								
	int start, end, temp, type;																																								
	in.open(filename_d);																																								
	if (in.fail())																																								
		return false;																																							
	while(!in.eof())																																								
	{																																								
		in >> start >> end >> temp >> type;																																							
		edge_matrix[start][end].upper_bound = temp*transport[type-1][0];																																							
	}																																								
	return true;																																								
}																																									
																																									
int input::get_nr_of_cities()																																									
{																																									
	return nr_of_cities + 2;	// supersink, supersource and superman																																							
}																																									
																																									
double input::get_dist(int i, int j)																																									
{																																									
	return edge_matrix[i][j].distance_time;																																								
}																																									
																																									
void input::restart()																																									
{	
	adj_list.clear();
	read_dist("distances_nr.txt", 1);
	for (int j = 0; j <get_nr_of_cities(); j++)																																								
		for (int k = 0; k <get_nr_of_cities(); k++)																																								
		{																																								
			edge_matrix[j][k].distance_time = 1000000000;	// set all values to high number at beginning																																						
			edge_matrix[j][k].flow = 0;																																							
			edge_matrix[j][k].cost_per_ton = -1; // might still be changed																																							
			edge_matrix[j][k].cost_per_ton_r = 0;																																							
			edge_matrix[j][k].residual = 0;																																							
			edge_matrix[j][k].dist_label_F_W = 1000000; // in input																																							
			edge_matrix[j][k].pred_F_W = -1;
			edge_matrix[j][k].upper_bound = 0; //_M Arc 8-->9 has flow now
		}																																								
}																																									
