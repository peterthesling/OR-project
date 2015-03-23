#include "StdAfx.h"
#include "data.h"
#include "iostream"
using namespace std;
data::data(void)
{
	km_to_miles = 0.62137;
}


data::~data(void)
{
	edge_matrix.clear();
	transport.clear();
}

bool data::read_trans(string filename_transport)
{
	ifstream in;
	in.open(filename_transport);
	if (in.fail())
		return false;
	transport.resize(3); // change
	for (int i = 0; i < 3; i++)
	{	transport[i].resize(2); // change
		for (int j = 0; j < 2; j++)
			in >> transport[i][j];}
	in.close();
	return true;
}

bool data::read_dist(string filename_dist)
{
	string start, end;
	ifstream in;
	int start_node, end_node, temp, type;
	in.open(filename_dist);
	if (in.fail())
		return false;
	in >> nr_of_cities;
	adj_list.resize(nr_of_cities + 2); 
	
	edge_matrix.resize(nr_of_cities +2); // 11 cities + supersink + superstart
	for (int i = 0; i < nr_of_cities +2; i++)
		edge_matrix[i].resize(nr_of_cities+ 2);

	for (int j = 0; j <nr_of_cities +2; j++)
		for (int k = 0; k <nr_of_cities +2; k++)
		{
			edge_matrix[j][k].distance_time = 1000000000;	// set all values to high number at beginning
			edge_matrix[j][k].upper_bound = -1; // means there is no upper bound
			edge_matrix[j][k].flow = 0;
			edge_matrix[j][k].cost_per_ton = -1; // might still be changed
		}

	edge_matrix[0][1].distance_time = 0;
	edge_matrix[0][2].distance_time = 0;
	edge_matrix[0][1].cost_per_ton = 0;
	edge_matrix[0][2].cost_per_ton = 0;
	edge_matrix[9][12].distance_time = 0;
	edge_matrix[10][12].distance_time = 0;
	edge_matrix[11][12].distance_time = 0;
	edge_matrix[9][12].cost_per_ton = 0;
	edge_matrix[10][12].cost_per_ton = 0;
	edge_matrix[11][12].cost_per_ton = 0;


	adj_list[0].push_back(1); // Boston and jack. are incident to super nova
	adj_list[0].push_back(2);
	adj_list[9].push_back(12); // super sink connected to 3 cities
	adj_list[10].push_back(12);
	adj_list[11].push_back(12);

	while(!in.eof())
	{
		in >> start >> end >> temp >> type;
		
		start_node = get_node(start); // give every city a number
		end_node = get_node(end);
		if (adj_list[start_node].empty() || adj_list[start_node].back() != end_node)
			adj_list[start_node].push_back(end_node);
		edge_matrix[start_node][end_node].distance_time = temp*km_to_miles/(transport[type-1][1]); // distances* number to convert to miles / speed to get to time
																					// speed saved in second column of transport
	}
	in.close();
	return true;
}

bool data::read_cost(string filename_cost)
{
	int start_node, end_node, temp, type;
	string start, end;
	ifstream in;
	in.open(filename_cost);
	if (in.fail())
		return false;
	while(!in.eof())
	{
		in >> start >> end >> temp >> type;
		start_node = get_node(start); // give every city a number
		end_node = get_node(end);
		edge_matrix[start_node][end_node].cost_per_ton = temp/transport[type-1][0];
	}

	in.close();

	return true;
}



bool data::read_upperbound(string filename_upperbound)
{
	int start_node, end_node, temp, type;
	string start, end;
	ifstream in;
	in.open(filename_upperbound);
	if (in.fail())
		return false;
	while(!in.eof())
	{
		in >> start >> end >> temp >> type;
		start_node = get_node(start); // give every city a number
		end_node = get_node(end);
		edge_matrix[start_node][end_node].upper_bound = temp*transport[type-1][0];
	}
	in.close();
	return true;
}



bool data::read_supply(string filename_supply, algorithms & nerd)
{
	ifstream in;
	nerd.labels.resize(nr_of_cities + 2);
	string city;
	int temp, node;
	in.open(filename_supply);
	if (in.fail())
		return false;
	while(!in.eof())
	{
		in >> city >> temp;
		node = get_node(city);
		nerd.labels[node].supply = temp;
	}

	return true;
}

int data::get_node(string start)
{
	if (start == "Boston")					
		return 1;
	else if (start == "Jacksonville")
		return 2;
	else if (start == "Berlin")
		return 3;
	else if (start == "Hamburg")
		return 4;
	else if (start == "Istanbul")
		return 5;
	else if (start == "London")
		return 6;
	else if (start == "Rotterdam")
		return 7;
	else if (start == "Napoli")
		return 8;
	else if (start == "St.Petersburg")				
		return 9;			
	else if (start == "Moscow")				
		return 10;			
	else if (start == "Rostov")				
		return 11;	
	return -1;
}

int data::get_nr_of_cities()
{
	return nr_of_cities;
}

double data::get_dist(int i, int j)
{
	return edge_matrix[i][j].distance_time;
}
