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
  Serial.begin(9600);
  
  servo_a.attach(3);
  servo_b.attach(5);
  servo_c.attach(9);
  
  servo_a.write(x);
  servo_b.write(y);
  servo_c.write(z);
}

void loop() {
  if (Serial.available()){
    char c = Serial.read();
    if (c != '/') {
      message += c;
    } else {
      if (message.length() > 1) {
        
        int currentAngle = message.toInt();
        Serial.print("move to angle : ");
        Serial.println(currentAngle);

        if(message.endsWith("a")) {
          moveServo(servo_a, currentAngle, x);
        }else
        if(message.endsWith("b")) {
          moveServo(servo_a, currentAngle, y);
        }else
        if(message.endsWith("c")) {
          moveServo(servo_a, currentAngle, z);
        }
        
        message="";
      }
    }
  }
}

void moveServo(Servo &servo, int &currentAngle, int &oldAngle){
  if (currentAngle > oldAngle) {
    for (int i = oldAngle; i < currentAngle; i++) {
      servo.write(i);
      delay(10);
    }
  } else {
    for (int i = oldAngle; i < currentAngle; i--) {
      servo.write(i);
      delay(30);
    }
  }
  oldAngle = currentAngle;
}
