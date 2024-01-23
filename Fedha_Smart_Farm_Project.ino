
#include <LiquidCrystal_I2C.h>
int relay_pin = 12;
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define dry_threshold 500
#define wet_threshold 400
#define IN1 6
#define IN2 5
unsigned long valveOpenTime = 0;
unsigned long valveCloseTime = 0;
const unsigned long valveRunningTime = 12000;
bool valveOpen = false;

void setup()
{
  Serial.begin(115200);
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
  // Print a message to the LCD
  lcd.setCursor(0, 0); // Set the cursor to the top left
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

}
void loop() {
   // put your main code here, to run repeatedly
  lcd.clear();
  controlLight();
  controlValve();
  delay(2000);
  
}
void openValve()
{  
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   valveOpenTime = millis();

  if(millis() - valveOpenTime >= valveRunningTime)
  {
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   valveOpen =  true;
  }
}
void closeValve()
{ 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  valveCloseTime = millis();

  if( valveOpen && millis()-valveCloseTime >= valveRunningTime)
  {

   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   valveOpen = false;
  }
   

}

void controlValve()
{

int val = analogRead(A0);
 Serial.print("Soil humidity is ");
 Serial.println(val);
   
lcd.setCursor(0, 0);
lcd.print("Soil humidity is ");
lcd.print(val);

    

if (val > dry_threshold && !valveOpen)
  {
Serial.println("the soil is dry");
lcd.setCursor(0, 1);
lcd.print("the soil is dry");

openValve();

  }
  else if (val < wet_threshold) 
  {
    
Serial.println("the soil is wet");
lcd.setCursor(0, 1);
lcd.print("the soil is wet");

closeValve();

  }
  else
  { 
Serial.println("the soil is perfect");
lcd.setCursor(0, 1);
lcd.print("the soil is perfect");

closeValve();


  }

}
void controlLight()
{
  
int ldr_val = analogRead(A1);
Serial.print("LDR value is ");
Serial.println(ldr_val);
   
lcd.setCursor(0, 2);
lcd.print("LDR value is ");
lcd.print(ldr_val);

if(ldr_val < 100)
  {
  digitalWrite(relay_pin, LOW);  
  Serial.println("Light is ON");
  lcd.setCursor(0, 3);
lcd.print("light is ON");

  }
else if (ldr_val > 300)
  {
digitalWrite(relay_pin, HIGH);  
Serial.println("Light is OFF");
lcd.setCursor(0, 3);
lcd.print("Light is OFF");

  }
}

