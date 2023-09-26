#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h> 

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "SimpleWeather.h"

String replaceSpacesWithNewlines(const String &input);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid     = "SSID";
const char* password = "PASSWORD";



// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String hour;
String minute;


const int ThermistorPin = 32;

int Vo;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;

//steinhart-hart coeficients for thermistor
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; 

void back();
// void sleep();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//http://api.openweathermap.org/data/2.5/weather?q=CITY,CO&APPID={API key}

String Key = ""; //API key
weatherData w;
OpenWeather weather(Key, "City,Co");
OpenWeather forecast(Key, "City,Co",1); //Co = country from openweather api


unsigned long interval = 0;  // Interval of 10 minutes in milliseconds
unsigned long previousMillis = 0;
bool run = true;




unsigned long bprevMillis = 0;
unsigned long clockInterval = 0;




// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

void setup() {
  Serial.begin(115200);



  // Start I2C communication with the Multiplexer
  Wire.begin();
  delay(1000);

// Init OLED display on bus number 2
  TCA9548A(0);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(1);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 4
  TCA9548A(2);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();


  // Init OLED display on bus number 5
  TCA9548A(3);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

    TCA9548A(4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();


    TCA9548A(5);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

      TCA9548A(6);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

      TCA9548A(7);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();
      TCA9548A(8);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

back();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  timeClient.begin();
  timeClient.setTimeOffset(-14400); //set time offest, EST example

  
 
 

}
 
void loop() {
  



  // Serial.print("DATE: ");
  // Serial.println(dayStamp);


  Vo = analogRead(ThermistorPin);
  //Serial.println(Vo);
  R2 = R1 * (4096.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15; //convert Kelvin to Celcius
  T = int(floor((T * 9.0)/ 5.0 + 32.0)); //convert Celcius to Fahrenheit

  // Serial.print("Temperature: "); 
  // Serial.print(T);
  // Serial.println(" F"); 

unsigned long currentMillis = millis();


if(currentMillis - bprevMillis >= clockInterval){
  bprevMillis = currentMillis;
  if(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedTime();
  //Serial.println("");
  //Serial.println(formattedDate);

  hour = formattedDate.substring(0,2);
  minute = formattedDate.substring(3,5);

  //Serial.print(hour);
  if (hour == "00") {
    hour = "12";
  }else if (hour == "13"){
    hour = " 1";
  }else if (hour == "14"){
    hour = " 2";  
  }else if (hour == "15"){
    hour = " 3"; 
  }else if (hour == "16"){
    hour = " 4";
  }else if (hour == "17"){
   hour = " 5"; 
  }else if (hour == "18"){
   hour = " 6"; 
  }else if (hour == "19"){
   hour = " 7"; 
  }else if (hour == "20"){
   hour = " 8"; 
  }else if (hour == "21"){
   hour = " 9"; 
  }else if (hour == "22"){
   hour = "10"; 
  }else if (hour == "23"){
    hour = "11";
  }else if (hour == "01"){
    hour = " 1";
  }else if (hour == "02"){
    hour = " 2";
  }else if (hour == "03"){
    hour = " 3";
  }else if (hour == "04"){
    hour = " 4";
  }else if (hour == "05"){
    hour = " 5";
  }else if (hour == "06"){
    hour = " 6";
  }else if (hour == "07"){
    hour = " 7";
  }else if (hour == "08"){
    hour = " 8";
  }else if (hour == "09"){
    hour = " 9";
  }

  //Serial.print("    ");
  //Serial.println(minute);

  clockInterval = 30000;
}
  // Check if the interval has passed
  if (currentMillis - previousMillis >= interval) {
    // Save the current time
    previousMillis = currentMillis;

    // Call your function here
     weather.updateStatus(&w);

    TCA9548A(4); //outside temp
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Present");
    display.setTextSize(2);
    String desc = w.description;
    desc = replaceSpacesWithNewlines(desc);
    display.print(desc);
    // /display.println("1");//fixed 6
    display.display();

    TCA9548A(3); //Weather now
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Outside");
    display.setTextSize(2);
    int F = (int(floor(w.current_Temp * 9.0)/ 5.0 + 32.0));
    display.print(F);
    display.println(" F");//fixed 6
    display.print("Hum:");
    display.print(int(floor(w.humidity)));
    display.println("%");

    display.print("Prep:");
    display.print(int(floor(w.rain)));
    display.println("%");
    display.display();

    run = true;
    interval = 600000;
  }









  //back();
  TCA9548A(0);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println(hour); //fixed 2
  display.display(); 

  TCA9548A(1);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println(minute); //fixed 2
  display.display(); 














if (run == true){

  // Write to OLED on bus number 5
  TCA9548A(2); //Inside temp
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  // Display static text
  display.println("Inside");
  display.print(int(T));
  display.println(" F");//fixed 6
  display.display(); 


  forecast.updateStatus(&w);

    TCA9548A(5); //Tomorrow weather
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Future");//fixed 6
  display.setTextSize(2);
  String desc = w.description;
  desc = replaceSpacesWithNewlines(desc);
  display.print(desc);
  display.display();
  run = false;
}


// Serial.println("\nOpenWeather Current:");

//   Serial.print("Weather: ");
//   Serial.println(w.weather);
//   Serial.print("Description: ");
//   Serial.println(w.description);
//   Serial.print("ID: ");
//   Serial.println(w.id);
//   Serial.print("Current Temp: ");
//   Serial.println(w.current_Temp);
//   Serial.print("Min Temp: ");
//   Serial.println(w.min_temp);
//   Serial.print("Max Temp: ");
//   Serial.println(w.max_temp);
//   Serial.print("Humidity: ");
//   Serial.println(w.humidity);
//   Serial.print("Rain: ");
//   Serial.println(w.rain);
//   Serial.print("Full Response: ");
//   Serial.println(weather.getResponse().c_str());

//   Serial.println("\nOpenWeather Forecast:");
//   forecast.updateStatus(&w);
//   Serial.print("Weather: ");
//   Serial.println(w.weather);
//   Serial.print("Description: ");
//   Serial.println(w.description);
//   Serial.print("ID: ");
//   Serial.println(w.id);
//   Serial.print("Current Temp: ");
//   Serial.println(w.current_Temp);
//   Serial.print("Min Temp: ");
//   Serial.println(w.min_temp);
//   Serial.print("Max Temp: ");
//   Serial.println(w.max_temp);
//   Serial.print("Humidity: ");
//   Serial.println(w.humidity);
//   Serial.print("Rain: ");
//   Serial.println(w.rain);
//   Serial.print("Full Response: ");
//   Serial.println(forecast.getResponse().c_str());



//   delay(600000);       // Wait for 600 seconds










}




void back(){

  TCA9548A(0);
  display.drawCircle(10,10,10,WHITE);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("0"); //fixed 2
  display.display(); 

  // Write to OLED on bus number 3
  TCA9548A(1);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("1"); //fixed Youtube 7
  display.display(); 
  
  // Write to OLED on bus number 4
  TCA9548A(2);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("2"); //fixed  8 
  display.display(); 
  
  // Write to OLED on bus number 5
  TCA9548A(3);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("3");//fixed 6
  display.display(); 

    TCA9548A(4);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("4");//fixed 5 
  display.display(); 


    TCA9548A(5);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("5"); //fixed 3
  display.display(); 

      TCA9548A(6);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("6"); //fixed 1
  display.display(); 

      TCA9548A(7);
  
   display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text 
  display.println("7"); //fixed 4
  display.display(); 
  delay(1000);

}

String replaceSpacesWithNewlines(const String &input) {
  String modified = "";

  for (size_t i = 0; i < input.length(); i++) {
    if (input[i] == ' ') {
      modified += '\n';
    } else {
      modified += input[i];
    }
  }

  return modified;
}