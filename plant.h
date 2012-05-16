using namespace std;
/*
This class is used to hold our "plants" for the growing season.
*/
#ifndef PLANT_H
#define PLANT_H
class Plant
{
  public:
	Plant(string name_in, int grow_period, int rain_in, int sun_days);
	void print();
	string ret_abrv_name();
	int ret_rain_amt();
	int ret_sun_days();
	int ret_grow_period();  
  private:
	string name;
	int rain_amount;
	int grow_period;
	int sun_days; 
};
#endif
