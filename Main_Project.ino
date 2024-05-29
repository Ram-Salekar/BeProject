#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#include <Servo.h>

// Constants for potentiometer
#define POT_PIN A0
#define POT_MIN_VALUE 0
#define POT_MAX_VALUE 1023

// Constants for servo motors
#define SERVO1_PIN 52
#define SERVO2_PIN 53
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

Servo servo1;  // Create an instance of the first servo motor
Servo servo2;  // Create an instance of the second servo motor

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(19 , 18);

#else

#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Define keypad pins and size
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define LCD pins and initialize
LiquidCrystal lcd(12, 11, 25, 22, 23, 24);

// Define relay pin
const int r = 26;

// Define password
const char password[] = "1234";




void setup()

{

 servo1.attach(SERVO1_PIN);  // Attach the first servo motor to the corresponding pin
  servo2.attach(SERVO2_PIN);  // Attach the second servo motor to the corresponding pin
 

  finger.begin(57600);
    while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  delay(5);

  if (finger.verifyPassword()) 

  {

  } 

  else 

  {

    while (1) { delay(1); }

  }

  

  pinMode(r, OUTPUT);
  digitalWrite(r, HIGH);  

   lcd.begin(16, 2);
  lcd.print("Enter password:");

 
  

}
void loop() {
  static char enteredPassword[5]; // Store entered password (4 digits + null terminator)
  static int passwordIndex = 0; // Index for entering password

  char key = keypad.getKey(); // Read keypad input

  if (key != NO_KEY) {
    // A key was pressed
    lcd.print('*'); // Print asterisk instead of entered digit
    enteredPassword[passwordIndex++] = key; // Store entered digit in password
    if (passwordIndex == 4) {
      // Entered full password, compare to correct password
      if (strcmp(enteredPassword, password) == 0) {
        // Correct password entered
        lcd.clear();
        lcd.print("UNLOCK");
        

        digitalWrite(r, LOW); // Activate relay
        
      } 
      
        
      
      else {
        // Incorrect password entered
        lcd.clear();
        lcd.print("LOCK");
      }
      // Reset password index and clear entered password
      passwordIndex = 0;
      enteredPassword[0] = '\0';
    }
  }
  
     if  ( getFingerPrint() != -1)

  {

    
 lcd.clear();


        lcd.print("UNLOCK");
        

        digitalWrite(r, LOW); // Activate relay
       

  }
   int potValue = analogRead(POT_PIN);
  
  // Map the potentiometer value to the servo angle range
  int angle = map(potValue, POT_MIN_VALUE, POT_MAX_VALUE, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  
  // Move the first servo to the calculated angle
  servo1.write(angle);
  
  // Move the second servo to the calculated angle (can be adjusted based on your setup)
  servo2.write(angle);
  
  // Wait for a brief moment before taking the next reading
  delay(100);
   
 


 
 
}
int getFingerPrint() 

{

  int p = finger.getImage();

  if (p != FINGERPRINT_OK)  return -1;



  p = finger.image2Tz();

  if (p != FINGERPRINT_OK)  return -1;



  p = finger.fingerFastSearch();

  if (p != FINGERPRINT_OK)  return -1;



  return finger.fingerID;

}
