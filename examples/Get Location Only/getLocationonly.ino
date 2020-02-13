// The first two includes are required
// The TFT include is for the ESP32 use whatever you need
// This is just an example to show you how to get the vars
// You need to provide the keys and other defines as indicated for this to work

#include <weatherLocation.h>
#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>


weatherLocation WL;

#define HOMESSID "Your SSID"
#define HOMEPW "Your PW"
#define DARKSKYKEY "Your DS Key"
#define GOOGLEKEY "Your Google Key"



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  
  // Use whatever you use to connect to WiFi
  
  WiFi.begin(HOMESSID,HOMEPW);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to connect");
    delay(1000);
  } 
  Serial.print("Connected to ");
  Serial.println(HOMESSID);

  

WL.getCoordinates(GOOGLEKEY); //<-------------------- Gets ONLY LOCATION variables, no weather

// Returns number of wifi networks found before printing these variables
// In case you are wondering what that rogue number means

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
