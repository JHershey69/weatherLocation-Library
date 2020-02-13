/*
weatherLocation library created by Jessica Hershey February 2020
Use it in good health
Viva La Resistance
*/


#include "Arduino.h"
#include "weatherLocation.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>	// ArduinoJSON 6 please

weatherLocation::weatherLocation() {

}

// Put the parsing routines here
// DON’T CHANGE THESE PLEASE


#define DARKSKYWEB "https://api.darksky.net/forecast/"
#define DARKSKYEXCLUDES "exclude=minutely,hourly,daily,flags"

#define GEOLOCATIONURL "https://www.googleapis.com/geolocation/v1/geolocate"
#define GEO_REVERSE_URL "https://maps.googleapis.com/maps/api/geocode/json?latlng="
#define GEO_REVERSE_END "&location_type=ROOFTOP&key="

float latitude;
float longitude;

float temperature;
int uvIndex;          // Color coded for display
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

  HTTPClient http;

  weatherLocation::geoCoordinates(userGKey);	// ←----- Must call this to get lat and lon for dark sky call

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


void weatherLocation::geoCoordinates(String googleKey) {

  HTTPClient http;

  String jsonSend;    // This will be the json we send to Google with nearby MAC ADDRESSES

  // Scan for nearby WiFi networks

  int numNetworks = WiFi.scanNetworks();
  Serial.println(numNetworks);                // Hello World check

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

}

//==================================================================================
// End Geolocation
//==================================================================================


//==================================================================================
// Get Location Name from latitude and longitude
//==================================================================================

void weatherLocation::getLocName(String myGkey) {

  HTTPClient http;

  weatherLocation::geoCoordinates(myGkey);	// ←----- Must call this to get lat and lon

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

    JsonObject results_0_address_components_0 = results_0_address_components[0];
    const char* results_0_address_components_0_long_name = results_0_address_components_0["long_name"]; // "23"
    const char* results_0_address_components_0_short_name = results_0_address_components_0["short_name"]; // "23"

    const char* results_0_address_components_0_types_0 = results_0_address_components_0["types"][0]; // "street_number"

    JsonObject results_0_address_components_1 = results_0_address_components[1];
    const char* results_0_address_components_1_long_name = results_0_address_components_1["long_name"]; // "Maple Street"
    const char* results_0_address_components_1_short_name = results_0_address_components_1["short_name"]; // "Maple St"

    const char* results_0_address_components_1_types_0 = results_0_address_components_1["types"][0]; // "route"

    JsonObject results_0_address_components_2 = results_0_address_components[2];
    const char* results_0_address_components_2_long_name = results_0_address_components_2["long_name"]; // "Toms River"
    const char* results_0_address_components_2_short_name = results_0_address_components_2["short_name"]; // "Toms River"

    const char* results_0_address_components_2_types_0 = results_0_address_components_2["types"][0]; // "locality"
    const char* results_0_address_components_2_types_1 = results_0_address_components_2["types"][1]; // "political"

    JsonObject results_0_address_components_3 = results_0_address_components[3];
    const char* results_0_address_components_3_long_name = results_0_address_components_3["long_name"]; // "Ocean County"
    const char* results_0_address_components_3_short_name = results_0_address_components_3["short_name"]; // "Ocean County"

    const char* results_0_address_components_3_types_0 = results_0_address_components_3["types"][0]; // "administrative_area_level_2"
    const char* results_0_address_components_3_types_1 = results_0_address_components_3["types"][1]; // "political"

    JsonObject results_0_address_components_4 = results_0_address_components[4];
    const char* results_0_address_components_4_long_name = results_0_address_components_4["long_name"]; // "New Jersey"
    const char* results_0_address_components_4_short_name = results_0_address_components_4["short_name"]; // "NJ"

    const char* results_0_address_components_4_types_0 = results_0_address_components_4["types"][0]; // "administrative_area_level_1"
    const char* results_0_address_components_4_types_1 = results_0_address_components_4["types"][1]; // "political"

    JsonObject results_0_address_components_5 = results_0_address_components[5];
    const char* results_0_address_components_5_long_name = results_0_address_components_5["long_name"]; // "United States"
    const char* results_0_address_components_5_short_name = results_0_address_components_5["short_name"]; // "US"

    const char* results_0_address_components_5_types_0 = results_0_address_components_5["types"][0]; // "country"
    const char* results_0_address_components_5_types_1 = results_0_address_components_5["types"][1]; // "political"

    JsonObject results_0_address_components_6 = results_0_address_components[6];
    const char* results_0_address_components_6_long_name = results_0_address_components_6["long_name"]; // "08753"
    const char* results_0_address_components_6_short_name = results_0_address_components_6["short_name"]; // "08753"

    const char* results_0_address_components_6_types_0 = results_0_address_components_6["types"][0]; // "postal_code"

    const char* results_0_formatted_address = results_0["formatted_address"]; // "23 Maple St, Toms River, NJ 08753, USA"

    JsonObject results_0_geometry = results_0["geometry"];

    float results_0_geometry_location_lat = results_0_geometry["location"]["lat"]; // 40.0412299
    float results_0_geometry_location_lng = results_0_geometry["location"]["lng"]; // -74.2255086

    const char* results_0_geometry_location_type = results_0_geometry["location_type"]; // "ROOFTOP"

    float results_0_geometry_viewport_northeast_lat = results_0_geometry["viewport"]["northeast"]["lat"]; // 40.04257888029149
    float results_0_geometry_viewport_northeast_lng = results_0_geometry["viewport"]["northeast"]["lng"]; // -74.22415961970849

    float results_0_geometry_viewport_southwest_lat = results_0_geometry["viewport"]["southwest"]["lat"]; // 40.03988091970849
    float results_0_geometry_viewport_southwest_lng = results_0_geometry["viewport"]["southwest"]["lng"]; // -74.22685758029151

    const char* results_0_place_id = results_0["place_id"]; // "ChIJK3GdtgV4wYkRAl0Q2RB7yPE"

    const char* results_0_plus_code_compound_code = results_0["plus_code"]["compound_code"]; // "2QRF+FQ Toms River, New Jersey, United States"
    const char* results_0_plus_code_global_code = results_0["plus_code"]["global_code"]; // "87G72QRF+FQ"

    const char* results_0_types_0 = results_0["types"][0]; // "street_address"

    JsonObject results_1 = locName["results"][1];

    JsonArray results_1_address_components = results_1["address_components"];

    JsonObject results_1_address_components_0 = results_1_address_components[0];
    const char* results_1_address_components_0_long_name = results_1_address_components_0["long_name"]; // "2250"
    const char* results_1_address_components_0_short_name = results_1_address_components_0["short_name"]; // "2250"

    const char* results_1_address_components_0_types_0 = results_1_address_components_0["types"][0]; // "street_number"

    JsonObject results_1_address_components_1 = results_1_address_components[1];
    const char* results_1_address_components_1_long_name = results_1_address_components_1["long_name"]; // "U.S. 9"
    const char* results_1_address_components_1_short_name = results_1_address_components_1["short_name"]; // "U.S. 9"

    const char* results_1_address_components_1_types_0 = results_1_address_components_1["types"][0]; // "route"

    JsonObject results_1_address_components_2 = results_1_address_components[2];
    const char* results_1_address_components_2_long_name = results_1_address_components_2["long_name"]; // "Toms River"
    const char* results_1_address_components_2_short_name = results_1_address_components_2["short_name"]; // "Toms River"

    const char* results_1_address_components_2_types_0 = results_1_address_components_2["types"][0]; // "locality"
    const char* results_1_address_components_2_types_1 = results_1_address_components_2["types"][1]; // "political"

    JsonObject results_1_address_components_3 = results_1_address_components[3];
    const char* results_1_address_components_3_long_name = results_1_address_components_3["long_name"]; // "Ocean County"
    const char* results_1_address_components_3_short_name = results_1_address_components_3["short_name"]; // "Ocean County"

    const char* results_1_address_components_3_types_0 = results_1_address_components_3["types"][0]; // "administrative_area_level_2"
    const char* results_1_address_components_3_types_1 = results_1_address_components_3["types"][1]; // "political"

    JsonObject results_1_address_components_4 = results_1_address_components[4];
    const char* results_1_address_components_4_long_name = results_1_address_components_4["long_name"]; // "New Jersey"
    const char* results_1_address_components_4_short_name = results_1_address_components_4["short_name"]; // "NJ"

    const char* results_1_address_components_4_types_0 = results_1_address_components_4["types"][0]; // "administrative_area_level_1"
    const char* results_1_address_components_4_types_1 = results_1_address_components_4["types"][1]; // "political"

    JsonObject results_1_address_components_5 = results_1_address_components[5];
    const char* results_1_address_components_5_long_name = results_1_address_components_5["long_name"]; // "United States"
    const char* results_1_address_components_5_short_name = results_1_address_components_5["short_name"]; // "US"

    const char* results_1_address_components_5_types_0 = results_1_address_components_5["types"][0]; // "country"
    const char* results_1_address_components_5_types_1 = results_1_address_components_5["types"][1]; // "political"

    JsonObject results_1_address_components_6 = results_1_address_components[6];
    const char* results_1_address_components_6_long_name = results_1_address_components_6["long_name"]; // "08755"
    const char* results_1_address_components_6_short_name = results_1_address_components_6["short_name"]; // "08755"

    const char* results_1_address_components_6_types_0 = results_1_address_components_6["types"][0]; // "postal_code"

    JsonObject results_1_address_components_7 = results_1_address_components[7];
    const char* results_1_address_components_7_long_name = results_1_address_components_7["long_name"]; // "1248"
    const char* results_1_address_components_7_short_name = results_1_address_components_7["short_name"]; // "1248"

    const char* results_1_address_components_7_types_0 = results_1_address_components_7["types"][0]; // "postal_code_suffix"

    const char* results_1_formatted_address = results_1["formatted_address"]; // "2250 U.S. 9, Toms River, NJ 08755, USA"

    JsonObject results_1_geometry = results_1["geometry"];

    float results_1_geometry_location_lat = results_1_geometry["location"]["lat"]; // 40.0393536
    float results_1_geometry_location_lng = results_1_geometry["location"]["lng"]; // -74.22755029999999

    const char* results_1_geometry_location_type = results_1_geometry["location_type"]; // "ROOFTOP"

    float results_1_geometry_viewport_northeast_lat = results_1_geometry["viewport"]["northeast"]["lat"]; // 40.0407025802915
    float results_1_geometry_viewport_northeast_lng = results_1_geometry["viewport"]["northeast"]["lng"]; // -74.22620131970848

    float results_1_geometry_viewport_southwest_lat = results_1_geometry["viewport"]["southwest"]["lat"]; // 40.0380046197085
    float results_1_geometry_viewport_southwest_lng = results_1_geometry["viewport"]["southwest"]["lng"]; // -74.2288992802915

    const char* results_1_place_id = results_1["place_id"]; // "ChIJ90f-VgF4wYkR9KNr7pTkTCM"

    const char* results_1_plus_code_compound_code = results_1["plus_code"]["compound_code"]; // "2QQC+PX Toms River, New Jersey, United States"
    const char* results_1_plus_code_global_code = results_1["plus_code"]["global_code"]; // "87G72QQC+PX"

    JsonArray results_1_types = results_1["types"];
    const char* results_1_types_0 = results_1_types[0]; // "establishment"
    const char* results_1_types_1 = results_1_types[1]; // "lodging"
    const char* results_1_types_2 = results_1_types[2]; // "point_of_interest"
    const char* results_1_types_3 = results_1_types[3]; // "rv_park"

    const char* status = locName["status"]; // "OK"

    strcpy(city, results_0_address_components_2_short_name);
    strcpy(state, results_1_address_components_4_short_name);

  }

  http.end();

}

//===================================================================================
// End Location Name from Lat Lon
//===================================================================================
