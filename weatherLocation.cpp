/*
weatherLocation library created by Jessica Hershey February 2020
Use it in good health
Viva La Resistance
*/


#include "Arduino.h"
#include "weatherLocation.h"
#include <HTTPClient.h>		// Please see the readme file 
#include <ArduinoJson.h>	// ArduinoJSON 6 please

weatherLocation::weatherLocation() {

}

// DON’T CHANGE THESE PLEASE


#define DARKSKYWEB "https://api.darksky.net/forecast/"
#define DARKSKYEXCLUDES "exclude=minutely,hourly,daily,flags"

#define GEOLOCATIONURL "https://www.googleapis.com/geolocation/v1/geolocate"
#define GEO_REVERSE_URL "https://maps.googleapis.com/maps/api/geocode/json?latlng="
#define GEO_REVERSE_END "&location_type=ROOFTOP&key="

float latitude;
float longitude;

float temperature;
int uvIndex;          
float humidity;         // Multiply Dark Sky humidity float by 100 for display
float windSpeed;
float windGust;
int windBearing;
const char* icon;

char city[46];        // If a city name has more than 50 characters God help us all (maybe Truth or Consequences, NM)
char state[3];        // 2 char state code


const char* jsonSend;

//============================================================================
// Dark Sky... Requires both a Dark Sky Key and a Google Key
//============================================================================

void weatherLocation::getWeather(String userKey, String userGKey) {

  
  weatherLocation::getCoordinates(userGKey);
  delay(1000);
  

  HTTPClient http;

  String darkSkyURL = DARKSKYWEB;
  darkSkyURL.concat(userKey);
  darkSkyURL.concat("/");
  darkSkyURL.concat(String(latitude, 4));
  darkSkyURL.concat(",");
  darkSkyURL.concat(String(longitude, 4));
  darkSkyURL.concat("/");
  darkSkyURL.concat("?");
  darkSkyURL.concat(DARKSKYEXCLUDES);
  http.begin(darkSkyURL);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Connection to Dark Sky successful!");

	// ONLY CHANGE STUFF IF YOU KNOW WHAT YOU ARE DOING


    const size_t capacity = JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(19) + 340;
    DynamicJsonDocument doc(capacity);
    
    deserializeJson(doc, http.getString());

    float latitude = doc["latitude"];
    float longitude = doc["longitude"];
    const char* timezone = doc["timezone"];

    JsonObject currently = doc["currently"];
    icon = currently["icon"];
    temperature = currently["temperature"];
    humidity = currently["humidity"];
    windSpeed = currently["windSpeed"];
    windGust = currently["windGust"];
    windBearing = currently["windBearing"];
    uvIndex = currently["uvIndex"]; // 0

    int offset = doc["offset"]; // -8

   } else {
    Serial.println("Unable to connect to Dark Sky");
  }


  http.end();

}



//============================================================================
// Google Geolocation from MAC Addresses (No GPS)
//============================================================================


void weatherLocation::getCoordinates(String googleKey) {

  HTTPClient http;

  String jsonSend;    // This will be the json we send to Google with nearby MAC ADDRESSES

  // Scan for nearby WiFi networks

  int numNetworks = WiFi.scanNetworks();
  Serial.print("Found ");
  Serial.print(numNetworks);
  Serial.println(" WiFi Networks nearby");  // Hello World check

  // Add 3 nearest WiFi MAC Addresses in this JSON form:
  /*
    {
    "wifiAccessPoints": [
    "macAddress":"4A:5D:36:35:BA:6D",
    "macAddress":"48:5D:36:35:BA:6C",
    "macAddress":"B0:B9:8A:EF:D2:D8"
    ]
    }
  */

  // Creating the json with some string operations

  jsonSend = "{\n";
  jsonSend += "\"wifiAccessPoints\": [\n";
  for (int j = 0; j < numNetworks; ++j)
  {
    jsonSend += "{\n";
    jsonSend += "\"macAddress\" : \"";
    jsonSend += (WiFi.BSSIDstr(j));
    jsonSend += "\",\n";
    jsonSend += "\"signalStrength\": ";
    jsonSend += WiFi.RSSI(j);
    jsonSend += "\n";
    if (j < numNetworks - 1)
    {
      jsonSend += "},\n";
    }
    else
    {
      jsonSend += "}\n";
    }
  }
  jsonSend += ("]\n");
  jsonSend += ("}\n");


  // Setting up the url for the POST

  String geoLocURL = GEOLOCATIONURL;
  geoLocURL.concat("?key=");
  geoLocURL.concat(googleKey);

  // Lets send the json in the POST and get back an answer with getString(),
  // add header for application/json

  http.begin(geoLocURL);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonSend);


  // Setting the parsing routine

  const size_t capacity = 2 * JSON_OBJECT_SIZE(2) + 30;
  DynamicJsonDocument geo(capacity);


  deserializeJson(geo, http.getString());     // parsing the return from Google into geo Document

  // Variables in parse

  latitude = geo["location"]["lat"]; 
  longitude = geo["location"]["lng"];

  int accuracy = geo["accuracy"]; // 37


  http.end();

  weatherLocation::getLocName(googleKey);	// ←---------Must call this to get city and state uses lat and lon from above
					// Calls getCoordinates() from getLocName()

}

//==================================================================================
// End Geolocation
//==================================================================================


//==================================================================================
// Get Location Name from latitude and longitude
//==================================================================================

void weatherLocation::getLocName(String myGkey) {

  HTTPClient http;

 

  // Set up the URL this is only lat long, location type ROOFTOP and key
  // and returns a lot of info as you will see in the parsing portion

  String URL = GEO_REVERSE_URL;
  URL.concat(String(latitude, 6));
  URL.concat(",");
  URL.concat(String(longitude, 6));
  URL.concat(GEO_REVERSE_END);
  URL.concat(myGkey);



  http.begin(URL);
  int httpCode = http.GET();

  if (httpCode > 0) {

    const size_t capacity = 8 * JSON_ARRAY_SIZE(1) + 9 * JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(4) + JSON_ARRAY_SIZE(7) + JSON_ARRAY_SIZE(8) + 11 * JSON_OBJECT_SIZE(2) + 18 * JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(6) + 1920;

    DynamicJsonDocument locName(capacity);


    deserializeJson(locName, http.getString());   //<----------------- This is the line that streams the json for parsing

    const char* plus_code_compound_code = locName["plus_code"]["compound_code"]; // "2QRG+52 Toms River, NJ, USA"
    const char* plus_code_global_code = locName["plus_code"]["global_code"]; // "87G72QRG+52"

    JsonObject results_0 = locName["results"][0];

    JsonArray results_0_address_components = results_0["address_components"];

    
    JsonObject results_0_address_components_2 = results_0_address_components[2];
    strcpy(city,results_0_address_components_2["short_name"]); // "Toms River"

   
    JsonObject results_1 = locName["results"][1];

    JsonArray results_1_address_components = results_1["address_components"];
    
    JsonObject results_1_address_components_4 = results_1_address_components[4];
    strcpy(state,results_1_address_components_4["short_name"]); // "NJ"

    const char* status = locName["status"]; // "OK"

    
  }

  http.end();

}

//===================================================================================
// End Location Name from Lat Lon
//===================================================================================
