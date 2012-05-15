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

Ga::Ga(vector<Plant*> plants_in)
{
  pop_size = 30;
  //TODO: Only for dev, increase day size for real tests
  //12 months, 30 days
  calendar_days = 360;
  cur_gen = 0;
  cross_rate = .2;
  elite_pct = .2;
  //mutation_pct = .1;
  init_calendar(); 
  plant_list = plants_in;
  init_chromos();
  init_randoms();
}
//Fill our calendar with integer values, representing amount of rain
void Ga::init_calendar()
{
  for(int i = 0; i < calendar_days; i++)
	calendar.push_back(rand() % 4); 
}
//Fill each member of population with randomly selected plant growing days.
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
//Reinit pseudo random values because time-based won't work for rapid calls due to caching
void Ga::init_randoms()
{
  int i1,i2;
  i1 = rand() % 112312;
  i2 = rand() % 912312;
  srand48(i1 % i2);
}
//Print out current class info
void Ga::print()
{
  int row_size = 20;
  for(int gen_cnt = 0; gen_cnt <= cur_gen; gen_cnt++)
  { 
    cout << "Generation: " << gen_cnt << endl;
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
//Print out fitness for each generation
void Ga::print_fitness()
{
  int tot_gen_fit = 0;
  int max_fit = 0;
  //cout << "DEBUG: fitness.size = " << fitness.size() << endl;
  for(int i = 0; i < fitness.size(); i++)
  {
    tot_gen_fit += fitness[i];
    if(max_fit < fitness[i])
      max_fit = fitness[i];
    if(i % pop_size == 0)
    {  
      cout << " Total Gen Fit = " << tot_gen_fit << ", Max fit = " << max_fit << endl << "gen" << i / pop_size << "|";
      tot_gen_fit = 0;
      max_fit = 0;
    }
    cout << fitness[i] << "|";
  }
}

//Print a single chromosome
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

//Fetch a random plant pointer from our list. 
Plant * Ga::ret_rand_plant()
{
  Plant * ret_plant = NULL;
  int tmp = plant_list.size();
  ret_plant = plant_list[rand() % tmp];
  return ret_plant;
}

//Evaluate fitness of population for current generation 
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

//Used to advance onto next generation
//:selects the elite members
//:crosses their genes to produce new population
//:introduces new members to keep new blood (or should this be mutations?)
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
  
  //Print out elite members
  /*cout << "gen " << cur_gen << " elite chromos" << endl;
  for(int k = 0; k < fit_members.size(); k++)
    cout << "chromo" << fit_members[k].first << "|fit=" << fit_members[k].second << endl;
  */
  //Push elite members into new population 
  for(int k = 0; k < fit_members.size(); k++)
  {
    //cout << "DEBUG: Pushing elite member " << k << "(" << cur_gen << "*" << pop_size << "+" << fit_members[k].first << ")\n";  
    //cout << " : of size " << chromos[cur_gen * pop_size + fit_members[k].first].size() << endl;
    push_member = chromos[cur_gen * pop_size + fit_members[k].first];
    chromos.push_back(push_member);  
  }
  //Breed members
  breed_population(fit_members);
  
  //Mutate remaining?

  //Advance generation counter
  cur_gen++;
}

//Function used to insert into our elite member list
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
    //TODO: Make this one statement
    fit = fitness[cur_gen*pop_size+ i];
    cache = fit / tot_gen_fitness;
    member_fit_pct.push_back(cache); 
  }
  //Print out
  //for(int i = 0; i < member_fit_pct.size(); i++)
  //   cout << "fit pct @" << i << " = " << member_fit_pct[i] << endl;
  //cout << "DEBUG: members needed to breed = " << members_to_create << endl;  
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

//Get random number to be crossover point
int Ga::get_cross_point()
{
  init_randoms();
  return rand() % calendar_days;
}

//Apply roulette_selection
int Ga::roulette_selection(vector<double> fit_pct)
{
  init_randoms();
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
