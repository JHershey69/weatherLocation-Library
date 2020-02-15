/*
   weatherLocation parsing library
   Created by Jessica Hershey, February 11, 2020
   Viva La Resitance
*/


#ifndef pl
#define pl

#include "Arduino.h"


class weatherLocation
{
  public:
    //Constructor
    weatherLocation();

    //Methods
    void getWeather(String userKey, String userGKey);
    void getCoordinates(String googleKey);
    void getLocName(String myGkey);

    //Variables
    float temperature;
    int uvIndex;         
    float humidity;         
    float windSpeed;
    float windGust;
    int windBearing;
    

    float latitude;
    float longitude;

    char city[45];        // If a city name has more than 50 characters God help us all (maybe Truth or Consequences, NM)
    char state[3];        // 2 char state code in the USA
    char conditions[20];
  private:
    const char* icon;
};

#endif

