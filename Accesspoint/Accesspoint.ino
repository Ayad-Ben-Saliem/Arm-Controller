/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */


#include <ESP8266WiFi.h>

byte ledPin = 11;

char ssid[] = "WeMosD1AP";
char pass[] = "123456789";

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

WiFiClient client;


/*
 * NOTE: Four Servos of the Robotic Arm are connected to 4 PWM Pins of Arduino
 * and these 4 servos are named a, b, c and d.
 * If you want to control servo a, then type "90a/",
 * where "90" is the PWM value (range is 0 - 255),
 * "a" means servo a and "/" is string parse delimiter.
 * Some other examples: 100a/ or 120b/ or 40c/ or 25d/
 */
String readString;
int x=90, y=90, p=90;
#include <Servo.h>
Servo myservoa, myservob, myservod;

String message;


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();

  //pinMode(ledPin, OUTPUT);
  
  Serial.println("\nServer started.");
  Serial.println("IP  : " + WiFi.softAPIP().toString());
  Serial.println("MAC : " + WiFi.softAPmacAddress());

  myservoa.attach(3);
  myservob.attach(5);
  myservod.attach(9);
  myservoa.write(x);
  myservob.write(y);
  myservod.write(p);


  do{
    client = server.available();
  }while(!client);
  Serial.println("Client @(" +
                  client.remoteIP().toString() +
                  ") has been connected.");
}

void loop() {
  if (client && client.available()){
    char m = client.read();
    if (m != '/') {
      message += m;
    } else {
      if (message.length() > 1) {
        Serial.println(message);
        int temp = message.toInt();
        Serial.printf("writing Angle: %d", temp);
        
        if (message.indexOf('a') >= 0) {
          if (temp > x) {
            for (int i = x; i < temp; i++) {
              myservoa.write(i);
              delay(10);
            }
            x = temp;
          } else {
            for (int i=x; i>temp; i--) {
              myservoa.write(i);
              delay(30);
            }
          }
          x = temp;
        }
//////////////////////////////////////////////////////////////////////////////
        if (message.indexOf('b') >= 0) {
          if (temp>y) {
            for (int i=y; i<temp; i++) {
              myservob.write(i);
              delay(10);
            }
            y = temp;
          } else {
            for (int i=y; i>temp; i--){
              myservob.write(i);
              delay(10);
            }
            y = temp;
          }
        }
///////////////////////////////////////////////////////////////////////
        if (message.indexOf('d') >= 0) {
          if (temp > p) {
            for (int i = p; i < temp; i++){
              myservod.write(i);
              delay(10);
            }
            p = temp;
          } else {
            for (int i = p; i > temp; i--){
              myservod.write(i);
              delay(30);
            }
          }
          p = temp;
        }
        message = "";
      }
    }
  }
}
