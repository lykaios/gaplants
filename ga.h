using namespace std;
#include "plant.h"
/*
  This class is used to simulate a genetic algorithm
*/

#ifndef GA_H
#define GA_H
class Ga
{
  public:
	Ga(vector<Plant*> plants_in);
	void print();
	void eval_fitness();
	void advance_generation(); 
  private:
	void init_calendar();
	void init_chromos();	
	Plant *ret_rand_plant();
	int chrom_fitness(int chrom_index);
	//Store "farms" as a vector of plant pointers
	vector< vector<Plant*> > chromos;
	//List of available plants
	vector<Plant*> plant_list;
	//The growing calendar we have to work with
	vector<int> calendar;
	//Number of days in our given calender
	int calendar_days;
	//Size of GA population 
	int pop_size;
	//Crossover rate
	double cross_rate;
	//Percentage of population to be "elite"
	double elite_pct; 
	//Fitness of each chromosome, each generation
	vector<int> fitness;
	//Current generation we are on. This way we can reference prior generations fitness
	int cur_gen;
	class Node
	{
	    public:
	      Node(int index_in, int fit_in) {c_fitness = fit_in; c_index = index_in;};
	    int c_fitness;
	    int c_index;  
	};
};
#endif
