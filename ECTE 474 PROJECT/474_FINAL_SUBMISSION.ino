#include "WiFi.h"
#include <HTTPClient.h>

#include <DHT.h>

#define LED 18
#define soil_pin 34
#define ldr_pin 36
#define On_Board_LED_PIN  2

const char* ssid = "WIFI_NAME";  //--> Your wifi name
const char* password = "WIFI_PASSWORD"; //--> Your wifi password

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycby3-vTqRDAkjdjqiTCw2de6cA-fdP1JSHjXocqVW_5O7ajZAY8N2HTEaZ9HYraaH40/exec";


//initialize the DHT object
DHT dht(4,DHT11);

bool ideal;

void setup() {
  Serial.begin(9600);
  
  //inputs
  pinMode(soil_pin,INPUT);
  pinMode(ldr_pin,INPUT);
  pinMode(4,INPUT); // for DHT

  //output
  //initialise LED
  pinMode(LED,OUTPUT);

  //start DHT object
  dht.begin();

   //----------------------------------------Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  //---------------------------------------- 

  //----------------------------------------Connect to Wi-Fi (STA).
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  //:::::::::::::::::: The process of connecting ESP32 with WiFi Hotspot / WiFi Router.
  // The process timeout of connecting ESP32 with WiFi Hotspot / WiFi Router is 20 seconds.
  // If within 20 seconds the ESP32 has not been successfully connected to WiFi, the ESP32 will restart.
  // I made this condition because on my ESP32, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(On_Board_LED_PIN, HIGH);
    delay(250);
    digitalWrite(On_Board_LED_PIN, LOW);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }

}
}

void loop() {
  
  //read soil moisture as percentage
  float soil =  (100 - ( (analogRead(soil_pin)/4095.00) * 100 ));
  //read light intensity
  int light = analogRead(ldr_pin);
  //read temperature and humidity
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();


  //set conditions for setting the plant in its ideal condition
  //conditions to set the status as true or false
    if ((light > 600) && (soil >= 30) && (temp >22) && (temp < 27) && (humid>=65) && (humid <80)){
      ideal = true;
      
    }
    else {
      ideal = false;
      
    }
    
    //turn on the led if the brightness of the ldr is too low
    if (light <= 600){
      digitalWrite(LED,HIGH);
    }
    else if (light > 600){
      digitalWrite(LED,LOW);
    }
  if (WiFi.status() == WL_CONNECTED) {
    

    // Create a URL for sending or writing data to Google Sheets.
    String Send_Data_URL = Web_App_URL + "?sts=write";
    //Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&temp=" + String(temp);
    Send_Data_URL += "&humid=" + String(humid);
    Send_Data_URL += "&moist=" + String(soil);
    Send_Data_URL += "&light=" + String(light);

    Serial.println();
    Serial.println("-------------");
    Serial.println("Send data to Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(Send_Data_URL);

    //::::::::::::::::::The process of sending or writing data to Google Sheets.
      // Initialize HTTPClient as "http".
      HTTPClient http;
  
      // HTTP GET Request.
      http.begin(Send_Data_URL.c_str());
      //http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
      // Gets the HTTP status code.
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code : ");
      Serial.println(httpCode);
  
      // Getting response from google sheets.
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload : " + payload);    
      }
      
      http.end();
    //::::::::::::::::::
    
    Serial.println("-------------");
  }
  //----------------------------------------

  delay(60000);
}
