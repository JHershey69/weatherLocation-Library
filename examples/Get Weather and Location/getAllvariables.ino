
// This is just an example to show you how to get the vars
// You need to provide the keys and other defines as indicated for this to work
// This example assumes you print to a Serial Monitor if you don't and doing so will
// break something, please change to print destination as required

#include <weatherLocation.h>
#include <WiFi.h>



weatherLocation WL;

#define HOMESSID "Your SSID"
#define HOMEPW "Your PW"
#define DARKSKYKEY "Your DS Key"
#define GOOGLEKEY "Your Google Key"



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  

  WiFi.begin(HOMESSID,HOMEPW);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to connect");
    delay(1000);
  } 
  Serial.print("Connected to ");
  Serial.println(HOMESSID);

  

WL.getWeather(DARKSKYKEY,GOOGLEKEY); //<-------------------- Gets ALL variables


Serial.println("LOCATION VARS");
Serial.print("WL.latitude: ");
Serial.println(WL.latitude,7); // Latitude and Longitude are floats so you can give the decimal range here
Serial.print("WL.longitude: ");
Serial.println(WL.longitude,7);
Serial.print("WL.city: ");
Serial.println(WL.city);        // char[45]
Serial.print("WL.state: ");
Serial.println(WL.state);

Serial.println("WEATHER VARS");
Serial.print("WL.temperature: ");
Serial.println(WL.temperature); // int value
Serial.print("WL.humidity: ");
Serial.println(WL.humidity); // float value
Serial.print("WL.uvIndex: ");
Serial.println(WL.uvIndex); // int value
Serial.print("WL.windSpeed: ");
Serial.println(WL.windSpeed);   // float value
Serial.print("WL.windGust: ");
Serial.println(WL.windGust); // float value
Serial.print("WL.windBearing: ");
Serial.println(WL.windBearing); // int value 0 to 359 degrees



}

void loop() {
  // put your main code here, to run repeatedly:

}
