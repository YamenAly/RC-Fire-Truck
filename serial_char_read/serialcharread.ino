void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0){
    char c = Serial.read();
    if(c == "F"){
      fire = 1;
    }else{
      fire = 0;
    }

    while(Serial.available()>0) Serial.read();
  }

  //deal with fire == 1
}
