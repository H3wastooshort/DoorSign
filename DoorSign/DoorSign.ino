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

int om1;
int oh1;
int cm1;
int ch1;
int om2;
int oh2;
int cm2;
int ch2;
int om3;
int oh3;
int cm3;
int ch3;
int om4;
int oh4;
int cm4;
int ch4;
int om5;
int oh5;
int cm5;
int ch5;
int om6;
int oh6;
int cm6;
int ch6;
int om7;
int oh7;
int cm7;
int ch7;

int address = 0;



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
<form method="get" action="closed">
<input type="submit" value="Close">
</form>
<hr>
<h3>Time Control</h3>
Please enter UTC time in the "xx:xx" format.<br><br>
<form method="get" action="timectrl">
<label for="open1">Opening Time (Monday)</label><br>
<input type="text" id="open1" name="open1" value="xx:xx"><br>
<label for="close1">Closing Time(Monday)</label><br>
<input type="text" id="close1" name="close1" value="xx:xx"><br><br>

<label for="open2">Opening Time (Tuesday)</label><br>
<input type="text" id="open2" name="open2" value="xx:xx"><br>
<label for="close2">Closing Time(Tuesday)</label><br>
<input type="text" id="close2" name="close2" value="xx:xx"><br><br>

<label for="open3">Opening Time (Wendsday)</label><br>
<input type="text" id="open3" name="open3" value="xx:xx"><br>
<label for="close3">Closing Time(Wendsday)</label><br>
<input type="text" id="close3" name="close3" value="xx:xx"><br><br>

<label for="open4">Opening Time (Thursday)</label><br>
<input type="text" id="open4" name="open4" value="xx:xx"><br>
<label for="close4">Closing Time(Thursday)</label><br>
<input type="text" id="close4" name="close4" value="xx:xx"><br><br>

<label for="open5">Opening Time (Friday)</label><br>
<input type="text" id="open5" name="open5" value="xx:xx"><br>
<label for="close5">Closing Time(Friday)</label><br>
<input type="text" id="close5" name="close5" value="xx:xx"><br><br>

<label for="open6">Opening Time (Saturtay)</label><br>
<input type="text" id="open6" name="open6" value="xx:xx"><br>
<label for="close6">Closing Time(Saturday)</label><br>
<input type="text" id="close6" name="close6" value="xx:xx"><br><br>

<label for="open7">Opening Time (Sunday)</label><br>
<input type="text" id="open7" name="open7" value="xx:xx"><br>
<label for="close7">Closing Time(Sunday)</label><br>
<input type="text" id="close7" name="close7" value="xx:xx"><br><br>

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
  int ch;
 int cm;
 int oh;
 int om;
 switch(timeClient.getDay()) {
  case 1:
  ch = ch1;
  cm = cm1;
  om = om1;
  oh = oh1;
  break;
  case 2:
  ch = ch2;
  cm = cm2;
  om = om2;
  oh = oh2;
  break;
  case 3:
  ch = ch3;
  cm = cm3;
  om = om3;
  oh = oh3;
  break;
  case 4:
  ch = ch4;
  cm = cm4;
  om = om4;
  oh = oh4;
  break;
  case 5:
  ch = ch5;
  cm = cm5;
  om = om5;
  oh = oh5;
  break;
  case 6:
  ch = ch6;
  cm = cm6;
  om = om6;
  oh = oh6;
  break;
  case 0:
  ch = ch7;
  cm = cm7;
  om = om7;
  oh = oh7;
  break;
  
 }
 lcd.setCursor(0,1);
 lcd.print(oh);
 lcd.print(":");
 lcd.print(om);
 lcd.print("UTC to ");
 lcd.print(ch);
 lcd.print(":");
 lcd.print(cm);
 lcd.print("UTC");
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
 int ch;
 int cm;
 int oh;
 int om;
 switch(timeClient.getDay()) {
  case 1:
  ch = ch1;
  cm = cm1;
  om = om1;
  oh = oh1;
  break;
  case 2:
  ch = ch2;
  cm = cm2;
  om = om2;
  oh = oh2;
  break;
  case 3:
  ch = ch3;
  cm = cm3;
  om = om3;
  oh = oh3;
  break;
  case 4:
  ch = ch4;
  cm = cm4;
  om = om4;
  oh = oh4;
  break;
  case 5:
  ch = ch5;
  cm = cm5;
  om = om5;
  oh = oh5;
  break;
  case 6:
  ch = ch6;
  cm = cm6;
  om = om6;
  oh = oh6;
  break;
  case 0:
  ch = ch7;
  cm = cm7;
  om = om7;
  oh = oh7;
  break;
  
 }
 
 lcd.setCursor(0,1);
 lcd.print(ch);
 lcd.print(":");
 lcd.print(cm);
 lcd.print("UTC to ");
 lcd.print(oh);
 lcd.print(":");
 lcd.print(om);
 lcd.print("UTC");
 EEPROM.write(address, 0);
 EEPROM.commit();
 digitalWrite(D4, LOW);
}

void handleTimeCtrl() {
  String msg;
  digitalWrite(D4, HIGH);
  
  String opentime1 = server.arg("open1");
  String closetime1 = server.arg("close1");
  String opentime2 = server.arg("open2");
  String closetime2 = server.arg("close2");
  String opentime3 = server.arg("open3");
  String closetime3 = server.arg("close3");
  String opentime4 = server.arg("open4");
  String closetime4 = server.arg("close4");
  String opentime5 = server.arg("open5");
  String closetime5 = server.arg("close5");
  String opentime6 = server.arg("open6");
  String closetime6 = server.arg("close6");
  String opentime7 = server.arg("open7");
  String closetime7 = server.arg("close7");

  oh1 = opentime1.substring(0, opentime1.indexOf(":")).toInt();
  om1 = opentime1.substring(opentime1.indexOf(":") + 1).toInt();
  ch1 = closetime1.substring(0, closetime1.indexOf(":")).toInt();
  cm1 = closetime1.substring(closetime1.indexOf(":") + 1).toInt();
  oh2 = opentime2.substring(0, opentime2.indexOf(":")).toInt();
  om2 = opentime2.substring(opentime2.indexOf(":") + 1).toInt();
  ch2 = closetime2.substring(0, closetime2.indexOf(":")).toInt();
  cm2 = closetime2.substring(closetime2.indexOf(":") + 1).toInt();
  oh3 = opentime3.substring(0, opentime3.indexOf(":")).toInt();
  om3 = opentime3.substring(opentime3.indexOf(":") + 1).toInt();
  ch3 = closetime3.substring(0, closetime3.indexOf(":")).toInt();
  cm3 = closetime3.substring(closetime3.indexOf(":") + 1).toInt();
  oh4 = opentime4.substring(0, opentime4.indexOf(":")).toInt();
  om4 = opentime4.substring(opentime4.indexOf(":") + 1).toInt();
  ch4 = closetime4.substring(0, closetime4.indexOf(":")).toInt();
  cm4 = closetime4.substring(closetime4.indexOf(":") + 1).toInt();
  oh5 = opentime5.substring(0, opentime5.indexOf(":")).toInt();
  om5 = opentime5.substring(opentime5.indexOf(":") + 1).toInt();
  ch5 = closetime5.substring(0, closetime5.indexOf(":")).toInt();
  cm5 = closetime5.substring(closetime5.indexOf(":") + 1).toInt();
  oh6 = opentime6.substring(0, opentime6.indexOf(":")).toInt();
  om6 = opentime6.substring(opentime6.indexOf(":") + 1).toInt();
  ch6 = closetime6.substring(0, closetime6.indexOf(":")).toInt();
  cm6 = closetime6.substring(closetime6.indexOf(":") + 1).toInt();
  oh7 = opentime7.substring(0, opentime7.indexOf(":")).toInt();
  om7 = opentime7.substring(opentime7.indexOf(":") + 1).toInt();
  ch7 = closetime7.substring(0, closetime7.indexOf(":")).toInt();
  cm7 = closetime7.substring(closetime7.indexOf(":") + 1).toInt();
  
  
  EEPROM.write(address+1, om1);
  EEPROM.write(address+2, oh1);
  EEPROM.write(address+3, cm1);
  EEPROM.write(address+4, ch1);

  EEPROM.write(address+5, om2);
  EEPROM.write(address+6, oh2);
  EEPROM.write(address+7, cm2);
  EEPROM.write(address+8, ch2);

  EEPROM.write(address+9, om3);
  EEPROM.write(address+10, oh3);
  EEPROM.write(address+11, cm3);
  EEPROM.write(address+12, ch3);

  EEPROM.write(address+13, om4);
  EEPROM.write(address+14, oh4);
  EEPROM.write(address+15, cm4);
  EEPROM.write(address+16, ch4);

  EEPROM.write(address+17, om5);
  EEPROM.write(address+18, oh5);
  EEPROM.write(address+19, cm5);
  EEPROM.write(address+20, ch5);

  EEPROM.write(address+21, om6);
  EEPROM.write(address+22, oh6);
  EEPROM.write(address+23, cm6);
  EEPROM.write(address+24, ch6);

  EEPROM.write(address+25, om7);
  EEPROM.write(address+26, oh7);
  EEPROM.write(address+27, cm7);
  EEPROM.write(address+28, ch7);
  EEPROM.commit();
  
  server.send(200, "text/plain", "OK!"); //Send web page
  Serial.println("Time Set!");
  handleOpen();
  digitalWrite(D4, LOW);
}

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D4, OUTPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(20, 2);
  lcd.clear();

  lcd.print("EEPROM...");
  EEPROM.begin(512);
  int inist = EEPROM.read(address);
  
  om1 = EEPROM.read(address+1);
  oh1 = EEPROM.read(address+2);
  cm1 = EEPROM.read(address+3);
  ch1 = EEPROM.read(address+4);

  om2 = EEPROM.read(address+5);
  oh2 = EEPROM.read(address+6);
  cm2 = EEPROM.read(address+7);
  ch2 = EEPROM.read(address+8);

  om3 = EEPROM.read(address+9);
  oh3 = EEPROM.read(address+10);
  cm3 = EEPROM.read(address+11);
  ch3 = EEPROM.read(address+12);

  om4 = EEPROM.read(address+13);
  oh4 = EEPROM.read(address+14);
  cm4 = EEPROM.read(address+15);
  ch4 = EEPROM.read(address+16);

  om5 = EEPROM.read(address+17);
  oh5 = EEPROM.read(address+18);
  cm5 = EEPROM.read(address+19);
  ch5 = EEPROM.read(address+20);

  om6 = EEPROM.read(address+21);
  oh6 = EEPROM.read(address+22);
  cm6 = EEPROM.read(address+23);
  ch6 = EEPROM.read(address+24);

  om7 = EEPROM.read(address+25);
  oh7 = EEPROM.read(address+26);
  cm7 = EEPROM.read(address+27);
  ch7 = EEPROM.read(address+28);
  
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
  if (timeClient.getHours() == oh1 and timeClient.getMinutes() == om1 and timeClient.getSeconds() < 10 and timeClient.getDay() == 1) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch1 and timeClient.getMinutes() == cm1 and timeClient.getSeconds() < 10 and timeClient.getDay() == 1) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh2 and timeClient.getMinutes() == om2 and timeClient.getSeconds() < 10 and timeClient.getDay() == 2) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch2 and timeClient.getMinutes() == cm2 and timeClient.getSeconds() < 10 and timeClient.getDay() == 2) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh3 and timeClient.getMinutes() == om3 and timeClient.getSeconds() < 10 and timeClient.getDay() == 3) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch3 and timeClient.getMinutes() == cm3 and timeClient.getSeconds() < 10 and timeClient.getDay() == 3) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh4 and timeClient.getMinutes() == om4 and timeClient.getSeconds() < 10 and timeClient.getDay() == 4) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch4 and timeClient.getMinutes() == cm4 and timeClient.getSeconds() < 10 and timeClient.getDay() == 4) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh5 and timeClient.getMinutes() == om5 and timeClient.getSeconds() < 10 and timeClient.getDay() == 5) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch5 and timeClient.getMinutes() == cm5 and timeClient.getSeconds() < 10 and timeClient.getDay() == 5) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh6 and timeClient.getMinutes() == om6 and timeClient.getSeconds() < 10 and timeClient.getDay() == 6) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch6 and timeClient.getMinutes() == cm6 and timeClient.getSeconds() < 10 and timeClient.getDay() == 6) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
  if (timeClient.getHours() == oh7 and timeClient.getMinutes() == om7 and timeClient.getSeconds() < 10 and timeClient.getDay() == 7) {
    Serial.println("Opened by Timer");
    handleOpen();
    digitalWrite(D0, LOW);
  }
  if(timeClient.getHours() == ch7 and timeClient.getMinutes() == cm7 and timeClient.getSeconds() < 10 and timeClient.getDay() == 7) {
    Serial.println("Closed by Timer");
    handleClosed();
    digitalWrite(D0, HIGH);
}
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  ArduinoOTA.handle();
  checkTime();
  Serial.println(timeClient.getFormattedTime());
  lcd.setCursor(0,0);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(250);
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
  }
}