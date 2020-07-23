#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(D2, D1, D5, D6, D7, D8);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

int om;
int oh;
int cm;
int ch;

int address = 0;
int oma = 1;
int oha = 2;
int cma = 3;
int cha = 4;



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
<hr>
<h3>Live Control</h3>
<form method="get" action="open">
<input type="submit" value="Open">
</form>
<form method="get" action="close">
<input type="submit" value="Close">
</form>
<hr>
<h3>Time Control</h3>
Please enter UTC time in the "xx:xx" format.<br>
<form method="get" action="timectrl">
<label for="open">Opening Time</label><br>
<input type="text" id="open" name="open" value="07:00"><br>
<label for="close">Closing Time</label><br>
<input type="text" id="close" name="close" value="20:00"><br><br>
<input type="submit" value="Submit">
</form>
</center>
</body>
</html>
)=====";
//---------------------------------------------------------------

void handleRoot() {
 Serial.println("Called Root Page");
 digitalWrite(D4, HIGH);
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
 digitalWrite(D4, LOW);
}

void handleOpen() {
 Serial.println("Open!");
 digitalWrite(D4, HIGH);
 server.send(200, "text/plain", "OK! Status: Open"); //Send web page
 lcd.clear();
 lcd.print("       OPEN!!       ");
 EEPROM.write(address, 1);
 EEPROM.commit();
 digitalWrite(D4, LOW);
}

void handleClosed() {
 Serial.println("Closed!");
 digitalWrite(D4, HIGH);
 server.send(200, "text/plain", "OK! Status: Closed"); //Send web page
 lcd.clear();
 lcd.print("      CLOSED!!      ");
 EEPROM.write(address, 0);
 EEPROM.commit();
 digitalWrite(D4, LOW);
}

void handleTimeCtrl() {
  String msg;
  digitalWrite(D4, HIGH);
  
  String opentime = server.arg("open");
  String closetime = server.arg("close");
  oh = opentime.substring(0, opentime.indexOf(":")).toInt();
  om = opentime.substring(opentime.indexOf(":") + 1).toInt();
  ch = closetime.substring(0, closetime.indexOf(":")).toInt();
  cm = closetime.substring(closetime.indexOf(":") + 1).toInt();
  
  EEPROM.write(oma, om);
  EEPROM.write(oha, oh);
  EEPROM.write(cma, cm);
  EEPROM.write(cha, ch);
  EEPROM.commit();

  
  msg += oh;
  msg += ":";
  msg += om;
  msg += " -> ";
  msg += ch;
  msg += ":";
  msg += cm;
  msg += "\n";
  
  
  server.send(200, "text/plain", msg); //Send web page
  Serial.println(msg);
  digitalWrite(D4, LOW);
}

void setup() {
  pinMode(D4, OUTPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(20, 2);
  lcd.clear();

  lcd.print("EEPROM...");
  EEPROM.begin(512);
  int inist = EEPROM.read(address);
  om = EEPROM.read(oma);
  oh = EEPROM.read(oha);
  cm = EEPROM.read(cma);
  ch = EEPROM.read(cha);
  lcd.clear();
  lcd.print("EEPROM... OK");
  delay(1000);
  lcd.clear();
  
  lcd.print("WiFi...");
  WiFiManager wm;
  while(!wm.autoConnect()) {
    ESP.restart();
  }
  
  lcd.clear();
  lcd.print("WiFi... OK");
  delay(500);
  
  lcd.clear();
  lcd.print("OTA...");
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("DoorSign");

  // No authentication by default
  //ArduinoOTA.setPassword("doorie");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    lcd.clear();
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
  server.on("/timectrl", handleTimeCtrl);

  server.begin(); //Start server
  Serial.println("HTTP server started");
  lcd.clear();
  lcd.print("HTTP... OK");
  delay(1000);
  lcd.clear();
  
  if (inist == 1) {
    handleOpen();
  }
  else {
    handleClosed();
  }
}

void checkTime() {
  timeClient.update();
  lcd.setCursor(0,1);
  lcd.print("    ");
  lcd.print(timeClient.getFormattedTime());
  lcd.print(" UTC");
  if (timeClient.getHours() == oh and timeClient.getMinutes() == om) {
    Serial.println("Opened by Timer");
    lcd.clear();
    lcd.print("       OPEN!!       ");
  }
  if(timeClient.getHours() == ch and timeClient.getMinutes() == cm) {
    Serial.println("Closed by Timer");
    lcd.clear();
    lcd.print("      CLOSED!!      ");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  ArduinoOTA.handle();
  checkTime();
  lcd.setCursor(0,0);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
  }
}