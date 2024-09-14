# Automatic Dipping Unit for Layer-by-Layer (LBL) Assembly

This project is designed to automate the dipping process for Layer-by-Layer (LBL) assembly, a technique used to create thin films by alternating exposure of a substrate to various solutions. The system utilizes stepper motors to control the vertical motion of the dipping arm and the rotation of a carousel holding multiple solution containers. It also integrates limit switches, an LCD display, and safety features for accurate and safe operation.

## Table of Contents
- [Project Overview](#project-overview)
- [Hardware Setup](#hardware-setup)
- [Software Requirements](#software-requirements)
- [How It Works](#how-it-works)
- [How to Use](#how-to-use)
- [Pin Configuration](#pin-configuration)
- [Future Enhancements](#future-enhancements)
- [License](#license)

## Project Overview
The project automates the Layer-by-Layer (LBL) assembly process with precise control over:
- Vertical movement for dipping and drying.
- Rotational movement to switch between different solution containers.
- Safety mechanisms such as door sensors and limit switches to ensure safe operation.
- A simple user interface using an LCD display for real-time feedback.

### Key Features
- **Vertical Stepper Motor**: Controls the up-and-down movement of the dipping arm.
- **Carousel Stepper Motor**: Rotates a carousel holding up to 8 solution containers.
- **Safety Door Sensor**: Stops the process if the safety door is opened.
- **LCD Display**: Provides real-time status updates, including dipping times and process progress.

## Hardware Setup

### Components
- **Arduino**: The microcontroller used to control the system.
- **Stepper Motors (2)**: One for vertical movement and one for rotating the solution carousel.
- **Limit Switches (2)**: To detect the top and bottom positions of the vertical movement.
- **Solution Carousel**: Holds 8 solution containers for the dipping process.
- **LCD Display**: Provides process feedback.
- **Safety Door Sensor**: Ensures the system stops if the safety door is opened.
- **Power Supply**: A regulated power source for the motors and Arduino.

### Wiring Diagram
Connect the components as follows:

- **Vertical Stepper Motor (Pins 8, 9, 10, 11)**: Controls vertical movement.
- **Carousel Stepper Motor (Pins A0, A1, A2, A3)**: Rotates the solution carousel.
- **Top Limit Switch (Pin 2)**: Detects the top position of the dipping arm.
- **Bottom Limit Switch (Pin 3)**: Detects the bottom position of the dipping arm.
- **Solution Select Pins (Pins 4, 5, 6, 7)**: Controls the selection of solutions.
- **Safety Door Sensor (Pin 12)**: Monitors whether the safety door is closed.
- **LCD Display (I2C)**: Displays process information.

## Software Requirements
- **Arduino IDE**: To upload the code to the Arduino board.
- **LiquidCrystal_I2C Library**: For controlling the LCD display.
- **Stepper Library**: For controlling the stepper motors.

Install the necessary libraries via the Arduino Library Manager:
```bash
Sketch > Include Library > Manage Libraries > Search for "LiquidCrystal_I2C" and "Stepper"
```

## How It Works

### Process Flow
1. **Initialization**: The system first checks if the safety door is closed. The carousel and vertical arm are initialized by moving the arm to the top position.
2. **Solution Selection**: The carousel rotates to select the correct solution container.
3. **Dipping**: The vertical stepper motor lowers the substrate into the solution for a specified time.
4. **Drying**: After dipping, the arm slowly rises to dry the substrate.
5. **Repeat**: The process repeats for multiple solutions and bilayers.

### Dipping Cycle
Each bilayer consists of the following steps:
- Dip into **PEI Solution**.
- Rinse with three **DI Water** solutions.
- Dip into **PAA Solution**.
- Rinse with another set of three **DI Water** solutions.

### Safety Features
- **Limit Switches**: Ensure that the dipping arm does not exceed its allowed range.
- **Safety Door Sensor**: Halts the process if the door is opened during operation.

## How to Use

### 1. Upload the Code
1. Connect the Arduino to your computer.
2. Open the code in the Arduino IDE.
3. Upload the code to the Arduino.

### 2. Start the Process
1. Make sure all solutions are in place on the carousel.
2. Close the safety door.
3. Use the serial monitor to send the command `'S'` to start the process.
4. The process will proceed automatically with real-time feedback on the LCD screen.

### 3. Monitor the Process
- The LCD will display which solution is currently being used and the remaining dipping time.
- If the safety door is opened, the process will stop, and an error message will be displayed.

## Pin Configuration

| Component               | Arduino Pin |
|-------------------------|-------------|
| Vertical Stepper Motor   | 8, 9, 10, 11|
| Carousel Stepper Motor   | A0, A1, A2, A3|
| Top Limit Switch         | 2           |
| Bottom Limit Switch      | 3           |
| Solution Select (4 pins) | 4, 5, 6, 7  |
| Safety Door Sensor       | 12          |
| LCD (I2C)                | I2C pins (SDA, SCL) |

## Future Enhancements
- **Automated Solution Refreshing**: Add pumps and sensors to automate the refilling of solutions.
- **Advanced Error Handling**: Include more detailed error messages for debugging.
- **Data Logging**: Save the process data to an SD card for quality control and analysis.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

---

Feel free to modify and extend the project for your specific LBL assembly needs.
