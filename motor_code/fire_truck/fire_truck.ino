#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#define INCLUDE_LEDCONTROL_MODULE
#include <Dabble.h>

//Motor A (Right Motor)
#define enA 11 //11 because, pin 3 is for the bluetooth module
#define in1 A4 //Connected to one of the analog pins (A4), and force the analog pin to work digitally
#define in2 4

//Motor B (Left Motor)
#define enB 9
#define in3 8
#define in4 7

//Ultrasonic Sensor
#define trigPin 6
#define echoPin 12
float distance_cm, duration;

//Water-level Sensor
#define POWER_PIN  13
#define SIGNAL_PIN A5

int value = 0; // variable to store the sensor value

//Flame sensor
#define flame_sensor A0
#define buzzer 10
bool flame_detected = HIGH;

#define serialTime 200
long lastSerial = 0;
bool fire = 0;

//Water-pump
#define pump 5

//Raspberry Pi Communication
String charrcv;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  pinMode(buzzer, OUTPUT);
  pinMode(flame_sensor, INPUT);

  Dabble.begin(9600, 2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:

  int rightMotorSpeed=0;
  int leftMotorSpeed=0;
  Dabble.processInput();

  
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);


  distance_cm = (0.017 *duration);

  if(distance_cm >= 15.0){
    if (GamePad.isUpPressed())
    {
      Serial.println("up");
      rightMotorSpeed = 255;
      leftMotorSpeed = 255;
    }

    if (GamePad.isDownPressed())
    {
      Serial.println("down");
      rightMotorSpeed = -255;
      leftMotorSpeed = -255;
    }

    if (GamePad.isLeftPressed())
    {
      Serial.println("left");
      rightMotorSpeed = 255;
      leftMotorSpeed = 0;
    }

    if (GamePad.isRightPressed())
    {
      Serial.println("right");
      rightMotorSpeed = 0;
      leftMotorSpeed = 255;
    }

    rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }
  else{
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    if (GamePad.isDownPressed())
    {
      rightMotorSpeed = -255;
      leftMotorSpeed = -255;
    }

    rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }

  //Water-level Sensor
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);

  //Receive data from Raspberry Pi
  // if(Serial.available()){
  //   charrcv = Serial.readStringUntil('\n');
  //   charrcv.trim();
  //   // if(charrcv.equals("test")){
  //   //   digitalWrite(buzzer, HIGH);
  //   //   delay(2000);
  //   //   digitalWrite(buzzer, LOW);
  //   //   delay(1000);
  //   //   digitalWrite(buzzer, HIGH);
  //   //   delay(2000);
  //   //   digitalWrite(buzzer, LOW);
  //   // }
  // }
  
  if(Serial.available()>0){
    char c = Serial.read(); 
      Serial.println(c);
      fire = 1;
    Serial.println("Flame detected...! + RBPi");
    digitalWrite(buzzer, HIGH);
    analogWrite(pump, 220);
     
      lastSerial = millis();
    while(Serial.available()>0) Serial.read();
  }

  if((millis() - lastSerial)>serialTime){
    fire = 0;
  }

  
  //Flame Sensor
  flame_detected = digitalRead(flame_sensor);


  if(value < 20){
    //digitalWrite(buzzer, HIGH);
    //delay(300);
    //digitalWrite(buzzer, LOW);
    //delay(200);
    digitalWrite(buzzer, HIGH);
   // delay(300);
    digitalWrite(buzzer, LOW);
    Serial.println("Please refill water tank!");
    //delay(200);
  } 
  else {
      if (flame_detected == LOW){
      Serial.println("Hellllllllllllllllllllllllllllllllo");
      Serial.println("Flame detected...! ");
      digitalWrite(buzzer, HIGH);
      analogWrite(pump, 220);
      delay(1000);
      analogWrite(pump, 0);

      if (GamePad.isSquarePressed()){
        analogWrite(pump, 220);
      }
      else{
        analogWrite(pump, 0);
      }
    }
    else
    {
      Serial.println("No flame detected");
      digitalWrite(buzzer, LOW);
      if (GamePad.isSquarePressed()){
        analogWrite(pump, 220);
      }
      else{
        analogWrite(pump, 0);
      }
    }
  }

  // Ultrasonic Sensor
  // Serial.print("distance: ");
  // Serial.print(distance_cm);
  // Serial.println(" cm");

  delay(500);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed >= 0)
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);    
  }
  else if (rightMotorSpeed < 0)
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);      
  }

  if (leftMotorSpeed >= 0)
  {
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);    
  }
  else if (leftMotorSpeed < 0)
  {
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);      
  }

  analogWrite(enA, abs(rightMotorSpeed));
  analogWrite(enB, abs(leftMotorSpeed));
  Serial.println("lezzgo");    
}
