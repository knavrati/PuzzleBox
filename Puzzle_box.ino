#define BLYNK_TEMPLATE_ID "TMPLG8d-ct4S"
#define BLYNK_DEVICE_NAME "Puzzle Box"
#define BLYNK_AUTH_TOKEN "fvDbQWDocpZ8qXm8vsRTVGN8LaT75_na"

#define BLYNK_PRINT SerialUSB
#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>
#include <Servo.h>


// RGB LED pins
int redPin = 6;
int greenPin = 8;
int bluePin = 7;

char ssid[] = "Navratil";
char password[] = "Rr5170335!";
char auth[] = BLYNK_AUTH_TOKEN;

 // LCD screen pins
 const int rs = 12,
          en = 11,
          d4 = 2,
          d5 = 3,
          d6 = 4,
          d7 = 5;

bool start = true;

// Variables to store the combination value

int SliderValueOne = 0;
int SliderValueTwo = 0;
int SliderValueThree = 7;

int pos = 0;    // variable to store the servo position
Servo myservo;  // create servo object to control a servo


// Blynk functions to retrive values
BLYNK_WRITE(V1) {
  SliderValueOne = param.asInt(); // assigning incoming value from pin V1 to a variable

}
BLYNK_WRITE(V2) {
  SliderValueTwo = param.asInt(); // assigning incoming value from pin V2 to a variable

}
BLYNK_WRITE(V3) {
  SliderValueThree = param.asInt(); // assigning incoming value from pin V3 to a variable
}


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  
  
  analogWrite(A3, 0); // set the brightness of the LCD screen to the maximum value
  Serial.begin(9600); 
  lcd.begin(16, 2); // begin LCD screen with 16 columns and 2 rows
  SerialUSB.begin(115200);
  Blynk.begin(auth, ssid, password); // start Blynk functionalities
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos); // set the servo in position 0

}

void loop() {
  Blynk.run();
  // Variambles to temporarily store the combination
  int Temp_Slider_One_value = SliderValueOne;
  int Temp_Slider_Two_value = SliderValueTwo;
  int Temp_Slider_Three_value = SliderValueThree;
  
  Blynk.run(); // poll new combination values from the online app
  
  // check if combination values are changed and print them on the console
  if(Temp_Slider_One_value != SliderValueOne || Temp_Slider_Two_value != SliderValueTwo || Temp_Slider_Three_value != SliderValueThree){
    Serial.print("New combination: ");
    Serial.print(SliderValueOne);
    Serial.print(" ");
    Serial.print(SliderValueTwo);
    Serial.print(" ");
    Serial.println(SliderValueThree);
  } 
  
  int PotOne = map(analogRead(A0), 100, 1023, 0, 9);
  int PotTwo = map(analogRead(A1), 100, 1023, 0, 9);
  int PotThree = map(analogRead(A2), 100, 1023, 0, 9);
  lcd.setCursor(0, 0);
  lcd.print(PotOne);
  lcd.setCursor(2, 0);
  lcd.print(PotTwo);
  lcd.setCursor(4, 0);
  lcd.print(PotThree);


  if (start) {
    giveColorFeedback(PotOne, PotTwo, PotThree);
    if (PotOne == SliderValueOne && PotTwo == SliderValueTwo && PotThree == SliderValueThree) {
      
      open_the_box();
      blinkGreenLed();
     lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("Congratulations!");
      delay(5000);
      lcd.setCursor(0,0);
      lcd.print("                ");
      start = false;
    }
  }
  
  if(!start) {
    if(PotOne == 0 && PotTwo == 0 && PotThree == 0){
      close_the_box();
      start = true;
    }
  }

}

// Give feedback based on how close the potentiometer are to the combination value 
// The more it's close the warmer is the color of the LED
void giveColorFeedback(int PotOne, int PotTwo, int PotThree) {

  if (abs(PotOne - SliderValueOne) <= 1 && abs(PotTwo - SliderValueTwo) <= 1 && abs(PotThree - SliderValueThree) <= 1 ) {
    // Red
    setColor(0,0,255);
  }
  else   if (abs(PotOne - SliderValueOne) <= 3 && abs(PotTwo - SliderValueTwo) <= 3 && abs(PotThree - SliderValueThree) <= 3 ) {
    // yellow
    setColor(255, 255, 0);
  }
    else   if (abs(PotOne - SliderValueOne) <= 4 && abs(PotTwo - SliderValueTwo) <= 4 && abs(PotThree - SliderValueThree) <= 4 ) {
    // aqua
    setColor(0, 255, 255);  
  }
  else {
    // blue
    setColor(255, 0, 0);
  }
}

void blinkGreenLed() {
  for (int a = 0; a < 2; a++) {
    for (int b = 0; b <= 255; b += 5) {
      setColor(0, b, 0); 
      delay(5);
    }
    for (int b = 255; b >= 0; b -= 5) {
      setColor(0, b, 0); 
      delay(5);
    }
  }
  for (int b = 0; b <= 255; b += 5) {
    setColor(0, b, 0); 
    delay(5);
  }
}

// Send RGB values to the LED pins
void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void open_the_box(){
      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void close_the_box(){
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
}
