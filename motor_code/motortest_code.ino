//Motor A
#define enA 3
#define in1 2
#define in2 4

//Motor B
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
#define flame_sensor 5
#define buzzer 10
int flame_detected = HIGH;

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
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(enA, 90);
  analogWrite(enB, 90);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);


  distance_cm = (0.017 *duration);

  if(distance_cm >= 20){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  else{
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }

  //Water-level Sensor
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);

  //Flame Sensor
  flame_detected = digitalRead(flame_sensor);
  
  if (flame_detected == LOW)
  {
    Serial.println("Flame detected...! ");
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    Serial.println("No flame detected");
    digitalWrite(buzzer, LOW);
  }

  // Ultrasonic Sensor
  // Serial.print("distance: ");
  // Serial.print(distance_cm);
  // Serial.println(" cm");

  delay(500);
}
