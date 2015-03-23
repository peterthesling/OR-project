#include "StdAfx.h"
#include "solutions.h"



solutions::solutions(void)
{
}

solutions::~solutions(void)
{
}

void solutions::calc_shortest_path(input & allies, algorithms & nerd)
{
	allies.read_trans("transport.txt");
	allies.read_dist("distances_nr.txt", 0);
	nerd.restart(allies);
	nerd.Dijkstra(allies);
	shortest_path.resize(allies.get_nr_of_cities());

	// (b)
	for (int i = allies.get_nr_of_cities() - 1; i > 0; i--) // shortest path from start node superflous
	{
		shortest_path[i].push_back(nerd.labels[i].pred);
		for (int j = allies.get_nr_of_cities() - 1; j > 0; j--)
		{
			if (!shortest_path[i].empty())
			{
				if (j == shortest_path[i].front())
					shortest_path[i].push_front(nerd.labels[j].pred);
			}
		}
	}
	list <int>:: iterator it;
	for (int i = 9; i < 12; i++)
	{
		cout << "For city number " << i << " we have the following shortest path:" << endl;	
		for (it = shortest_path[i].begin(); it != shortest_path[i].end(); it++)
			cout << *it << "	";
		cout << endl;
		cout << "The time needed for this path is " << nerd.labels[i].value << ". " << endl << endl;
	}
}

void solutions::min_cost(input & allies, algorithms & nerd)
{
	allies.read_trans("transport.txt");
	allies.read_dist("distances_nr.txt", 0);
	nerd.restart(allies);
	allies.read_cost("costs.txt");
	allies.read_upperbound("upperbounds.txt");
	allies.read_supply("supply.txt", nerd);
	nerd.min_cost(allies);

	for (int i = 0; i < allies.get_nr_of_cities(); i++)
		for (int j = 1; j < allies.get_nr_of_cities(); j++)
			if (allies.edge_matrix[i][j].flow > 0) 
				cout << "There is a flow from city " << i << " to city " << j << " of " << allies.edge_matrix[i][j].flow << " . " << endl;

	cout << "The total cost is equal to " << nerd.total_flow << " . " << endl;
}

void solutions::max_flow(input & allies, algorithms & nerd)
{
	allies.read_trans("transport.txt");
	allies.read_dist("distances_nr.txt", 0); // _M
	nerd.restart(allies); // _M
	allies.read_upperbound_max("upperbounds_d.txt"); //_M
	nerd.labelling(allies); 
	for (int i = 0; i < allies.get_nr_of_cities(); i++)
		for (int j = 1; j < allies.get_nr_of_cities(); j++) //_M -- flow from city 0 to 0
			if (allies.edge_matrix[i][j].flow > 0)
				cout << "There is a flow from city " << i << " to city " << j << " of " << allies.edge_matrix[i][j].flow << " . " << endl;
	cout << "The total flow is equal to " << allies.edge_matrix[0][1].flow + allies.edge_matrix[0][2].flow <<  " . " << endl;
}

void solutions::min_tree(minimum_spanning_tree & tree)
{
	tree.read_dist("minimum_spanning_tree.txt"); //correct
	tree.create(); // correct
	tree.display(); // vector subscript out of range at the end of the algortihm?
}
