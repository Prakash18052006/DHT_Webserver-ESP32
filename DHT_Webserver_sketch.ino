#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "ECB_TEQIP";
const char* password = "";

#define DHTPIN 27    

#define DHTTYPE    DHT22    
DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();
    if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Date Object in Js </title>
    <style>
        .container{
            font-size: 23px;
            background-color: blanchedalmond;
            border: 2px solid gray;
            padding: 34px;
            margin: 4px;
            text-align: center;
        }
    </style>
</head>
<body>
    <div class="container">
           current time is : <span id="time"></span>
    </div>
    <script>
        console.log("This is date and time tutorial");
        let now = new Date();
        console.log(now);

        let dt = new Date(10000); // in mili sec
        console.log(dt);

        // let newDate = new Date("2029-09-30");
        // console.log(newDate);

        // let newDate = new Date(year, month, date, hours, minutes, seconds, miliseconds);
        let newDate = new Date(2005, 5, 18, 6, 15, 15, 15); 
        // constructer that relate values of objects
        console.log(newDate);

        let yr = newDate.getFullYear();
        console.log("The year is ", yr);

        let dat = newDate.getDate();
        console.log("The date is ", dat);

        let month = newDate.getMonth();
        console.log("The month is ", month);

        newDate.setDate(59);
        newDate.setMinutes(235);
        console.log(newDate);
        setInterval(upadateTime,1000);

        function upadateTime(){
            time.innerHTML = new Date();
        }


    </script>
</body>
</html>


)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Start server
  server.begin();
}
void loop(){
}