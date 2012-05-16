#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include "plant.h"
using namespace std;

/**********************************************************
Name: 
Parameters:
Purpose: 
Returns: 
**********************************************************/
Plant::Plant(string name_in,int grow_in, int rain_in, int sun_in)
{
  name = name_in;
  rain_amount = rain_in;
  grow_period = grow_in;
  sun_days = sun_in;
}

/**********************************************************
Name: 
Parameters:
Purpose: 
Returns: 
**********************************************************/
void Plant::print()
{
  cout << name << endl;
  cout << "-rain: " << rain_amount << endl;
  cout << "-sun: " << sun_days << endl;
  cout << "-grow: " << grow_period << endl;
}

/**********************************************************
Name: 
Parameters:
Purpose: 
Returns: 
**********************************************************/
string Plant::ret_abrv_name()
{
  return name.substr(0,4);
}
/**********************************************************
Name: 
Parameters:
Purpose: 
Returns: 
**********************************************************/
int Plant::ret_rain_amt() {return rain_amount;};
int Plant::ret_sun_days() {return sun_days;};
int Plant::ret_grow_period(){return grow_period;};  
