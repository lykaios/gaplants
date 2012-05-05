#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <omp.h>
#include <vector>
#include <cfloat>
#include "plant.h"
using namespace std;
/***********************************************
Nate Allen
CSCI 551
Final Assignment: Parallel Implementation of Genetic Algorithm
***********************************************/
//Used to hold information about plants. 
struct plant{
	string name;
	int rain_amount;
	int grow_period;
	int sun_days; 
};

void init_calendar(vector<int> &in_vect);
void print_vect(vector<int> in_vect);

int main(int argc, char *argv[])
{
  
  srand(time(NULL));
  //Expects number of threads, run number, name of logfile, name of plant list to be passed.
  if(argc < 4)
  {
	cout << "Incorrect number of arguments" << endl;
	return 1;
  }
  //Declare file stream to store runtimes
  ofstream fout;
  ifstream plant_file;
  fout.open(argv[3], ios::app);    
  plant_file.open(argv[4]);
  string in_name;
  int in_rain,in_sun,in_grow;
  /* 
  int thread_num = atoi(argv[1]);
  time_t begin, end; 
  //Set and print OpenMP information
  omp_set_num_threads(thread_num); 
  fout << "=====Run (" << argv[3] <<")=====\n";
  fout << ":Number of cores = " << omp_get_num_procs() << endl;
  fout << ":Thread number = " << omp_get_num_threads() << endl;
  fout << ":Max thread number = " << omp_get_max_threads() << endl;
  time(&begin);
  */
  vector<Plant*> plant_list;
  while(plant_file) 
  {
	plant_file >> in_name;
	plant_file >> in_grow;
	plant_file >> in_rain;
	plant_file >> in_sun;
	plant_list.push_back(new Plant(in_name, in_grow, in_rain, in_sun));
  }
  plant_list[0]->print(); 
  plant_list[1]->print(); 
  //Initialize our calendar
  //represent as vector of ints
  vector<int> calendar;
  init_calendar(calendar);
  //print_vect(calendar);

  
  //Print time and call verification step
  //fout << "Time elapsed: " << difftime(end, begin) << " seconds where n = " << msize;
  //Close file
  fout.close();
  
  return 0;
}

void init_calendar(vector<int> &in_vect)
{
  for(int i = 0; i < 360; i++)
	in_vect.push_back(rand() % 10); 
}

void print_vect(vector<int> in_vect)
{
	//cout << "Printing " << v_to_print << " size " << n << " vector\n";
	for(int i = 0; i < in_vect.size(); i++)
	{  
	  if(i % 10 == 0)
		cout << endl << "row" << i/10 << "|";
	  cout << in_vect[i] << "|";
	}
}

