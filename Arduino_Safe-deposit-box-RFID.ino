//RFID Module define
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//Servo define
#include <Servo.h>
Servo myservo1;
Servo myservo2;
int pos = 0;

//LCD define
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

//LED and buzzer define
int green_light_pin = A1;
int red_light_pin = A2;
int blue_light_pin = A3;
int buzzer = 8;

void setup()
{
  lcd.init();               // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Please, insert your card");

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  // initialize the servo
  myservo1.attach(7);
  myservo2.attach(6);

  // initialize position servo
  myservo1.write(pos);
  myservo2.write(pos);

  // initialize the RGB led & buzzer
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  RGB_color(0, 0, 255); // Blue
}
void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;

  //print
  lcd.setCursor(0, 0);
  lcd.print("Please, insert your card");

  RGB_color(0, 0, 255); //Blue

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "8A B8 74 82") //change here the UID of the card/cards that you want to give access
  {
    //lcd print
    Serial.println("Authorized access");
    lcd.setCursor(0, 0);
    lcd.print("Authorized access");
    Serial.println();

    //led light on
    RGB_color(0, 255, 0); // Green

    //Open the door
    sweepForward(150, 1);
    sweepForward(90, 2);

    delay(5000);

    //Alert alarm
    buzzerBlink(5, 200);

    //Close the door
    sweepReverse(150, 1);
    sweepReverse(90, 2);
    delay(2000);
    RGB_color(0, 0, 0);
    delay(1000);
  }

  else   {
    //lcd print
    Serial.println(" Access denied");
    lcd.setCursor(0, 0);
    lcd.print("Access denied");

    //led light on
    RGB_color(255, 0, 0); // Red

    //buzzer on
    buzzerBlink(5, 100);

    //delay(3000);
  }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value) {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void buzzerBlink(int numTime, int frequency) {
  for (int i = 0; i < numTime; i++) {
    digitalWrite(buzzer, HIGH);
    delay(frequency);
    digitalWrite(buzzer, LOW);
    delay(frequency);
  }
}

void sweepReverse(int angle, int servoNum) {
  switch (servoNum) {
    case 1:
      for (pos = angle; pos >= 0; pos -= 1) {
        myservo1.write(pos);
        delay(15);
      }
      break;
    case 2:
      for (pos = angle; pos >= 0; pos -= 1) {
        myservo2.write(pos);
        delay(15);
      }
      break;
  }
}

void sweepForward(int angle, int servoNum) {
  switch (servoNum) {
    case 1:
      for (pos = 0; pos <= angle; pos += 1) {
        myservo1.write(pos);
        delay(15);
      }
      break;
    case 2:
      for (pos = 0; pos <= angle; pos += 1) {
        myservo2.write(pos);
        delay(15);
      }
      break;
  }
}
