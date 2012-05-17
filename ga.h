using namespace std;
#include "plant.h"
/*
  This class is used to hold all the variables necessary to initialize, and simulate a genetic algorithm
*/

#ifndef GA_H
#define GA_H
class Ga
{
  public:
	Ga(vector<Plant*> plants_in);
	void print();
	void print_fitness();
	void print_chromosome(vector<Plant *> chromo_to_print, int row_size);
	void eval_fitness();
	void advance_generation(); 
  private:
	void init_calendar();
	void init_chromos();	
	Plant *ret_rand_plant();
	int chrom_fitness(int chrom_index);
	void insert_elite(vector<pair <int,int> > &fit_members, int fitness, int member_index, int target_index);
	void breed_population(vector<pair <int,int> > fit_members);
	int roulette_selection(vector<double> fit_pct);
	void mutate_chromo(vector<Plant *> &chromo);
	int get_cross_point();

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
	//Percentage of population to be "elite"
	double elite_pct; 
	//Mutation rate
	double mutate_pct;	
	//Fitness of each chromosome, each generation
	vector<int> fitness;
	int gen0_fit;
	int genmax_fit;
	//Current generation we are on. This way we can reference prior generations fitness
	int cur_gen;
};
#endif
