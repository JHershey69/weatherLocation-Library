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
  

  WiFi.begin(HOMESSID,HOMEPW);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to connect");
    delay(1000);
  } 
  Serial.print("Connected to ");
  Serial.println(HOMESSID);

  

WL.getWeather(DARKSKYKEY,GOOGLEKEY);
// WL.getCoordinates(GOOGLEKEY); 
// The line above is what you use just for lat, lon, city and state... NO WEATHER

// Returns number of wifi networks found before printing these variables
// In case you are wondering what that rogue number means

Serial.println(WL.latitude,7); // Latitude and Longitude are floats so you can give the decimal range here
Serial.println(WL.longitude,7);
Serial.println(WL.temperature); // int value
Serial.println(WL.windSpeed);   // float value
Serial.println(WL.city);        // char[45]


}

void loop() {
  // put your main code here, to run repeatedly:

}
