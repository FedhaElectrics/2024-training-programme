// Analog pin where the LDR is connected
const int ldrPin = A0;       
// Adjust this threshold value based on your LDR and lighting conditions
const int threshold = 500;   

void setup() {
  Serial.begin(9600);        // Initialize serial communication
}

void loop() {
  int ldrValue = analogRead(ldrPin);  // Read analog value from LDR

  if (ldrValue > threshold) {
    Serial.println("It's bright!");
  } else {
    Serial.println("It's dark!");
  }

  delay(1000);  // Delay for better readability, adjust as needed
}
