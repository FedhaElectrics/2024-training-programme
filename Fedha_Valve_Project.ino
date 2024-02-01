
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
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

}
void loop() {
   // put your main code here, to run repeatedly
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

   int val = analogRead(A1);
   Serial.print("Soil humidity is ");
   Serial.println(val);
    

    if (val > dry_threshold && !valveOpen)
  {
     Serial.println("the soil is dry");
     openValve();

  }
  else if (val < wet_threshold) 
  {
    
     Serial.println("the soil is wet");
     closeValve();

  }
  else
  { 
    Serial.println("the soil is perfect");
    closeValve();

  }

}

