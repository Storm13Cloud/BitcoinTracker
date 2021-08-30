/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* ssid = "SSID";
const char* password = "PSWD";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("connected");
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://api.coindesk.com/v1/bpi/currentprice/BTC.json");
    int httpCode = http.GET();
    display.clearDisplay();
    display.setCursor(0, 0);

    if (httpCode > 0) 
    {
      const size_t capacity = JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + 2*JSON_OBJECT_SIZE(4) + 460;
      DynamicJsonDocument doc(capacity);
      String payload = http.getString();
      unsigned int strlength = payload.length();
      char copy[strlength];
      payload.toCharArray(copy, strlength);
      const char* json = copy;
      
      deserializeJson(doc, json);
      
      JsonObject time = doc["time"];
      const char* time_updated = time["updated"]; // "Aug 12, 2020 02:05:00 UTC"
      const char* time_updatedISO = time["updatedISO"]; // "2020-08-12T02:05:00+00:00"
      const char* time_updateduk = time["updateduk"]; // "Aug 12, 2020 at 03:05 BST"
      
      const char* disclaimer = doc["disclaimer"]; // "This data was produced from the CoinDesk Bitcoin Price Index (USD). Non-USD currency data converted using hourly conversion rate from openexchangerates.org"
      
      JsonObject bpi_USD = doc["bpi"]["USD"];
      const char* bpi_USD_code = bpi_USD["code"]; // "USD"
      const char* bpi_USD_rate = bpi_USD["rate"]; // "11,340.0257"
      const char* bpi_USD_description = bpi_USD["description"]; // "United States Dollar"
      float bpi_USD_rate_float = bpi_USD["rate_float"]; // 11340.0257
      
      JsonObject bpi_BTC = doc["bpi"]["BTC"];
      const char* bpi_BTC_code = bpi_BTC["code"]; // "BTC"
      const char* bpi_BTC_rate = bpi_BTC["rate"]; // "1.0000"
      const char* bpi_BTC_description = bpi_BTC["description"]; // "Bitcoin"
      int bpi_BTC_rate_float = bpi_BTC["rate_float"]; // 1

      
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.println(bpi_USD_rate);
      display.display();
      delay(2000);

    }
  }
  delay(10000);
}


void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}


    
  
