#include <FlexiTimer2.h>

#include <Servo.h>

#include <LiquidCrystal_I2C.h>

int ruptPin = A2; // select the input pin for the interrupter
int val = 0; // variable to store the value coming from the sensor
int Swipeled = 22;
int level1 = 44;
int level2 = 46;
int level3 = 48;
int doorPin = 49;
int startbutton = 50;
int level1led = 24;
int level2led = 26;
int level3led = 28;
int startbuttonled = 30;
int latchPin = 12; //12;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 13; //13;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 11; //11;           // Pin connected to DS of 74HC595（Pin14）
int comPin[] = {
  7,
  6,
  5,
  4
};
int second = 0; // Define variable for starting time in seconds
int mins = 0; // variable for holding minutes
int secs = 0; // variable for holding seconds
int splitmins1 = 0; // variable for holding first digits of minutes
int splitmins2 = 0; // variable for holding second digits of minutes
int splitsecs1 = 0; // variable for holding first digits of seconds
int splitsecs2 = 0; // variable for holding second digits of seconds
int buzzer = 15;
int VRx = A7;
int VRy = A6;
int servoPin1 = 16;
int servoPin2 = 17;
int PinBall1 = 18;
int BallCount = 0;
int PinBall2 = 19;
int Liftservo = 34;
int xPosition = 0;
int yPosition = 0;
int mapX = 0;
int mapY = 0;
int L1 = 0, L2 = 0, L3 = 0, Stage = 0;
bool Ball = false;
int level = 0;
bool timer = false;
bool pirstate = false;
bool cardswipe = false;
int statelevel = 0;
bool levelS1 = false;
bool levelS2 = false;
bool levelS3 = false;
long time = 0; // the last time the output pin was toggled
long debounce = 200; // the debounce time, increase if the output flickers
bool startb = false;

int ledPin = 32; // choose the pin for the LED
int inputPin = 2; // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected
int valPIR = 0;

int PlayerCount = 0;

int highscoremin1 = 0;
int highscoresec1 = 0;

int highscoremin2 = 0;
int highscoresec2 = 0;

int highscoremin3 = 0;
int highscoresec3 = 0;


boolean timerSet = false;

byte num[] = {
  0x3F,
  0x06,
  0x5B,
  0x4F,
  0x66,
  0x6D,
  0x7D,
  0x07,
  0x7F,
  0x6F,
  0x88,
  0x83,
  0xc6,
  0xa1,
  0x86,
  0x8e
};
byte myArray[] = {
  0x5B,
  0x3F,
  0x3F,
  0x3F
}; //array for representing led characters, inititally 4500

Servo myservo, myservo2, myservo3, myservo4, myservo5;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(PinBall1, OUTPUT);
  pinMode(PinBall2, OUTPUT);
  pinMode(Liftservo, OUTPUT);
  myservo.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(PinBall1);
  myservo4.attach(PinBall2);
  myservo4.attach(Liftservo);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(servoPin1, OUTPUT);
  pinMode(servoPin2, OUTPUT);
  pinMode(Swipeled, OUTPUT);
  pinMode(level1, INPUT);
  pinMode(level2, INPUT);
  pinMode(level3, INPUT);
  pinMode(startbutton, INPUT);
  pinMode(level1led, OUTPUT);
  pinMode(level2led, OUTPUT);
  pinMode(level3led, OUTPUT);
  pinMode(startbuttonled, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //pinMode(doorPin, INPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(comPin[i], OUTPUT);
  }

  pinMode(ledPin, OUTPUT); // declare LED as output
  pinMode(inputPin, INPUT); // set door lock monitor pin to high

  digitalWrite(doorPin, HIGH); // set door lock monitor pin to high
  FlexiTimer2::set(1000, timerInt); // configure the timer and interrupt function
  FlexiTimer2::start();
}
void loop() {
 val = analogRead(ruptPin);
 Serial.print(val);
if(val < 10 || cardswipe == true){
  digitalWrite(Swipeled, HIGH);
  if(statelevel == 0)
  {
    LevelSelect();
  } else if(statelevel > 0 && timerSet == false) {
   startb = digitalRead(startbutton);
   digitalWrite(startbuttonled,HIGH);
    if (startb == true && timerSet == false) {
     timerSet = true;
     PlayerCount++;
      BallCount = 2;
      lcd.setCursor(0, 1);
      lcd.print("Game Started");
     Serial.print("\n") ;
     Serial.print("PlayerCount");
     Serial.print(PlayerCount);
     if(statelevel == 1 || statelevel == 2){
     second = 90;
     } else {
      second = 45;
     }
     Serial.print("\n") ;
     Serial.print("Level NO");
     Serial.print(statelevel);
     if (statelevel == 1){
      myservo3.write(120);
     } else if (statelevel == 2){
      myservo3.write(120);
      myservo4.write(120);
      myservo5.write(120);
     }else if(statelevel == 3){ 
      myservo3.write(120);
      myservo4.write(120);
      myservo5.write(120);
      }else { 
      myservo3.write(10);
      myservo4.write(10);
      myservo5.write(10);
      }
     
    }

  }
  else if(statelevel > 0 && timerSet == true) {
    // showArray();
   
    //pirstate = digitalRead(inputPin);
    xPosition = analogRead(VRx);
    yPosition = analogRead(VRy);
    valPIR = digitalRead(inputPin);
    mapX = map(xPosition, 0, 1023, 0, 180);
    mapY = map(yPosition, 0, 1023, 0, 180);
    //Serial.print(mapX);
   Serial.print(valPIR);
    myservo.write(mapX);
    myservo2.write(mapY);
    
    showArray();
   if(statelevel == 1){
   if (second == 1 || valPIR == HIGH) {
      valPIR = LOW;
      timerSet = false; 
      digitalWrite(ledPin, HIGH);
      startb = false;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Game Over 1");
      digitalWrite(buzzer, HIGH);
      delay(200);
      if (highscoremin1 <= mins) {
        if (highscoresec1 <= secs) {
          highscoresec1 = secs;
          highscoremin1 = mins;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print("High Score ");
      lcd.print(highscoremin1);
      lcd.print(":");
      lcd.print(highscoresec1);
      Serial.print("\n") ;
      Serial.print(" PIR pin = ");
      Serial.print(valPIR);
      digitalWrite(buzzer, LOW);
      digitalWrite(ledPin, LOW);
     statelevel = 0;
     delay(2500);
    cardswipe = false;
     digitalWrite(startbuttonled,LOW);
    lcd.clear();
    }}
    else if(statelevel == 2){
        Serial.print("\n");
        Serial.print(valPIR);
      if (valPIR == LOW){
        Ball = true;
        }
      if (valPIR == HIGH &&  BallCount != 0){
        if (Ball == true &&  BallCount != 0)
        { BallCount--;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Ball in : ");
          lcd.print(BallCount);
          Ball = false;
          }
        } 
        if( second == 1 ||BallCount == 0 ){     
      if (second == 1 || valPIR == HIGH) {
      valPIR = LOW;
      timerSet = false;
      digitalWrite(ledPin, HIGH);
      startb = false;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Game Over 2");
      digitalWrite(buzzer, HIGH);
      delay(200);
      if (highscoremin2 <= mins) {
        if (highscoresec2 <= secs) {
          highscoresec2 = secs;
          highscoremin2 = mins;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print("High Score ");
      lcd.print(highscoremin2);
      lcd.print(":");
      lcd.print(highscoresec2);
      Serial.print("\n") ;
      Serial.print(" PIR pin = ");
      Serial.print(valPIR);
      digitalWrite(buzzer, LOW);
      digitalWrite(ledPin, LOW);
     statelevel = 0;
     delay(2500);
    cardswipe = false;
    lcd.clear();
    digitalWrite(startbuttonled,LOW);
      }
        }
        }
      else if (statelevel == 3){
          Serial.print("\n");
        Serial.print(valPIR);
      if (valPIR == LOW){
        Ball = true;
        }
      if (valPIR == HIGH &&  BallCount != 0){
        if (Ball == true &&  BallCount != 0)
        { BallCount--;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Ball in : ");
          lcd.print(BallCount);
          Ball = false;
          }
        } 
         if( second == 1 ||BallCount == 0 ){ 
      if (second == 1 || valPIR == HIGH) {
      valPIR = LOW;
      timerSet = false;
      digitalWrite(ledPin, HIGH);
      startb = false;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Game Over 3");
      digitalWrite(buzzer, HIGH);
      delay(200);
      if (highscoremin3 <= mins) {
        if (highscoresec3 <= secs) {
          highscoresec3 = secs;
          highscoremin3 = mins;
        }
      }
      lcd.setCursor(0, 1);
      lcd.print("High Score ");
      lcd.print(highscoremin3);
      lcd.print(":");
      lcd.print(highscoresec3);
      Serial.print("\n") ;
      Serial.print(" PIR pin = ");
      Serial.print(valPIR);
      digitalWrite(buzzer, LOW);
      digitalWrite(ledPin, LOW);
     statelevel = 0;
     delay(2500);
    cardswipe = false;
    digitalWrite(startbuttonled,LOW);
    lcd.clear();
      }
      }
  }}else {
    Serial.print("Card Not Swiped");
    lcd.setCursor(1, 0);
    lcd.print("in this loop ");
    lcd.setCursor(0, 1);
    lcd.print(" to Begin "); // put your main code here, to run repeatedly:
    digitalWrite(Swipeled, LOW);
  }  
} else {
  
    
    lcd.setCursor(0, 0);
    lcd.print("Player Count");
    lcd.print(":");
    lcd.print(PlayerCount);
     lcd.setCursor(0, 1);
      lcd.print("High Score ");
      lcd.print(highscoremin3);
      lcd.print(":");
      lcd.print(highscoresec3);// put your main code here, to run repeatedly:
    digitalWrite(Swipeled, LOW);
  }
}
//Serial.print(cardswipe);
  // main();
void timerInt() {
  //if(digitalRead(doorPin)==LOW){  // while the switch at pin 3 is closed, this will run
  if (second > 0) {
  second--; // second minus 1
  
  }
  mins = int(second / 60); // minutes as 45, 44, 43…
  secs = int(second % 60); // seconds of every minute
  splitmins1 = int(mins / 10); // first digit of minutes
  splitmins2 = int(mins % 10); // second digit of minutes
  splitsecs1 = int(secs / 10); // first digit of seconds
  splitsecs2 = int(secs % 10); // second digit of seconds
  myArray[0] = num[splitmins1]; // get first digit hexcode from num array
  myArray[1] = num[splitmins2]; // get second digit hexcode from num array
  myArray[2] = num[splitsecs1]; // get third digit hexcode from num array
  myArray[3] = num[splitsecs2]; // get fourth digit hexcode from num array
  
  //}
  //else {                          // switch opens, the clock shows the time left at opening
  //  delay(10);
  //
      digitalWrite(level1led, LOW);
      digitalWrite(level2led, LOW);
      digitalWrite(level3led, LOW);
      valPIR = digitalRead(inputPin);
      Serial.print("\n") ;
      Serial.print(valPIR);
      Serial.print("\n") ;
      Serial.print(mins) ;
      Serial.print(" : ") ;
      Serial.print(secs) ;
   
  
}

void showArray() {
  for (int i = 0; i < 4; i++) {
    // Select a single 7-segment display
    chooseCommon(i);
    // Send data to 74HC595
    writeData(myArray[i]);
   
    //
    // Clear the display content
    writeData(0xff);
  }
}

void chooseCommon(byte com) {
  // Close all single 7-segment display
  for (int i = 0; i < 4; i++) {
    digitalWrite(comPin[i], HIGH);
  }
  // Open the selected single 7-segment display
  digitalWrite(comPin[com], LOW);
}

void writeData(int value) {
  // Make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // Make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

int LevelSelect() {
   Serial.print("Select Level");
   myservo4.write(0);
       delay(15);
    myservo3.write(0);
    lcd.clear();
     // lcd.setCursor(1, 1);
      //lcd.print("Select Level");
      cardswipe = true;

  while (1) {
    
    levelS2 = digitalRead(level2);
    levelS3 = digitalRead(level3);
    levelS1 = digitalRead(level1);
    Serial.print("\n Level1 : ");
    Serial.print(levelS1);
    Serial.print("\n Level2 :   ");
    Serial.print(levelS2);
    Serial.print("\n Level3 :     ");
    Serial.print(levelS3);
    if (levelS1 == true ) {
      statelevel = 1;
      delay(15);
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print("Level 1 Selected");
      lcd.setCursor(0, 1);
      lcd.print("Press START");
      digitalWrite(level1led, HIGH);
       Serial.print("Level 1 Selected");
      break;
    } else if (levelS2 == true && levelS1 == false && levelS3 == false) {
      statelevel = 2;
      delay(15);
      //do level 2 setting
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print("Level 2 Selected");
      lcd.setCursor(0, 1);
      lcd.print("Press START");
      digitalWrite(level2led, HIGH);
      Serial.print("Level 2 Selected");
      break;
    } else if (levelS3 == true && levelS1 == false && levelS2 == false) {
      statelevel = 3;
      lcd.setCursor(0,0);
       lcd.clear();
      lcd.print("Level 3 Selected");
      lcd.setCursor(0, 1);
      lcd.print("Press START");
      digitalWrite(level3led, HIGH);
      Serial.print("Level 3 Selected");
      break;
    } else {
      lcd.setCursor(1, 0);
      lcd.print("Select Level");
     
      // do nothing 
    }
  }

  return statelevel;
}

void startstop() {
    
//    if(timerSet) {
//      PlayerCount++;
//      Serial.print("/n PlayerCount =");
//      Serial.print(PlayerCount);
//      break;
//    }
  
 
}