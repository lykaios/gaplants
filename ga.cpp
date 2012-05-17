#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include <queue>
#include "ga.h"
using namespace std;

/**********************************************************
Name:Ga() 
Parameters:
  plants_in - list of plants to use for simulation
Purpose: 
  Initialize the variables of the genetic algorithm
Returns: 
  Nothing, constructor
**********************************************************/
Ga::Ga(vector<Plant*> plants_in)
{
  pop_size = 100;
  //Artifically increased to increase problem size (timing)
  calendar_days = 40000;
  cur_gen = 0;
  elite_pct = .2;
  mutate_pct = .02;
  init_calendar(); 
  plant_list = plants_in;
  init_chromos();
}

/**********************************************************
Name: init_calendar 
Parameters: 
  none
Purpose: 
  Fill our calendar with integer values, representing amount of rain
Returns: 
  Nothing, but updates the class calendar 
**********************************************************/
void Ga::init_calendar()
{
  for(int i = 0; i < calendar_days; i++)
	calendar.push_back(rand() % 4); 
}

/**********************************************************
Name: init_chromos 
Parameters: 
  none
Purpose: 
  Fill each member of population with randomly selected plant growing days.
Returns: 
  Nothing, but updates our chromosome population 
**********************************************************/
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

/**********************************************************
Name: print 
Parameters: 
  none 
Purpose: 
  Print out current class info
Returns: 
  nothing 
**********************************************************/
void Ga::print()
{
  int row_size = 20;
  for(int gen_cnt = 0; gen_cnt <= cur_gen; gen_cnt++)
  { 
    cout << "==========Generation: " << gen_cnt << "==============" << endl;
    //If on first print, show our constant calendar/plant list
    if(gen_cnt == 0)
    {
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
    }
    cout << "=====Chromosones=====\n";
    for(int i = 0; i < pop_size ; i++)
    {
      cout << "Member" << i << "\n---------------";
      print_chromosome(chromos[gen_cnt * pop_size + i], row_size);
    }
  };
}

/**********************************************************
Name: print_fitness() 
Parameters: 
  none
Purpose: 
  Print out fitness for each generation
Returns: 
  nothing 
**********************************************************/
void Ga::print_fitness()
{
  int tot_gen_fit = 0;
  int max_fit = 0;
  int row_size = 30;
  for(int i = 0; i < fitness.size(); i++)
  {
    tot_gen_fit += fitness[i];
    if(max_fit < fitness[i])
      max_fit = fitness[i];
    
    if(i % pop_size == 0 && i != 0)
    { 
      cout << "gen" << i / pop_size << "|Total Gen Fit = " << tot_gen_fit << ", Max fit = " << max_fit << endl;
      genmax_fit = max_fit;
      tot_gen_fit = 0;
      max_fit = 0;
    }
    if(i == 0)
      gen0_fit = max_fit;
    /*if(i % row_size == 0 && i != 0)
      cout << endl;
    else
      cout << fitness[i] << "|";
    */
  }
}

/**********************************************************
Name: print_chromosome 
Parameters: 
  chrom_to_print - a single chrmosome, row_size - used purely for print formatting
Purpose: 
  Print a single chromosome 
Returns: 
  nothing 
**********************************************************/
void Ga::print_chromosome(vector<Plant *> chromo_to_print, int row_size)
{
  for(int j = 0; j < chromo_to_print.size(); j++)
  {
    if(j % row_size == 0)
	  cout << endl << "row" << j/row_size << "|";
    cout << chromo_to_print[j]->ret_abrv_name() << "|";
  }
  cout << endl;
}

/**********************************************************
Name: ret_rand_plant 
Parameters: 
  none
Purpose: 
  Fetch a random plant pointer from our list. 
Returns: 
  Plant *  
**********************************************************/
Plant * Ga::ret_rand_plant()
{
  Plant * ret_plant = NULL;
  ret_plant = plant_list[rand() % plant_list.size()];
  return ret_plant;
}

/**********************************************************
Name: eval_fitness 
Parameters: 
  none
Purpose: 
  Evaluate fitness of population for current generation
Returns: 
  Nothing, but updates our class fitness variable 
**********************************************************/
void Ga::eval_fitness()
{
  int local_fitness = 0;
  //For each chromosone
  for(int i = 0; i < pop_size; i++)
  {
    local_fitness = chrom_fitness(i);
    fitness.push_back(local_fitness);   
  } 
}

/**********************************************************
Name: advance_generation 
Parameters: 
  none
Purpose: Used to advance onto next generation
  :selects the elite members
  :crosses their genes to produce new population
  :runs mutation on each chromosome
Returns: 
  nothing, but updates most class variables
**********************************************************/
void Ga::advance_generation()
{
  vector<Plant * > push_member;
  vector<pair <int, int> > fit_members;
  bool chromo_in = false;
  //Evaluate fitness of current generation
  eval_fitness();
  //Initialize with false values, but doing it to preserve size
  for(int x = 0; x < pop_size * elite_pct; x++)
    fit_members.push_back(make_pair(-1, -1));
  //Obtain elite members 
  for(int j = 0; j < pop_size; j++)
  {
    for(int k = 0; k < fit_members.size(); k++)
    {
      if(fitness[cur_gen * pop_size + j] > fit_members[k].second)
      {
	//Insert chromo into elite list. 
	insert_elite(fit_members,fitness[cur_gen * pop_size + j], j, k);		
	chromo_in = true; 
      }
      //If we have already inserted that member, exit our loop 
      if (chromo_in == true)
	break;
    }
    chromo_in = false;
  }
  
  //Push elite members into new population 
  for(int k = 0; k < fit_members.size(); k++)
  {
    push_member = chromos[cur_gen * pop_size + fit_members[k].first];
    chromos.push_back(push_member);  
  }
  //Breed members
  breed_population(fit_members);
  
  //Advance generation counter
  cur_gen++;
  
  //Mutate remaining 
  for(int h = 0; h < pop_size; h++)
  {  
    push_member = chromos[cur_gen * pop_size + h];
    mutate_chromo(push_member);
    chromos[cur_gen * pop_size + h] = push_member;
  }
}

/**********************************************************
Name: mutate_chromo 
Parameters: 
  chromo - Plant * vector to perform mutation on
Purpose: 
  To loop through each day, and see if that gene should be mutated. 
Returns: 
  Passing back by reference, the updated chromosome. 
**********************************************************/
void Ga::mutate_chromo(vector<Plant *> &chromo)
{
  double rand_select; 
  //For every day, check for mutation
  for(int i = 0; i < chromo.size(); i++)
  {
      rand_select = drand48();
      //If it is within pct chance, randomly select new plant for that day
      if(rand_select < mutate_pct)
	chromo[i] = ret_rand_plant(); 
  }
}
//
/**********************************************************
Name:insert_elite 
Parameters: 
  fit_members - a vector which holds fitness number, and index that chromosome is at in master list
  fitness - fitness number to insert
  member_index - location of that chromosome
  target_index - where we should be inserting
Purpose: Function used to insert into our elite member list. This helps keep track of which members
  are the most elite in a given population 
Returns: by reference, fit_members is updated 
**********************************************************/
void Ga::insert_elite(vector<pair <int,int> > &fit_members, int fitness, int member_index, int target_index)
{
  //Loops through, moving each member to the right
  for(int i = fit_members.size() - 1; i > target_index; i--)
  {
    fit_members[i].first = fit_members[i-1].first;
    fit_members[i].second = fit_members[i-1].second;
  }
  //Then sets the values at target index
  fit_members[target_index].first = member_index;
  fit_members[target_index].second = fitness;
}

/**********************************************************
Name:breed_population 
Parameters:
  fit_members - a vector of pairs which holds the fittest members of a population
Purpose: 
  Produce the next generation of chromosomes via "breeding" (crossover) 
Returns:
  Nothing, but updates the chromosome class variable 
**********************************************************/
void Ga::breed_population(vector<pair <int,int> > fit_members)
{
  //Implement crossover, then mutation
  int members_to_create = pop_size - (pop_size * elite_pct);// - (pop_size * mutation_pct);
  int parent1, parent2;
  double cache = 0.0;
  double fit;
  double tot_gen_fitness = 0;
  vector <double> member_fit_pct;
  vector <Plant*> new_member;
  //Sum the total generations fitness
  for(int i = 0; i < pop_size; i++)
    tot_gen_fitness += fitness[cur_gen * pop_size + i];
  //Calculate the pct of each members fitness
  for(int i = 0; i < pop_size; i++)
  {
    fit = fitness[cur_gen*pop_size+ i];
    cache = fit / tot_gen_fitness;
    member_fit_pct.push_back(cache); 
  }
  
  //For all missing members of population, apply roulette wheel selection
  for(int i = 0; i < members_to_create; i++)
  {
    //Select both parents
    parent1 = roulette_selection(member_fit_pct);
    parent2 = roulette_selection(member_fit_pct);
    //Start with parent Y's genes
    new_member = chromos[cur_gen * pop_size + parent2];   
    //From 0->cross point, parent X's genes
    for(int j = 0; j < get_cross_point(); j++)
      new_member[j] = chromos[pop_size* cur_gen + parent1][j];	
    //Push new member into population 
    chromos.push_back(new_member); 
  } 
  
}

/**********************************************************
Name: get_cross_point 
Parameters: none
Purpose: 
  Get random number to be crossover point 
Returns: 
  random int between 0 and number of calendar days 
**********************************************************/
int Ga::get_cross_point()
{
  return rand() % calendar_days;
}

/**********************************************************
Name: roulette_selection 
Parameters:
  fit_pct - a vector of the current generations chance to be selected
Purpose:
  Apply roulette_selection 
Returns: 
  int value representing the parent 
**********************************************************/
int Ga::roulette_selection(vector<double> fit_pct)
{
  //Select random double
  double rand_select = drand48();
  bool wcontinue = true;
  double cur_sum = 0.0;
  int i;

  //cout << "Generated rand = " << rand_select << endl;
  for(i = 0; i < fit_pct.size() - 1; i++)
  { 
    cur_sum += fit_pct[i];
    //cout << "run" << i << " sum = " << cur_sum << endl;
    if(rand_select > cur_sum && rand_select < (cur_sum + fit_pct[i+1]) )
      break;
  }
  return i; 
}

/**********************************************************
Name: chrom_fitness 
Parameters:
  chrom_index - index in chromosome vector
Purpose:
  Evalute the fitness of a specific chromosome 
Returns:
  Fitness of a given chromosome  
**********************************************************/
int Ga::chrom_fitness(int chrom_index)
{
  int sun_sum, rain_sum;
  int fitness = 0;
  //For each day chopping off last 50 days for simplicity for now
  //TODO: Fix so that it gets lowest grow period from plant list
  for(int i = 0; i < calendar_days - 50; i++)
  {
    sun_sum = 0;
    rain_sum = 0;
    //Loop through grow cycle, sum rain amount and sunny days
    for(int j = i; j < chromos[cur_gen * pop_size + chrom_index][i]->ret_grow_period(); j++)
    {
      if (calendar[j] == 0)
	sun_sum += 1;
      else
	rain_sum += calendar[j];
    } 
    //If the grow period met plants requirements, increment fitness of this chromosone
    if (rain_sum >= chromos[cur_gen * pop_size + chrom_index][i]->ret_rain_amt() && sun_sum >=  chromos[cur_gen * pop_size + chrom_index][i]->ret_sun_days() )
      fitness += 1; 
  }   
  return fitness;
}
