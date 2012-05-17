#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include "plant.h"
using namespace std;

/**********************************************************
Name:Plant 
Parameters:
  name_in - name of plant
  grow_in - grow time of plant
  rain_in - rain amount needed
  sun_in - sun amount needed
Purpose:
  To create a plant 
Returns:
  Nothing 
**********************************************************/
Plant::Plant(string name_in,int grow_in, int rain_in, int sun_in)
{
  name = name_in;
  rain_amount = rain_in;
  grow_period = grow_in;
  sun_days = sun_in;
}

/**********************************************************
Name:print 
Parameters:
  none
Purpose:
  To print out variables associated with a plant 
Returns:
  Nothing 
**********************************************************/
void Plant::print()
{
  cout << name << endl;
  cout << "-rain: " << rain_amount << endl;
  cout << "-sun: " << sun_days << endl;
  cout << "-grow: " << grow_period << endl;
}

/**********************************************************
Name:ret_abrv_name 
Parameters:
  none
Purpose:
  To return a part of the name, used for printing 
Returns:
  A substring of the plant name 
**********************************************************/
string Plant::ret_abrv_name()
{
  return name.substr(0,4);
}

//Return functions for summation
int Plant::ret_rain_amt() {return rain_amount;};
int Plant::ret_sun_days() {return sun_days;};
int Plant::ret_grow_period(){return grow_period;};  
