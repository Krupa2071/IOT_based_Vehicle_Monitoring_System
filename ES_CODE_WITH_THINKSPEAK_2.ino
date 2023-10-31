#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
static const int RX= D6, TX= D7;
static const uint32_t GPSBaud = 9600;
const char* ssid     = "Krupa's DEKSTOP";//Replace with Network SSID
const char* password = "Krupa@20";//Replace with Network Password
unsigned long ch_no = 1695964;//Replace with Thingspeak Channel number
const char * write_api = "2IFO0B9NE6AVRWME";//Replace with Thingspeak write API
TinyGPSPlus gps;
WiFiClient  client;
WiFiServer server(80);
SoftwareSerial soft(RX, TX);
String latitude_data;
String longitude_data;
void setup()
{
  Wire.begin(D2, D1);
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome to VTS");
  Serial.println("Welcome to VTS");
  lcd.setCursor(0,1);
  lcd.print("Embedded Systems");
  Serial.println("Embedded Systems");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Course faculty :");
  Serial.println("Course faculty :");
  lcd.setCursor(0,1);
  lcd.print("Prof.Hardik Joshi");
  Serial.println("Prof.Hardik Joshi");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Special Assignment");
  Serial.println("Special Assignment");
  lcd.setCursor(0,1);
  lcd.print("Group Members :");
  Serial.println("Group Members :");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Krupa    ");
  Serial.println("   Krupa    ");
  lcd.setCursor(0,1);
  lcd.print("   Manasi     ");
  Serial.println("   Manasi     ");
  delay(3000);
  lcd.clear(); 
  soft.begin(GPSBaud);
  WiFi.begin(ssid, password);
  server.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("WiFi connecting...          ");
    Serial.println("WiFi connecting...          ");
  }
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected          ");
  Serial.println("WiFi connected          ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  ThingSpeak.begin(client);
}
void loop()
{
  while (soft.available() > 0)
    if (gps.encode(soft.read()))
    {
      displaydata();
      displaywebpage();
    }
     while (soft.available() > 0)
    if (gps.encode(soft.read()))
      displaydata();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}
void displaydata()
{
  if (gps.location.isValid())
  {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    latitude_data= (String(latitude, 6));
    longitude_data= (String(longitude, 6));
    ThingSpeak.setField(1, latitude_data);
    ThingSpeak.setField(2, longitude_data);
    ThingSpeak.writeFields(ch_no, write_api);
    delay(20000);
  }
  else
  {
    Serial.println(F("Data error!!!"));
  }
}
void displaywebpage()
{
    WiFiClient client = server.available();
    if (!client)
    {
      return;
    }
    String page = "<html><center><p><h1>Real Time Vehicle Tracking using IoT</h1><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    page += latitude_data;
    page += "+";
    page += longitude_data;
    page += ">Click here For Live Location</a> </p></center></html>";
    
    client.print(page);
    delay(100);
}
