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
  
  private:
	void init_calendar();
	void init_chromos();	
	Plant *ret_rand_plant();

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
	int cross_rate;
};
#endif
