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
#include "ga.h"
using namespace std;
/***********************************************
Nate Allen
CSCI 551
Final Assignment: Parallel Implementation of Genetic Algorithm
***********************************************/
int main(int argc, char *argv[])
{
  
  srand(time(NULL));
  srand48(time(NULL));
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
  plant_file.open(argv[2]);
  string in_name;
  int in_rain,in_sun,in_grow;
  vector<Plant*> plant_list;
  time_t begin, end; 
  /* 
  int thread_num = atoi(argv[1]);
  //Set and print OpenMP information
  omp_set_num_threads(thread_num); 
  fout << "=====New Run=====\n";
  fout << ":Number of cores = " << omp_get_num_procs() << endl;
  fout << ":Thread number = " << omp_get_num_threads() << endl;
  fout << ":Max thread number = " << omp_get_max_threads() << endl;
  */
  //Read in our list of plants
  while(plant_file) 
  {
	plant_file >> in_name;
	plant_file >> in_grow;
	plant_file >> in_rain;
	plant_file >> in_sun;
	plant_list.push_back(new Plant(in_name, in_grow, in_rain, in_sun));
  }
  time(&begin);
  Ga * my_ga = new Ga(plant_list); 
  //my_ga->print();
  //Simulate for x generations
  for(int x = 0; x < 150; x++)
  {
    my_ga->advance_generation();
  }
  time(&end);
  //Print out fitness, used to verify fitness increases over time. 
  my_ga->print_fitness();
  //Print time out to file
  fout << "Time elapsed: " << difftime(end, begin) << " seconds \n";
  //Close file
  fout.close();
  
  return 0;
}
