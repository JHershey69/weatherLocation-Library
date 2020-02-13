The Weather and Location Library (weatherLocation.h) is a library designed to get just a few pieces of information from Dark Sky dot Net and Google’s Geolocation services.

The information collected by the library includes:
Latitude - Current
Longitude - Current
Temperature
UV Index
Humidity Percentage
Wind Speed
Wind Gust
Wind Direction
Location City Name
Location State Name

A WiFi connection is required, (also a Dark Sky API Key and a Google API Key both are free for 1000 API calls per day visit their respective websites to apply for the keys) and this was created using an ESP32 TTGO. Please be aware there is NO GPS MODULE required to get location information as the routines use found WiFi signals to triangulate a location from Google.

Installation and use is fairly straight forward. Install as you would any other Arduino Library and then create an objects such as:

weatherLocation WL;

Getting ALL information (weather and location) just call:

WL.getWeather("your Dark Sky API Key", "your Google API Key");

You can also get ONLY location information (latitude, longitude, city and state) without weather by calling:

WL.getCoordinates("your Google API key");

Once called the variables available will be:

LOCATION VARS

WL.latitude 
WL.latitude 
WL.longitude
WL.city
WL.state

WEATHER VARS

WL.temperature
WL.uvIndex
WL.humidity
WL.windSpeed
WL.windGust
WL.windDirection


Please remember you need the keys. So visit www.darksky.net and https://console.developers.google.com to get your keys and information on free usage.

The API calls are already created in the library, all you need to do is plug in your keys to the function calls and use the variables. The library uses HTTPClient and ArduinoJSON 6 so have those available if they don’t install automatically with this library.

I hope you find good use for this library, and that you make something fun!

Good coding, my friends!



weatherLocation Library is created, updated, and continually stared at by Jessica Hershey currently residing on The Jersey Shore. She’s never met anyone in the cast of the TV show by the same name. She assumes that’s a good thing. Anyway, this is her first offering to the Arduino world, and hopes it helps.

Feb2020

