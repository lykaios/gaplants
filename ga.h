using namespace std;
#include "plant.h"
/*
This class is used to store a system of linear equations in the Ax=b fashion. 
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
	
	vector< vector<Plant*> > chromos;
	vector<Plant*> plant_list;
	vector<int> calendar;
	int calendar_days;
	int pop_size;
	int cross_rate;
};
#endif
