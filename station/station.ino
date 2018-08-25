/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst
 *  Works with: accesspoint_bare_01.ino
 */

#include <ESP8266WiFi.h>

char ssid[] = "WIFI_AP";          // SSID of your AP
char pass[] = "123456789";          // password of your AP

IPAddress server(192,168,4,15);     // IP address of the AP
WiFiClient client;

String getString();

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  
  Serial.println("Connecting to the AccessPpoint");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(WiFi.status());
    delay(500);
  }
  
  Serial.println("\nConnected.");
  Serial.println("LocalIP : " + WiFi.localIP().toString());
  Serial.println("MAC     : " + WiFi.macAddress());
  Serial.println("Gateway : " + WiFi.gatewayIP().toString());
  Serial.println("AP MAC  : " + WiFi.BSSIDstr());

  client.connect(server, 80);
}

String msg;
char c;

void loop() {
  if(client){
    msg = getString();
    if(msg.length() > 0){
      Serial.println("Message : " + msg);
      client.print(msg + "\n");
      client.flush();
    }
  }else{
    client.connect(server, 80);
  }
}

String getString(){
  String result = "";

  if(!Serial.available()){
    delay(100);
  }
  while(Serial.available()){
    do{
      c = Serial.read();
      result += c;
    }while(Serial.available());
    delay(10);
  }
  return result;
}
