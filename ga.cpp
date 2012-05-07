#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include "ga.h"
using namespace std;

Ga::Ga(vector<Plant*> plants_in)
{
  pop_size = 4;
  //12 months, 30 days
  calendar_days = 360;
  init_calendar(); 
  plant_list = plants_in;
  init_chromos();
}

void Ga::init_calendar()
{
  for(int i = 0; i < calendar_days; i++)
	calendar.push_back(rand() % 10); 
}
void Ga::init_chromos()
{
  int plant_indx = 0;
  vector <Plant*> plant_builder;
  for(int i = 0; i < pop_size; i++)
  {
	plant_builder.clear();	
	plant_indx = i;
	for(int j = 0; j < calendar_days; j++)
	{
	  plant_builder.push_back(ret_rand_plant());
	}
	chromos.push_back(plant_builder);	
  }
}

void Ga::print()
{
  int row_size = 20;
  cout << "====Calendar====";
  for(int i = 0; i < calendar.size(); i++)
  {  
	if(i % row_size == 0)
	  cout << endl << "row" << i/row_size << "|";
	cout << calendar[i] << "|";
  }
  cout << "\n====Plants====\n";
  for(int i = 0; i < plant_list.size(); i++)
  {
	plant_list[i]->print();
  }
  cout << "=====Chromosones=====\n";
  for(int i = 0; i < pop_size ; i++)
  {
	cout << "Member" << i << "\n---------------";
	for(int j = 0; j < chromos[i].size(); j++)
	{
	  if(j % row_size == 0)
		cout << endl << "row" << j/row_size << "|";
	  cout << chromos[i][j]->ret_abrv_name() << "|";
	}
	cout << endl;
  }

}

Plant * Ga::ret_rand_plant()
{
  Plant * ret_plant = NULL;
  int tmp = plant_list.size();
  ret_plant = plant_list[rand() % tmp];
  return ret_plant;
}
