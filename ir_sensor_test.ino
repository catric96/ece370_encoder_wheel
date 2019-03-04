int LED = 13; // Use the onboard Uno LED
int isObstaclePin = 12;  // This is our input pin
int fwdpin = 10; // pins designated for backwards and forward motion
int bckwdpin = 9;
int isObstacle = LOW;  // LOW MEANS OBSTACLE PRESENT
int countStates = 0;
int countTick = 0;
long rotationStartT = 0;
long rotationEndT = 0;
long totalTime = 0;
float ticksPerS = 0.0;
int numIn;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(fwdpin, OUTPUT);
  pinMode(bckwdpin, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  Serial.begin(9600);
  }
void loop() {
  if(countTick == 0){
    rotationStartT = (millis());
  }
  if(Serial.available() > 0){
    numIn = Serial.parseInt();
    Serial.print("set speed to: ");
    Serial.println(numIn);
    runMotors(numIn);
  }
  isObstacle=digitalRead(isObstaclePin);
  if (isObstacle){
    //Serial.println("clear");
    //countStates++;
    digitalWrite(LED, LOW);
  }
  else{
    //Serial.println("OBSTACLE!!, OBSTACLE!!");
    countStates++;
    digitalWrite(LED, HIGH);
  }
  if(countStates == 2){
    countTick++;
    countStates = 0;
  }
  if(countTick == 75){
    countTick = 0;
    //Serial.println(rotationStartT);
    rotationEndT = (millis());
    totalTime = (rotationEndT - rotationStartT);
    Serial.println(totalTime);
    ticksPerS = ((1.0)/((float)totalTime))*1000;
    //Serial.print(ticksPerS);
    Serial.print("System running at ");
    Serial.print(ticksPerS, 6);
    Serial.println(" ticks per s");
  }
  delay(5);
}



void runMotors(int num){ // takes a num input from the serial, dataInput, converts to integer, and then sets direction and speed
  // 0 - 4 is backwards, with 4 being slowest
  // 5 is stop
  // 6 - 9 is forwards, with 6 being slowest
  
  int s; //speed of motor //integer holding value of direction and speed
 // if(hasNewData == true){
//    num = atoi(dataInput); // converts the char serial input to integer
 
  if((num <= 4)&&(num >= 0)){ //go backwards!
     // delay(100);
      analogWrite(fwdpin, 127); // block the other set of mosfets
    //  delay(10);
      s = map(num, 0, 4, 255, 220); // map the input number to a pwm output range to represent a few different duty cycles
      analogWrite(bckwdpin, s); // go backwards at desired speed
      delay(100);
      Serial.print("going backwards at ... ");
      Serial.println(s);
  }
  else if(num == 5){ // stop both motors!
     // delay(100);
      analogWrite(bckwdpin, 127); // block both mosfets, no voltage will get through at all!
      analogWrite(fwdpin, 127);
      delay(100);
      Serial.println("stopping.. ");
  }
  else if((num <= 9)&&(num >= 6)){
    //  delay(100);
      analogWrite(bckwdpin,127); // stop the other set of mosfets
   //   delay(10);
      s = map(num, 6, 9, 220, 255); // map the input number to a pwm output range to represent a few different duty cycles
      analogWrite(fwdpin, s); // go forward at desired speed
      delay(100);
      Serial.print("going forwards at ... ");
      Serial.println(s);
  }
  //  hasNewData = false; // set the flag to false so we dont send the same commands over and over again and so we can wait for a new input
  //}
}
