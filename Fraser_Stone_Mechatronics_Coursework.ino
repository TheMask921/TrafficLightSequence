//#-----Libraries-----#//
#include <Servo.h>
Servo myservo;

//#-----Variables-----#//
//Setting variable types and/or pins for all variables
int Main[] = { 4, 3, 2, 5, 6, 7, 9, 10, 11 };
int LDRPinCross1 = A4;
int LDRPinCross2 = A5;
const int pushButton = 13;
int pos = 0;
int ServoPin = 8;
float Cbutt1 = 0;
float Cbutt2 = 0;
int buttonState = 0;

//#-----Setup-----#//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(Main[i], OUTPUT);
  }
  pinMode(LDRPinCross1, INPUT);
  pinMode(LDRPinCross2, INPUT);
  pinMode(pushButton, INPUT);
  myservo.attach(ServoPin);
}

//#-----Functions-----#//
int crossing() {
  digitalWrite(Main[3], LOW);   //Turns crossing red light off
  digitalWrite(Main[4], HIGH);  //Turns crossing green light on
  delay(5000);                  //crossing delay
  digitalWrite(Main[4], LOW);   //Turns crossing green light off
  digitalWrite(Main[3], HIGH);  //Turns crossing red light on
}

int GateLightOn() {
  digitalWrite(Main[5], HIGH);  //Turns Gate Warning Light on
}

int GateLightOff() {
  digitalWrite(Main[5], LOW);  //Turns Gate Warning Light off
}

int GateOpen() {
  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
  digitalWrite(Main[7], HIGH);  //Turns on amber while red is still on
  delay(2000);
  digitalWrite(Main[6], LOW);   //Turns off red
  digitalWrite(Main[7], LOW);   //Turns off amber
  digitalWrite(Main[8], HIGH);  //Turns on green
}

int GateClose() {
  digitalWrite(Main[8], LOW);   //Turns off green
  digitalWrite(Main[7], HIGH);  //Turns on amber
  delay(1000);
  digitalWrite(Main[7], LOW);            //Turns off amber
  digitalWrite(Main[6], HIGH);           //Turns on red
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}


int StartMainTraffic() {
  digitalWrite(Main[1], HIGH);  //Turns on amber while red is still on
  delay(2000);
  digitalWrite(Main[2], LOW);   //Turns off red
  digitalWrite(Main[1], LOW);   //Turns off amber
  digitalWrite(Main[0], HIGH);  //Turns on green
}

int StopMainTraffic() {
  digitalWrite(Main[0], LOW);   //Turns off green
  digitalWrite(Main[1], HIGH);  //Turns on amber
  delay(1000);
  digitalWrite(Main[1], LOW);   //Turns off amber
  digitalWrite(Main[2], HIGH);  //Turns on red
}

//#-----Main Loop-----#//
void loop() {
  // put your main code here, to run repeatedly:
  int LDRC1Status = analogRead(LDRPinCross1);  //Status of Crossing1 LDR
  int LDRC2Status = analogRead(LDRPinCross2);  //Status of Crossing2 LDR
  int buttonState = digitalRead(pushButton);   //State of pushButton
  Cbutt1 = (5. / 1023.) * LDRC1Status;  //Makes Crossing1 LDR status more readable
  Cbutt2 = (5. / 1023.) * LDRC2Status;  //Makes Crossing2 LDR status more readable
  digitalWrite(Main[0], HIGH);          //Turns Main Road green light on
  digitalWrite(Main[6], HIGH);          //Turns Gate red light on
  digitalWrite(Main[3], HIGH);          //Turns Crossing red light on

  if (buttonState == 0 && Cbutt1 < 5 && Cbutt2 < 5) {  //If Gate = 0 and Crossing = 0
    digitalWrite(Main[0], HIGH);
    Serial.print("ZERO, ");
  } else if (buttonState == 0 && (Cbutt2 >= 5|| Cbutt1 >= 5)) {  //If Gate = 0 and Crossing = 1
    Serial.print("ONE");
    StopMainTraffic();
    crossing();
    delay(2000);
    StartMainTraffic();
  } else if (buttonState == 1 && (Cbutt2 <= 5 || Cbutt1 <= 5)) {  //If Gate = 1 and Crossing = 0
    StopMainTraffic();
    GateLightOn();
    delay(2000);
    GateOpen();
    GateLightOff();
    delay(5000);
    GateLightOn();
    delay(2000);
    GateClose();
    GateLightOff();
    StartMainTraffic();
  } else if (buttonState == 1 && (Cbutt2 >= 5 || Cbutt1 >= 5)) {  //If Gate = 1 and Crossing = 1
    StopMainTraffic();
    crossing();
    delay(2000);
    GateLightOn();
    delay(2000);
    GateOpen();
    GateLightOff();
    delay(5000);
    GateLightOn();
    delay(2000);
    GateClose();
    GateLightOff();
    StartMainTraffic();
  }
  //#-----Serial Print-----#//
  Serial.print("Cbutton1Val: ");
  Serial.print(Cbutt1);  //Prints status of Crossing LDR
  Serial.print(", ");
  Serial.print("Cbutton2Val: ");
  Serial.print(Cbutt2);
  Serial.print(", ");
  Serial.print("GbuttonVal: ");
  Serial.print(buttonState);  //Prints status of Gate LDR
  Serial.println(" ");
}