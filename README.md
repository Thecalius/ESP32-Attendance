# ESP32 RFID Attendance System

This project is an attendance system using an ESP32 microcontroller and an RFID-RC522 module. By tapping an RFID tag, users can clock in and clock out. The data is sent to a Flask server, which logs the attendance information.

## Table of Contents
1. [Introduction](#introduction)
2. [Hardware Requirements](#hardware-requirements)
3. [Software Requirements](#software-requirements)
4. [Installation and Setup](#installation-and-setup)
    - [Cloning the Repository](#cloning-the-repository)
    - [Setting Up the Flask Server](#setting-up-the-flask-server)
    - [Configuring the ESP32 Code](#configuring-the-esp32-code)
5. [Building and Uploading Code to ESP32](#building-and-uploading-code-to-esp32)
6. [Usage](#usage)
7. [Project Structure](#project-structure)
8. [Contributing](#contributing)
9. [License](#license)
    
## Introduction
This project provides a simple and efficient way to track attendance using an RFID tag and an ESP32 microcontroller. The system logs clock-in and clock-out times on a Flask server and can be extended to include more advanced features.

## Hardware Requirements
- ESP32 microcontroller
- RFID-RC522 module
- RFID tags
- Breadboard and jumper wires
- Power supply (e.g., USB cable)

## Software Requirements
- [Python 3.x](https://www.python.org/downloads/) (for running the Flask server)
- [Flask](https://flask.palletsprojects.com/) (Python web framework)
- [PlatformIO](https://platformio.org/) (for building and uploading code to ESP32)
- [VS Code](https://code.visualstudio.com/) (recommended IDE)
- [Arduino IDE](https://www.arduino.cc/en/software) (optional)

## Installation and Setup

### Cloning the Repository
First, clone the project repository from GitHub to your local machine:

    ```
    git clone https://github.com/your-username/esp32-rfid-attendance.git
    cd esp32-rfid-attendance
    ```
### Setting Up the Flask Server
1. Navigate to the Flask server directory:

    ```
    cd server
    ```
2. Install the required Python packages:

    You can install the dependencies by running:

    ```
    pip install -r requirements.txt
    ```
3. Ensure the index.html file is placed in the templates folder:

    The Flask server requires an index.html file to serve the web interface. This file should be located in a templates directory within the same folder as         server.py.

4. Run the Flask server:

    Start the server by running:

    ```
    python server.py
    ```
    The server will be accessible at http://localhost:8000.

## Configuring the ESP32 Code
1. Open the project in VS Code with PlatformIO:

    Launch VS Code, and open the project folder.

3. Update the Wi-Fi credentials:

    In the ESP32 code, find and modify the following lines to match your Wi-Fi network name and password:

    ```
    const char* ssid = "Your Network Name";  // Network's name (Service Set Identifier)
    const char* wifi_password = "Your Wi-Fi Password";
    ```
3. Ensure the correct port and board are selected:

    Make sure your ESP32 board is properly selected in PlatformIO, and the correct port is chosen for uploading the code.

# Building and Uploading Code to ESP32
1. Build the project:

    In VS Code with PlatformIO, click on the "PlatformIO: Build" button in the bottom bar or run:
    
    ```
    platformio run
    ```
2. Upload the code to ESP32:

    Connect your ESP32 to your computer via USB and click the "PlatformIO: Upload" button, or run:

    ```
    platformio run --target upload
    ```
The code will be compiled and uploaded to your ESP32 board.

# Usage
1. Power on the ESP32 and ensure it is connected to the Wi-Fi network.
2. Tap an RFID tag on the RC522 module.
3. The ESP32 will send the RFID data to the Flask server, which logs the clock-in or clock-out time.

# Project Structure
Here's an overview of the main files and directories in this project:

```
esp32-rfid-attendance/
│
├── server/
│   ├── templates/
│   │   └── index.html         # HTML file for the Flask web interface
│   ├── server.py              # Flask server script
│   └── requirements.txt       # Python dependencies
│
├── src/
│   ├── main.cpp               # Main code for the ESP32
│   └── platformio.ini         # PlatformIO configuration file
│
├── README.md                  # This README file
└── .gitignore                 # Files and directories to ignore in git
```

# Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue if you have suggestions or bug reports.

# License
This project is licensed under the MIT License - see the LICENSE file for details.
