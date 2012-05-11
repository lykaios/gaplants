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
  pop_size = 8;
  //12 months, 30 days
  calendar_days = 360;
  cur_gen = 0;
  init_calendar(); 
  plant_list = plants_in;
  init_chromos();
}
//Fill our calendar with integer values, representing amount of rain
void Ga::init_calendar()
{
  for(int i = 0; i < calendar_days; i++)
	calendar.push_back(rand() % 4); 
}
//Fill each memeber of population with randomly selected plant growing days.
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
//Print out current class info
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
//Fetch a random plant pointer from our list. 
Plant * Ga::ret_rand_plant()
{
  Plant * ret_plant = NULL;
  int tmp = plant_list.size();
  ret_plant = plant_list[rand() % tmp];
  return ret_plant;
}

void Ga::eval_fitness()
{
  int local_fitness = 0;
  //For each chromosone
  for(int i = 0; i < pop_size; i++)
  {
    local_fitness = chrom_fitness(i);
    //cout << "chromo(" << i << ") fitness = " << local_fitness << endl;
    fitness.push_back(local_fitness);   
  } 
}

int Ga::chrom_fitness(int chrom_index)
{
  int sun_sum, rain_sum;
  int fitness = 0;
  //For each day chopping off last 50 days for simplicity for now
  for(int i = 0; i < calendar_days - 50; i++)
  {
    sun_sum = 0;
    rain_sum = 0;
    //Loop through grow cycle, sum rain amount and sunny days
    for(int j = i; j < chromos[chrom_index][i]->ret_grow_period(); j++)
    {
      if (calendar[j] == 0)
	sun_sum += 1;
      else
	rain_sum += calendar[j];
    } 
    //If the grow period met plants requirements, increment fitness of this chromosone
    if (rain_sum >= chromos[chrom_index][i]->ret_rain_amt() && sun_sum >=  chromos[chrom_index][i]->ret_sun_days() )
      fitness += 1; 
  }   
  return fitness;
}
