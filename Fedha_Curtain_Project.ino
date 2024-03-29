
String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete
bool smsSent = false; // variable names should not have '-' in them
bool curtainOpen = false;  // Open == true, closed == false

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
    
    // clear the string:
    inputString = "";
    stringComplete = false;

  }
}

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


