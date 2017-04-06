/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <stdio.h>
#include "index2.h"
#include <Servo.h> 
#include <ESP.h>
#include <Arduino.h>
extern "C" {
#include <user_interface.h>
}
 
Servo myservo1;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
Servo myservo2;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

static const uint8_t PD0   = 16;
static const uint8_t PD1   = 5;
static const uint8_t PD2   = 4;
static const uint8_t PD3   = 0;
static const uint8_t PD4   = 2;
static const uint8_t PD5   = 14;
static const uint8_t PD6   = 12;
static const uint8_t PD7   = 13;
static const uint8_t PD8   = 15;
static const uint8_t PD9   = 3;
static const uint8_t PD10  = 1;

int led1 = PD0;
int led2 = PD1;
int servo1 = PD5;
int servo2 = PD6;
  String cled1 ="";
  int iled1 = 0;
  String cservo1 ="";
  int iservo1 = 0;
  String cservo2 ="";
  int iservo2 = 0;
  int endindex = 0;
  int startValindex = 0;
  int servo1Drv=0;
  int servo2Drv=0;

  int htmlCount = 0;
//ESP8266WebServer server(80);
WiFiServer server(80);
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */

void setup() {
  rst_info *rsti;

  rsti = ESP.getResetInfoPtr();

  pinMode ( led1, OUTPUT );
  pinMode ( led2, OUTPUT );
  pinMode ( servo1, OUTPUT );
  pinMode ( servo2, OUTPUT );
  myservo1.attach(servo1);  // attaches the servo on GIO2 to the servo object 
  myservo2.attach(servo2);  // attaches the servo on GIO2 to the servo object 
	//delay(1000);
	Serial.begin(115200);
  Serial.printf("RST_INFO = %lu\r\n", rsti->reason);
  Serial.print("RESET REASON => ");
  Serial.println(ESP.getResetReason());
  
  ESP.wdtDisable();
  ESP.wdtEnable(65000);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
  htmlCode_init();
  server.begin();
	Serial.println("HTTP server started");
  ESP.wdtFeed();
}

void loop() {
	//server.handleClient();
  ESP.wdtFeed();
  WiFiClient client = server.available();
  if (!client) {
    ESP.wdtFeed();
    return;
  }
   
  //Serial.println("new client");
  while(!client.available()){
    ESP.wdtFeed();
    delay(1);
  }
  ESP.wdtFeed();
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  if(req.indexOf("led1=") != -1)
  {
    iled1 = req.indexOf("led1=");
    endindex = req.indexOf('$');
    startValindex = req.indexOf('=');
    Serial.print(iled1);
    cled1 = req .substring(startValindex+1,endindex);
    Serial.println(cled1);
    ESP.wdtFeed();
    if (cled1.indexOf("true") != -1){
      digitalWrite ( led1, 1 );
      digitalWrite ( led2, 1 );
      Serial.println("LED1ON");
    }
    else if (cled1.indexOf("false") != -1){
      digitalWrite ( led1, 0 );
      digitalWrite ( led2, 0 );
      Serial.println("LED1OFF");
    }else{
      Serial.println("LED1ERR");
    }
    ESP.wdtFeed();
    iservo1 = req.indexOf("servo1=");
    endindex = req.indexOf('$',endindex+1);
    startValindex = req.indexOf('=',startValindex+1);
    Serial.print(iservo1);
    cservo1 = req .substring(startValindex+1,endindex);
    Serial.println(cservo1);
    servo1Drv=cservo1.toInt();
    myservo1.write(servo1Drv);
    Serial.println(servo1Drv,DEC);
    ESP.wdtFeed();
    iservo2 = req.indexOf("servo2=");
    endindex = req.indexOf('$',endindex+2);
    startValindex = req.indexOf('=',startValindex+2);
    Serial.print(iservo1);
    cservo2 = req .substring(startValindex+1,endindex);
    Serial.println(cservo2);
    servo2Drv=cservo2.toInt();
    myservo2.write(servo2Drv);
    Serial.println(servo2Drv,DEC);
    delay(10);
    ESP.wdtFeed();
  }
  else if(req.indexOf("/") != -1)
  //if(req.indexOf("/") != -1)
  {
    ESP.wdtFeed();
    for (htmlCount = 0; htmlCount<pageCount;htmlCount++){
      client.print(htmlCode[htmlCount]);
    }
    client.flush();
    ESP.wdtFeed();
  }
  
}
