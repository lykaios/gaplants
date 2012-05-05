using namespace std;
/*
This class is used to store a system of linear equations in the Ax=b fashion. 
*/
#ifndef PLANT_H
#define PLANT_H
class Plant
{
  public:
	Plant(string name_in, int grow_period, int rain_in, int sun_days);
	void print();
  
  private:
	string name;
	int rain_amount;
	int grow_period;
	int sun_days; 
};
#endif
