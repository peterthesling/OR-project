#include "StdAfx.h"																																									
#include "algorithms.h"																																									
#include "queue" 																																									
																																									
using namespace std;																																									
algorithms::algorithms(void)																																									
{																																									
}																																									
																																									
																																									
algorithms::~algorithms(void)																																									
{																																									
}																																									
																																									
void algorithms::Dijkstra(input & Allies)																																									
{																																									
	bool check = false;																																								
	int min_index;																																								
	labels.resize(Allies.get_nr_of_cities() );																																								
	labels[0].value = 0;																																								
																																									
	for (int j = 1; j < Allies.get_nr_of_cities() ; j++)																																								
		labels[j].value= 1000000;																																							
																																									
	while (!check)																																								
	{																																								
		double min_temp = -1;																																							
		for (int i = 0; i < Allies.get_nr_of_cities() ; i++)																																							
			if (labels[i].in_S != 1)																																						
			{																																						
				if (min_temp == -1 || labels[i].value < min_temp)																																					
				{																																					
					min_temp = labels[i].value;																																				
					min_index = i;																																				
				}																																					
			}																																						
			labels[min_index].in_S = 1;																																						
																																									
			list <int>::iterator it;				// also means that min_index is now permanently saved																																		
			for (it = Allies.adj_list[min_index].begin(); it != Allies.adj_list[min_index].end(); it++)																																						
				if (labels[min_index].value + Allies.get_dist(min_index,*it) < labels[*it].value) // l(u) + c(u,v) < l(v)																																					
				{																																					
					labels[*it].value = labels[min_index].value + Allies.get_dist(min_index,*it);																																				
					labels[*it].pred = min_index;																																				
				}																																					
																																									
			// check if S = V																																						
			check = true;																																						
		for (int i = 0; i < Allies.get_nr_of_cities() ; i++)																																							
			if (labels[i].in_S != 1)																																						
				check = false;																																					
	}																																								
}																																									
																																									
void algorithms::min_cost(input & Allies)																					// speed saved in second column of transport																				
{																																									
	list <int>::iterator it;																																								
	int i = 0, k = 0, x;																																								
	total_flow = 0;																																								
	vector <int> cycle;	

	// Redifining Flow from super source to super sink. _M
	for (it=Allies.adj_list[0].begin(); it != Allies.adj_list[0].end(); it++)											
	{											
		labels[0].supply+=labels[*it].supply;			// labels[1].supply is zero							
		Allies.edge_matrix[0][*it].upper_bound=labels[*it].supply;										
		labels[*it].supply=0; // transhipment note										
	}											
												
	for (int i=9; i<12; i++) // ADJUST?!!											
	{											
		labels[12].supply+=labels[i].supply;										
		Allies.edge_matrix[i][12].upper_bound = (-1)*labels[i].supply;										
		labels[i].supply=0;										
	}	

	labelling(Allies);	
	for (int s = 0; s < Allies.get_nr_of_cities() ; s++)																																								
		for (it = Allies.adj_list[s].begin(); it != Allies.adj_list[s].end(); it++)																																							
		{																																							
			Allies.edge_matrix[s][*it].residual = Allies.edge_matrix[s][*it].upper_bound - Allies.edge_matrix[s][*it].flow;																																						
			Allies.edge_matrix[*it][s].residual = Allies.edge_matrix[s][*it].flow;																																						
			Allies.edge_matrix[*it][s].cost_per_ton_r = (-1)* Allies.edge_matrix[s][*it].cost_per_ton;																																						
			Allies.edge_matrix[s][*it].cost_per_ton_r = Allies.edge_matrix[s][*it].cost_per_ton;																																						
		}																																							
																																									
	do																																								
	{ 																																								
		if(floyd_warshall(Allies, i, k) && augment_cycle_F_W(cycle, Allies, i, k))																																							
			x=1;																																						
		else																																							
			x=0;																																						
	}																																								
	while(x==1);																																								
																																									
	for (int i = 0; i < (int) Allies.get_nr_of_cities() ; i++)																																								
		for (int j = 0; j < (int) Allies.get_nr_of_cities() ; j++)																																							
			total_flow += Allies.edge_matrix[i][j].cost_per_ton*Allies.edge_matrix[i][j].flow;																																						
																																									
}																																									
																																									
bool algorithms::augment_cycle_F_W(vector <int> & cycle, input & Allies, int &l, int &k)																																									
{																																									
		vector<int> part1, part2;																																							
		part1.push_back(k);																																							
		int x = Allies.edge_matrix[l][k].pred_F_W;																																							
																																									
		while (x != l)																																							
		{																																							
			part1.push_back(x);																																						
			x = Allies.edge_matrix[l][x].pred_F_W;																																						
		}																																							
		part1.push_back(x);																																							
																																									
		part2.push_back(l);																																							
		x = Allies.edge_matrix[k][l].pred_F_W;																																							
		while (x != k)																																							
		{																																							
			part2.push_back(x);																																						
			x = Allies.edge_matrix[k][x].pred_F_W;																																						
		}																																							
		part2.push_back(x);																																							
																																									
		for (int i = (int) part1.size() - 1; i >= 0; i--)																																							
			cycle.push_back(part1[i]);																																						
		for (int i = (int) part2.size() - 2; i >= 0; i--)																																							
			cycle.push_back(part2[i]);																																						
																																									
		// Then we check whether we can send any flow along this cycle to improve our solution:																																							
		double current_min = 999999999;																																							
		for (int i = 0; i < (int)cycle.size() - 1; i++)																																							
		{																																							
																																									
			if (current_min > Allies.edge_matrix[cycle[i]][cycle[i+1]].residual)																																						
				current_min = Allies.edge_matrix[cycle[i]][cycle[i+1]].residual;																																					
		}																																							
																																									
		if (current_min == 0)																																							
			return false;																																						
																																									
		for (int i = 0; i < (int)cycle.size() - 1; i++) // update the residual in the following way:																																							
		{																																							
				Allies.edge_matrix[cycle[i]][cycle[i+1]].flow += current_min;																																					
				Allies.edge_matrix[cycle[i+1]][cycle[i]].flow -= current_min;																																					
				Allies.edge_matrix[cycle[i]][cycle[i+1]].residual -= current_min;																																					
				Allies.edge_matrix[cycle[i+1]][cycle[i]].residual += current_min;																																					
		}																																							
																																									
		cycle.clear();																																							
		part1.clear();																																							
		part2.clear();																																							
		return true;																																							
}																																									
																																									
bool algorithms::floyd_warshall(input& Allies, int &i, int &k)																																									
{																																									
	list <int>::iterator it;																																								
	for (int n = 0; n < Allies.get_nr_of_cities() ; n++)																																								
		Allies.edge_matrix[n][n].dist_label_F_W = 0;																																							
																																									
	for (int o = 0; o < Allies.get_nr_of_cities() ; o++)																																								
		for (int m= 0; m < Allies.get_nr_of_cities() ; m++)			// run over all till number of cities																																				
				if(Allies.edge_matrix[o][m].residual > 0)																																					
				{																																					
					Allies.edge_matrix[o][m].dist_label_F_W = Allies.edge_matrix[o][m].cost_per_ton_r; // use cost_per_ton redefine in input!!																																				
					Allies.edge_matrix[o][m].pred_F_W = o;																																				
				}																																					
	for (int p = 0; p < Allies.get_nr_of_cities() ; p++)																																								
		for (int q = 0; q < Allies.get_nr_of_cities() ; q++)																																							
			for (int r = 0; r < Allies.get_nr_of_cities() ; r++)																																						
			{																																						
				if (Allies.edge_matrix[q][r].dist_label_F_W > Allies.edge_matrix[q][p].dist_label_F_W + Allies.edge_matrix[p][r].dist_label_F_W)																																					
				{																																					
					Allies.edge_matrix[q][r].dist_label_F_W = Allies.edge_matrix[q][p].dist_label_F_W + Allies.edge_matrix[p][r].dist_label_F_W;							
					Allies.edge_matrix[q][r].pred_F_W = Allies.edge_matrix[p][r].pred_F_W;							
				}								
												
				if (r == q && Allies.edge_matrix[q][r].dist_label_F_W < 0 )								
				{								
					i = q;							
					k = p;							
					return true;							
				}								
			}									
	return false;											
}												
												
void algorithms::labelling(input & Allies)												
{												
	list <int>::iterator it;			
	total_flow = 0; 
	Allies.restart();
	restart(Allies);							
	list <int> LIST;	
	labels.resize(Allies.get_nr_of_cities());
	labels[12].value = 1;											
	start_residual (Allies);											
	while (labels[12].value == 1)											
	{											
		for (int i = 0; i < Allies.get_nr_of_cities() ; i++)										
		{										
			labels[i].value = 0;									
			labels[i].pred = 0;									
		}										
												
		labels[0].value = 1;										
												
		LIST.push_back(0);										
		while (!LIST.empty() && labels[12].value == 0)										
		{										
			int x = LIST.back();									
			LIST.pop_back();									
			list <int>::iterator it;									
												
				for (it=Allies.adj_list[x].begin(); it != Allies.adj_list[x].end(); it++)								
					if (labels[*it].value==0)							
					{							
						if (Allies.edge_matrix[x][*it].residual!=0)						
						{						
							labels[*it].value=1;					
							labels[*it].pred=x;					
							LIST.push_back(*it);					
						}						
					}							
												
		}										
	augment(Allies);											
	}	
	for (it = Allies.adj_list[0].begin(); it != Allies.adj_list[0].end(); it++)
		total_flow += Allies.edge_matrix[0][*it].flow;
}												
												
void algorithms::start_residual(input & allies) // in input all residuals are set to 1 million												
{												
	list <int>::iterator it;											
	for (int i = 0; i < allies.get_nr_of_cities() ; i++)											
		for (it=allies.adj_list[i].begin(); it != allies.adj_list[i].end(); it++)										
				allies.edge_matrix[i][*it].residual = allies.edge_matrix[i][*it].upper_bound - allies.edge_matrix[i][*it].flow + allies.edge_matrix[*it][i].flow; // definition residual								
}												
																							
void algorithms::augment(input & allies) // pg. 185 [1]												
{												
	double min = -1;											
	list <int> augmenting_path;											
	int i = 12;											
	augmenting_path.push_back(i);											
	while (i != 0)											
	{											
		if (allies.edge_matrix[labels[i].pred][i].residual < min || min == -1)										
			min = allies.edge_matrix[labels[i].pred][i].residual;									
		i=labels[i].pred;										
		augmenting_path.push_back(i);										
	}											
												
	list <int>::iterator it;											
	for (it=augmenting_path.begin(); it != augmenting_path.end(); it++)	
	{											
		allies.edge_matrix[labels[*it].pred][*it].residual -= min; // update residual										
		allies.edge_matrix[*it][labels[*it].pred].residual += min;										
		allies.edge_matrix[labels[*it].pred][*it].flow += min; // update flow										
	}											
}												
												
void algorithms::restart(input & allies)												
{	
	labels.resize(allies.get_nr_of_cities());
	for (int i = 0 ; i < allies.get_nr_of_cities() ; i++)											
	{											
		labels[i].value = 0;										
		labels[i].pred = 0;										
	}											
												
}												
