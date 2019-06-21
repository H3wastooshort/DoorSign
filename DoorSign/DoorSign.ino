#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(D2, D1, D5, D6, D7, D8);

//SSID and Password of your WiFi router
const char* ssid = "FRITZBoxKarin";
const char* password = "2871247716916185";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

bool dpTime = false;

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Door sign</title>
</head>
<body>
<center>
<h1>Door sign</h1><br>
Click to show <a href="open">Open</a><br>
Click to show <a href="closed">Closed</a><br>
<hr>
</center>

</body>
</html>
)=====";
//---------------------------------------------------------------

void handleRoot() {
 Serial.println("You called root page");
 digitalWrite(D4, HIGH);
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
 digitalWrite(D4, LOW);
}

void handleOpen() {
 Serial.println("You called open page");
 digitalWrite(D4, HIGH);
 server.send(200, "text/plain", "OK! Status: Open"); //Send web page
 lcd.clear();
 lcd.print("     OPEN!!     ");
 digitalWrite(D4, LOW);
 dpTime = true;
}

void handleClosed() {
 Serial.println("You called closed page");
 digitalWrite(D4, HIGH);
 server.send(200, "text/plain", "OK! Status: Closed"); //Send web page
 lcd.clear();
 lcd.print("    CLOSED!!    ");
 digitalWrite(D4, LOW);
 dpTime = true;
}

void setup() {
  pinMode(D4, OUTPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("WiFi...");
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
  }
  lcd.clear();
  lcd.print("WiFi... OK");
  delay(500);
  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("DoorSign");

  // No authentication by default
   ArduinoOTA.setPassword("doorie");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  lcd.print("OTA...");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
    lcd.clear();
    lcd.print("OTA... " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    lcd.clear();
    lcd.print("OTA... End");
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    digitalWrite(D4, HIGH);
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    lcd.setCursor(0,1);
    lcd.printf("Progress: %u%%\r", (progress / (total / 100)));
    delay(50);
    digitalWrite(D4, LOW);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
      lcd.clear();
      lcd.print("Auth Failed");
      digitalWrite(D4, HIGH);
      delay(5000);
      digitalWrite(D4, LOW);
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
      lcd.clear();
      lcd.print("Begin Failed");
      digitalWrite(D4, HIGH);
      delay(5000);
      digitalWrite(D4, LOW);
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.print("Connect Failed");
      lcd.clear();
      lcd.println("Connect Failed");
      digitalWrite(D4, HIGH);
      delay(5000);
      digitalWrite(D4, LOW);
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.print("Receive Failed");
      lcd.clear();
      lcd.print("Receive Failed");
      digitalWrite(D4, HIGH);
      delay(5000);
      digitalWrite(D4, LOW);
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
      lcd.clear();
      lcd.print("End Failed");
      digitalWrite(D4, HIGH);
      delay(5000);
      digitalWrite(D4, LOW);
    }
  });
  ArduinoOTA.begin();
  lcd.clear();
  lcd.print("OTA... OK");
  delay(500);

  lcd.clear();
  lcd.print("NTP...");
  timeClient.begin();
  lcd.clear();
  lcd.print("NTP... OK");
  delay(500);
    
  lcd.print("HTTP...");
  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/closed", handleClosed);

  server.begin(); //Start server
  Serial.println("HTTP server started");
  lcd.clear();
  lcd.print("HTTP... OK");
  delay(1000);
  lcd.clear();
  lcd.print("Use Webinterface");
  lcd.setCursor(0,1);
  lcd.print("to set Status!");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  ArduinoOTA.handle();
  timeClient.update();
  if (dpTime) {
  lcd.setCursor(0,1);
  lcd.print("   ");
  lcd.print(timeClient.getFormattedTime());
  lcd.print("UTC");
  lcd.setCursor(0,0);
  }
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
  }
}
