#define PWM_PIN1 32  // PWM on pin 32
#define PWM_PIN2 27  // PWM on pin 27
#define LED_PIN 13   // LED on pin 13

#define NUM_READINGS 5  // Number of readings for filtering

#define FREQ 25000
#define RESOL 10

int readings[NUM_READINGS];  // Array to store the last 5 readings
int currentIndex = 0;        // Current index in the circular buffer
int total = 0;               // Sum of all readings in the buffer
int average = 0;             // Average of the readings

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  ledcAttachChannel(PWM_PIN1, FREQ, RESOL, LEDC_CHANNEL_0);
  ledcAttachChannel(PWM_PIN2, FREQ, RESOL, LEDC_CHANNEL_0);

  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }

  Serial.println("started..");
}

void loop() {
  int newValue = analogRead(35);  // Read input from pin 35

  // Update the circular buffer
  total -= readings[currentIndex];  // Subtract the oldest value from the total
  readings[currentIndex] = newValue;  // Replace it with the new reading
  total += newValue;                // Add the new value to the total
  currentIndex = (currentIndex + 1) % NUM_READINGS;  // Move to the next index in the circular buffer

  // Calculate the average
  average = total / NUM_READINGS;
  // Replace map logic with manual range handling
  int mappedValue;
  if (average <= 2840) {
    mappedValue = 55;
  } else if (average <= 2930) {
    mappedValue = 105;
  } else if (average <= 3050) {
    mappedValue = 200;
  } else {
    mappedValue = 1000;
  }

  // Write the mapped value to PWM channels
  ledcWriteChannel(LEDC_CHANNEL_0, mappedValue);


  // Debugging information
  Serial.print("Input Value: ");
  Serial.print(newValue);
  Serial.print(" -> Filtered Value (Average): ");
  Serial.print(average);
  Serial.print(" -> Mapped Value: ");
  Serial.println(mappedValue);

  delay(200);
}
