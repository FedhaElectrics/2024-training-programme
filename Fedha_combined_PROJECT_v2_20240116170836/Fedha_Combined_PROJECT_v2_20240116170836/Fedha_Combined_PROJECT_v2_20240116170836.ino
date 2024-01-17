
String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete
bool smsSent = false; // variable names should not have '-' in them
bool curtainOpen = false;  // Open == true, closed == false
bool lightOffsmsSent = false;
bool lightOn = false; //ON == true, OFF == false
int relay_pin = 12;

// Define the motor control pins
const int enA = 3;
const int in1 = 6;
const int in2 = 5;


void setup()
{
  // initialize serial:
  Serial.begin(115200);

  // Set the motor control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set the PWM control for the motor speed
  analogWrite(enA, 0);  // Initially set the motor speed to 0

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(relay_pin, OUTPUT); //SET RELAY PIN AS AN OUTPUT
  digitalWrite(relay_pin, HIGH); 

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
    if (inputString.indexOf("curtain open") >= 0)
    {
      if (!curtainOpen)
      {
        moveMotorForward();
        delay(3500);  // Wait for 2 seconds
        stopMotor();      // Stop the motor

        curtainOpen = true;

        Serial.write("OPEN: ");
        Serial.println(inputString);

        if (!smsSent)
        {
          sendSMS("Curtain is open");
          smsSent = true;
        }
      }
      else
      {
        sendSMS("Curtain already open");
        Serial.println("Curtain already open");
      }
    }
    else if (inputString.indexOf("curtain close") >= 0)
    {
      if (curtainOpen)
      {
        moveMotorBackward();
        delay(3500);  // Wait for 3.5 seconds
        stopMotor();      // Stop the motor

        curtainOpen = false;

        Serial.write("CLOSE: ");
        Serial.println(inputString);

        if (!smsSent)
        {
          sendSMS("Curtain is closed");
          smsSent = true;
        }
      }
      else 
      {
        sendSMS("Curtain already closed");
        Serial.println("Curtain already closed");
      }
    }
    else if(inputString.indexOf("light on") >= 0) 
    {
      if (!lightOn) 
      {
        digitalWrite(relay_pin, LOW);

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
        digitalWrite(relay_pin, HIGH);

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
    stringComplete = false;

  }
}

/*
  SerialEvent occurs whenever new data comes in the hardware serial RX.
  This routine is run between each time loop() runs, so using delay inside
  loop can delay response. Multiple bytes of data may be available.
*/

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

// Function to move the motor forward with PWM speed control
void moveMotorForward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 250);  // Adjust the PWM value for the desired speed (0 to 255)
}

// Function to move the motor backward with PWM speed control
void moveMotorBackward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 250);  // Adjust the PWM value for the desired speed (0 to 255)
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);  // Set the motor speed to 0
}
