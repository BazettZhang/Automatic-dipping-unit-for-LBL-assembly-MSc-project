#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// Stepper motor setup for vertical movement
const int stepsPerRevolutionVertical = 200;
const int totalVerticalSteps = 1000; // Total number of steps to move from top to bottom
const int verticalMovementTime = 5000; // 5 seconds
Stepper myStepperVertical(stepsPerRevolutionVertical, 8, 9, 10, 11);

// Stepper motor setup for rotating solution carousel
const int stepsPerRevolutionCarousel = 200; // Adjust this based on your stepper motor
Stepper myStepperCarousel(stepsPerRevolutionCarousel, A0, A1, A2, A3); // Additional stepper motor on analog pins

// Limit switch pins
const int topLimitSwitch = 2;
const int bottomLimitSwitch = 3;

// Solution selection pins
const int solutionSelect[] = {4, 5, 6, 7};
const int numSolutions = 4;

// Safety door control
const int doorSensor = 12;

// Process parameters
const int numBilayers = 5;
const unsigned long peiPaaDippingTime = 300000; // 300 seconds (adjusted from 5 minutes for faster testing)
const unsigned long diWaterDippingTime = 90000; // 90 seconds (adjusted from 1.5 minutes for faster testing)
const unsigned long dryingTime = 60000; // 60 seconds (adjusted from 1 minute for faster testing)

// Constants for the carousel
const int numPositions = 8; // Number of positions on the carousel
const int stepsPerPosition = stepsPerRevolutionCarousel / numPositions; // Steps needed for each 45-degree rotation

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  myStepperVertical.setSpeed(60); // Set speed for vertical motor
  myStepperCarousel.setSpeed(60); // Set speed for the carousel motor
  Serial.begin(9600); // Start serial communication
  lcd.begin(16, 2); // Initialize LCD
  lcd.backlight(); // Turn on LCD backlight
  pinMode(topLimitSwitch, INPUT_PULLUP); // Set top limit switch as input with pullup
  pinMode(bottomLimitSwitch, INPUT_PULLUP); // Set bottom limit switch as input with pullup
  for (int i = 0; i < numSolutions; i++) {
    pinMode(solutionSelect[i], OUTPUT); // Set solution select pins as output
    digitalWrite(solutionSelect[i], LOW); // Initialize them to LOW
  }
  pinMode(doorSensor, INPUT_PULLUP); // Set door sensor as input with pullup
  moveToTop(); // Move to the top at the start
}

void loop() {
  if (Serial.available() > 0) { // Check if there is serial input
    char command = Serial.read();
    if (command == 'S') { // If 'S' is received, start the process
      startProcess();
    }
  }
}

void startProcess() {
  if (digitalRead(doorSensor) == HIGH) { // Check if the safety door is open
    Serial.println("Error: Safety door open");
    lcd.setCursor(0, 0);
    lcd.print("Door Open!     ");
    return;
  }

  for (int bilayer = 1; bilayer <= numBilayers; bilayer++) { // Process each bilayer
    Serial.print("Processing bilayer ");
    Serial.println(bilayer);
    
    processSolution("PEI", 1, peiPaaDippingTime, 1); // PEI at position 1
    
    for (int i = 2; i <= 4; i++) {
      processSolution("DI water", i, diWaterDippingTime, i); // DI water at positions 2, 3, 4
    }
    
    slowRise(); // Perform slow rise for drying
    
    processSolution("PAA", 5, peiPaaDippingTime, 5); // PAA at position 5
    
    for (int i = 6; i <= 8; i++) {
      processSolution("DI water", i, diWaterDippingTime, i); // DI water at positions 6, 7, 8
    }
    
    slowRise(); // Perform slow rise for drying
  }
  
  Serial.println("LBL assembly process complete!"); // Indicate that the process is complete
  lcd.setCursor(0, 0);
  lcd.print("Process Complete");
}

void processSolution(const char* solutionName, int solutionNumber, unsigned long dippingTime, int position) {
  // Process a specific solution at a given position with a defined dipping time
  Serial.print("Processing ");
  Serial.print(solutionName);
  Serial.print(" (Solution ");
  Serial.print(solutionNumber);
  Serial.print(" at Position ");
  Serial.print(position);
  Serial.println(")");

  moveToTop(); // Move the device to the top position before switching solutions

  rotateCarouselToPosition(position - 1); // Rotate the carousel to the target position (0-indexed)

  selectSolution(solutionNumber); // Select the solution at the current position

  moveToBottom(); // Move the device down to start dipping

  dip(dippingTime); // Start the dipping process
}

void selectSolution(int solution) {
  // Select the specific solution by turning the appropriate pin HIGH
  for (int i = 0; i < numSolutions; i++) {
    digitalWrite(solutionSelect[i], i == (solution - 1)); // Set the correct pin HIGH (0-indexed)
  }
}

void dip(unsigned long dippingTime) {
  // Perform dipping for the given time duration
  unsigned long startTime = millis();
  while (millis() - startTime < dippingTime) {
    unsigned long remainingTime = (dippingTime - (millis() - startTime)) / 1000; // Calculate remaining time in seconds
    lcd.setCursor(0, 0);
    lcd.print("Dipping: ");
    lcd.print(remainingTime);
    lcd.print(" s    ");
    delay(100); // Update every 100ms
  }
  delay(500); // Small delay after dipping
}

void slowRise() {
  // Slowly raise the device for drying over the defined drying time
  Serial.println("Slow rising for drying");
  unsigned long stepDelay = dryingTime / 100; // Delay between each step
  for (int i = 0; i < 100; i++) {
    myStepperVertical.step(stepsPerRevolutionVertical / 100); // Move up in small steps
    unsigned long remainingTime = ((100 - i) * stepDelay) / 1000; // Calculate remaining drying time in seconds
    lcd.setCursor(0, 0);
    lcd.print("Drying: ");
    lcd.print(remainingTime);
    lcd.print(" s    ");
    delay(stepDelay); // Delay between steps
  }
}

void moveToTop() {
  // Move the device up over a 5-second period
  unsigned long stepDelay = verticalMovementTime / totalVerticalSteps;
  for (int i = 0; i < totalVerticalSteps; i++) {
    myStepperVertical.step(1); // Move upwards one step at a time
    delay(stepDelay); // Delay between steps to ensure 5-second movement
  }
  Serial.println("Reached top position");
}

void moveToBottom() {
  // Move the device down over a 5-second period
  unsigned long stepDelay = verticalMovementTime / totalVerticalSteps;
  for (int i = 0; i < totalVerticalSteps; i++) {
    myStepperVertical.step(-1); // Move downwards one step at a time
    delay(stepDelay); // Delay between steps to ensure 5-second movement
  }
  Serial.println("Reached bottom position");
}

void rotateCarouselToPosition(int position) {
  // Rotate the carousel to a specific position
  int stepsToMove = position * stepsPerPosition;
  myStepperCarousel.step(stepsToMove); // Move the carousel by the required number of steps
  Serial.print("Rotated carousel to position ");
  Serial.println(position + 1); // Output the new position (1-indexed for clarity)
}
