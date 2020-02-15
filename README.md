The Weather and Location Library (weatherLocation.h) v1.04 (Current)
is a library designed to get just a few pieces of information 
from Dark Sky dot Net and Google’s Geolocation services.

Other required (installed) libraries are:

**ArduinoJSON 6** can be installed from the Library Manager
**HTTPClient** is here: https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient
**WiFi.h** also in the Library Manager 

The information collected by the library includes:

Latitude - Current 

Longitude - Current

Temperature

UV Index

Humidity Percentage

Wind Speed

Wind Gust

Wind Direction

Conditions

Location City Name

Location State Name

A WiFi connection is required. Use your favorite method in setup().
 
A Dark Sky API Key and a Google API Key are both required and are free for 1000 API calls per day.

**DarkSky API Key:** https://darksky.net/dev

**Google API Key:** https://developers.google.com/maps/documentation/geolocation/get-api-key


Please be aware there is *NO GPS MODULE required* to get location information 
as the routines use nearby WiFi signals to triangulate a location from Google.

Installation and use is fairly straight forward: 

Install as you would any other Arduino Library on github, download and install from zip.

Then create an object such as:

weatherLocation WL; // You can call this whatever you like WL, wetHen, Blue, Peggy

Getting ALL information (weather and location) just call:

WL.getWeather(your Dark Sky API Key, your Google API Key);

You can also get ONLY location information (latitude, longitude, city and state) 
without weather by calling:

WL.getCoordinates(your Google API key);

Once called the variables available will be:

**LOCATION VARS**

WL.latitude 

WL.longitude

WL.city

WL.state

**WEATHER VARS**


WL.temperature

WL.uvIndex

WL.humidity

WL.windSpeed

WL.windGust

WL.windDirection

WL.conditions


The API calls are already created in the library, all you need to do is plug in 
your keys to the function calls and use the variables. 

Again, the library uses **HTTPClient and ArduinoJSON 6** 
so have those available if they don’t install automatically with this library.

I hope you find good use for this library, and that you make something fun!

Good coding, my friends!



weatherLocation Library is created, updated, and continually stared at by Jessica Hershey 
currently residing on The Jersey Shore. She’s never met anyone in the cast of the TV show 
by the same name. She assumes that’s a good thing. 

Anyway, this is her first offering to the Arduino world, and hopes it helps.

Feb2020

