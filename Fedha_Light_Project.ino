String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete
bool smsSent = false; // variable names should not have '-' in them
bool lightOffsmsSent = false;
bool lightOn = false; //ON == true, OFF == false
int  ledPin = 12;


void setup()
{
  // initialize serial:
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, LOW); 

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  delay(2000);
  Serial.println("AT\r");
  delay(500);
  Serial.println("AT+CMGF=1\r");
  delay(500);
  Serial.println("AT+CNMI=2,2,0,0,0\r");
  delay(500);
  Serial.println("AT+COPS?\r");
  delay(500);
}

void loop()
{
  // print the string when a newline arrives:
  int val = analogRead(A0);
  if (stringComplete)
   {
    if(inputString.indexOf("light on") >= 0) 
    {
      if (!lightOn) 
      {
        digitalWrite(ledPin, HIGH);

        lightOn = true;

        Serial.write("ON: ");
        Serial.println(inputString);

        if(!smsSent && val > 300)
        {
          Serial.print("Val: ");
          Serial.println(val);

          delay(500);
          
          sendSMS("Light is on");
          smsSent = true;
        }
      } else 
      {
        sendSMS("Light already on");
        Serial.println("Light already on");
      }    
    }
    else if (inputString.indexOf("light off") >= 0)
    {
      if (lightOn)
      {
        digitalWrite(ledPin,LOW );

        lightOn = false;

        Serial.write("OFF: ");
        Serial.println(inputString);

        if(!smsSent && val < 100 )
        {
          Serial.print("Val: ");
          Serial.println(val);

          delay(500);

          sendSMS("Light is off");
          Serial.println("Light is off");
        }
      } else {
        sendSMS("Light is already off");
        Serial.println("Light is already off");
      }      
    }
    // clear the string:
    inputString = "";
 } stringComplete = false;  
}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

void sendSMS(const char* message)
{
  Serial.println("AT+CMGS=\"0725598031\" \r ");
  delay(100);
  Serial.println(message);
  Serial.write(0x1A); // Ctrl+Z
  delay(300);
}
