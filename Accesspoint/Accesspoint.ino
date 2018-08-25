/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */


#include <ESP8266WiFi.h>

char ssid[] = "WIFI_AP";
char pass[] = "123456789";

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

WiFiClient client;


/* NOTE: Three Servo motors of the Robotic Arm are connected to 
 * 3 PWM Pins of Arduino and these 3 servos are named a, b and c.
 * If you want to control servo a, then type "90a/",
 * where "90" is the PWM value (range is 0 - 255),
 * "a" means servo a and "/" is string parse delimiter.
 * Some other examples: 100a/ or 120b/ or 40c/
*/

#include <Servo.h>

String message;
int x=90, y=90, z=90;

Servo servo_a, servo_b, servo_c;

void moveServo(Servo &servo, int &currentAngle, int &oldAngle);

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  
  Serial.println("\nServer started.");
  Serial.println("IP  : " + WiFi.softAPIP().toString());
  Serial.println("MAC : " + WiFi.softAPmacAddress());


  do{
    client = server.available();
  }while(!client);
  Serial.println("Client @(" +
                  client.remoteIP().toString() +
                  ") has been connected.");

  servo_a.attach(D3);
  servo_b.attach(D4);
  servo_c.attach(D5);
  
  servo_a.write(x);
  servo_b.write(y);
  servo_c.write(z);
}

void loop() {
  while (!client)
    client = server.available();

  if(client.available()){
    char c = client.read();
    
    if (c != '/') {
      message += c;
    } else {
      if (message.length() > 1) {
        
        int currentAngle = message.toInt();
        Serial.print("move to angle : ");
        Serial.println(currentAngle);

        if(message.endsWith("a")) {
          Serial.println("Servo a");
          moveServo(servo_a, currentAngle, x);
        }else
        if(message.endsWith("b")) {
          Serial.println("Servo b");
          moveServo(servo_b, currentAngle, y);
        }else
        if(message.endsWith("c")) {
          Serial.println("Servo c");
          moveServo(servo_c, currentAngle, z);
        }
        
        message="";
      }
    }
  }
}

void moveServo(Servo &servo, int &currentAngle, int &oldAngle){

  Serial.printf("%d to %d\n",oldAngle, currentAngle);
  
  if (currentAngle > oldAngle) {
    for (int i = oldAngle; i < currentAngle; i++) {
      servo.write(i);
      delay(10);
    }
  } else {
    for (int i = oldAngle; i > currentAngle; i--) {
      servo.write(i);
      delay(10);
    }
  }
  oldAngle = currentAngle;
}
