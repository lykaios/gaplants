#include <iostream>
#include <fstream>
#include <omp.h>
#include "plant.h"
using namespace std;

Plant::Plant(string name_in,int grow_in, int rain_in, int sun_in)
{
  name = name_in;
  rain_amount = rain_in;
  grow_period = grow_in;
  sun_days = sun_in;
}

void Plant::print()
{
  cout << name << endl;
  cout << "-rain: " << rain_amount << endl;
  cout << "-sun: " << sun_days << endl;
  cout << "-grow: " << grow_period << endl;
}
