#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_USE_LONG_LONG 1

#include <ESP8266WiFi.h>

//#include <WifiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "cryptos.h"

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


WiFiManager wm;


// ----------------------------
// Configurations 
// ----------------------------
unsigned long screenChangeDelay = 5000; //5seg between screen updates
unsigned long apiUpdateRate = 240000; //4 min between api updates

// ----------------------------
// ----------------------------

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Display connected to SCL and SDA pinout


int cryptosCount = (sizeof(cryptos) / sizeof(cryptos[0])); //Cryptos to show

HTTPClient http;
WiFiClientSecure client;

#define LED D2 // Led in NodeMCU 

//Some variables
unsigned long previousMillisApi = 0;
unsigned long previousMillisScreen = 0;
int currentCrypto = 0;
bool startUpFlag = true;


void displayShowText(String text)
{
  display.clearDisplay();
  display.setCursor(0,18);
  display.print(text);
  display.display();
}

void displayStartUp()
{
  display.clearDisplay();

  display.setTextSize(2);             
  display.setCursor(12,2);            
  display.println(F("CRYPTO"));
  display.setCursor(40,18); 
  display.println(F("Tracker"));

  display.setTextSize(1);
  display.setCursor(90,50);
  display.println(F("v1.0"));   

  //Fancy graph
  display.drawLine(1,63,12,55,SSD1306_WHITE);
  display.drawLine(12,55,15,59,SSD1306_WHITE);
  display.drawLine(15,59,20,50,SSD1306_WHITE);
  display.drawLine(20,50,23,52,SSD1306_WHITE);
  display.drawLine(23,52,30,49,SSD1306_WHITE);
  display.drawLine(30,49,42,40,SSD1306_WHITE);

  display.display();
}

void displayWifiManager(){
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(20,2);             
  display.println(F("No SSID found"));
  display.setCursor(2,12); 
  display.println(F("Connect to:"));
  display.setCursor(10,22); 
  display.println(F("default"));
  display.setCursor(10,32); 
  display.println(F("Password: password"));

  display.setCursor(2,45);
  display.println(F("And configure a new"));
  display.setCursor(2,55);
  display.println(F("SSID"));

  display.display();      // Show text
  Serial.println("Connect to new ssid");
}

void displayConnected(){
  display.clearDisplay();

  display.setTextSize(1);             
  display.setCursor(30,2);             
  display.println(F("Connected"));          
  display.setCursor(2,14);             
  display.println(F("IP address:"));
  display.setCursor(15,30);             
  display.println(WiFi.localIP()); // Print wifi IP addess

  display.display();

  delay(2500);
}

void waitConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    displayWifiManager();
    delay(300);
  }

  displayConnected();
  Serial.println("connected");
}

String getApiURL(String vsCurrency)
{
  String cryptosString = "";

  for (int i = 0; i < cryptosCount; i++)
  {
    cryptosString += cryptos[i].apiName + ",";
  }
  String result = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=" + vsCurrency + "&ids=" + cryptosString + "&order=market_cap_desc&per_page=100&page=1&sparkline=false&price_change_percentage=24h%2C7d";
  Serial.println("API URL:"+ result);

  return "https://api.coingecko.com/api/v3/coins/markets?vs_currency=" + vsCurrency + "&ids=" + cryptosString + "&order=market_cap_desc&per_page=100&page=1&sparkline=false&price_change_percentage=24h%2C7d";
}

int getCryptoIndexById(String id)
{
  for (int i = 0; i < cryptosCount; i++)
  {
    if (cryptos[i].apiName == id)
      return i;
  }
  return 0;
}

void downloadData(String vsCurrency)
{
  digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually
                      //the LED is on; this is because it is acive low on the ESP8266.

  http.useHTTP10(true);
  client.setInsecure();

  String apiUrl = getApiURL(vsCurrency);

  client.connect(apiUrl, 443);
  http.begin(client, apiUrl);

  if (http.GET() != HTTP_CODE_OK)
  {
    displayShowText("Error connecting to API");
    Serial.println("Error connecting to API");
    return;
  }

  // Create a filter to reduce memory usage

  StaticJsonDocument<1024> filter;

  for (int i = 0; i < cryptosCount; i++)
  {
    filter[i]["id"] = true;
    filter[i]["symbol"] = true;
    filter[i]["current_price"] = true;
    filter[i]["price_change_percentage_24h_in_currency"] = true;
    filter[i]["price_change_percentage_7d_in_currency"] = true;
  }

  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    displayShowText("JSON deserialization error");
    return;
  }

  Serial.println("Cryptos:");
  for (int i = 0; i < cryptosCount; i++)
  {
    JsonObject json = doc[i];
    String id = json["id"];
    int cryptoIndex = getCryptoIndexById(id);
    String symbol = json["symbol"];
    symbol.toUpperCase();
    double currentPrice = json["current_price"];
    double dayChange = json["price_change_percentage_24h_in_currency"];
    double weekChange = json["price_change_percentage_7d_in_currency"];

    cryptos[cryptoIndex].symbol = symbol;
    cryptos[cryptoIndex].priceUsd = currentPrice;
    cryptos[cryptoIndex].dayChange = dayChange;
    cryptos[cryptoIndex].weekChange = weekChange;
    Serial.println( cryptos[cryptoIndex].symbol);
    Serial.println( cryptos[cryptoIndex].priceUsd);
    Serial.println( cryptos[cryptoIndex].dayChange);
    Serial.println( cryptos[cryptoIndex].weekChange);
  }
  
    
  digitalWrite(LED, LOW); // turn the LED on.
}

String formatCurrency(float price)
{
  int digitsAfterDecimal = 3;

  if (price >= 1000)
  {
    digitsAfterDecimal = 0;
  }
  else if (price >= 100)
  {
    digitsAfterDecimal = 3;
  }
  else if (price >= 1)
  {
    digitsAfterDecimal = 4;
  }
  else if (price < 1)
  {
    digitsAfterDecimal = 4;
  }

  return String(price, digitsAfterDecimal);
}

void displayCryptoLogo(Crypto crypto)
{
  int offX = 90;
  int offY = 4;
  int width = 32;
  int height = 32;
  int iSize = 128;

  display.drawBitmap(offX, offY, (uint8_t *)(crypto.bitmap), width, height, WHITE);
}

String formatPercentageChange(double change)
{
  if (change >= 0)
  {
    return "+" + String(change) + "%";
  }
  else
  {
    return String(change) + "%";
  }
}

void displayCrypto(Crypto crypto)
{
  display.clearDisplay();
  displayCryptoLogo(crypto);

  if (crypto.symbol.length() <= 3)
  {
    display.setTextSize(2);
    display.setCursor(85,45);
    display.print(crypto.symbol);
  }
  else
  {
    display.setTextSize(2);
    display.setCursor(78,45);
    display.print(crypto.symbol);
  }

  display.setTextSize(2);
  display.setCursor(0,6);
  display.print("$");
  display.setTextSize(2);
  display.setCursor(12,6);
  display.print(formatCurrency(crypto.priceUsd));
  display.setTextSize(1);
  display.setCursor(0,42);
  display.print("24h:  " + formatPercentageChange(crypto.dayChange));
  display.setCursor(0,54);
  display.print("7d:  " + formatPercentageChange(crypto.weekChange));
  display.display();
}

void setup()
{

  Serial.begin(115200);

  pinMode(LED, OUTPUT); // set the digital pin as output.


  //display.init();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
    // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  displayStartUp();
  delay(2500); // Pause for 2.5 seconds
  

  WiFi.mode(WIFI_STA);

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(180);

  bool res;
  wm.setDebugOutput(true);
  
  res = wm.autoConnect("default","password"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      displayWifiManager();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
      wm.startConfigPortal();
  }

  // Clear the buffer
  display.clearDisplay();
  Serial.println("Colecting data");
  downloadData("usd");
}

void loop()
{
  wm.process(); //WiFi Manager process

  if (WiFi.status() == WL_CONNECTED){
    if (startUpFlag){
      startUpFlag = false;
      displayConnected();
      Serial.println("connected");
    }

    unsigned long currentMillis = millis();
  

    if (currentMillis - previousMillisApi >= apiUpdateRate) {

      previousMillisApi = currentMillis;

      Serial.println("Colecting data");
      downloadData("usd");
    }


    if (currentMillis - previousMillisScreen >= screenChangeDelay) {
      previousMillisScreen = currentMillis;
    
      if (currentCrypto < cryptosCount){
        displayCrypto(cryptos[currentCrypto]);
        currentCrypto++;
      } else {
        currentCrypto = 0; //Restart crypto to show
      }
    }
  }
  
  
}
