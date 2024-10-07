# Experimental Pixhawk Control with Nvidia Jetson Nano

This repository contains the experimental code developed by me(Joel) and Aaron to enable control of a Pixhawk flight controller using an Nvidia Jetson Nano. To break it down it establishes communication between the Jetson Nano and Pixhawk via MAVLink and the corresponding python library for it, allowing for operations such as arming the vehicle and requesting specific telemetry data. This is still a work in progress but will receive continual updates from myself and Aaron over the coming weeks.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [How It Works](#how-it-works)
- [Alignment with Control Team Requirements](#alignment-with-control-team-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Introduction

To begin with, the goal of this project is to develop a comprehensive control system for our ROV using the Nvidia Jetson Nano and Pixhawk flight controller. This code serves as a foundational step towards integrating various sensors, actuators, and control interfaces to achieve seamless operation of the ROV.

## Features

- Establishes the MAVLink connection between the Jetson Nano and Pixhawk.
- Arms the Pixhawk-controlled vehicle programmatically.
- Requests specific telemetry data at defined intervals.
- Provides a framework for future integration of sensors and actuators.

## How It Works

The provided Python script performs the following actions:

1. **MAVLink Connection**: Connects to the Pixhawk flight controller via MAVLink over TCP.
2. **Heartbeat Wait**: Waits for a heartbeat message to ensure communication is established.
3. **Vehicle Arming**: Sends a command to arm the vehicle.
4. **Data Request**: Defines functions to request specific MAVLink messages at set intervals and to handle received data.
5. **Data Logging**: Logs the requested data into a file for further analysis.

### Key Components

- **`mavutil.mavlink_connection`**: Establishes the MAVLink connection.
- **`master.wait_heartbeat()`**: Waits for the Pixhawk to send a heartbeat message.
- **Arming Command**: Uses `command_long_send` to arm the vehicle.
- **Message Interval Request**: `request_message_interval` function requests specific telemetry data.
- **Data Retrieval**: `get_requested_data` function retrieves and logs the requested data.

## Alignment with Control Team Requirements

This code aligns with the tasks outlined for the Control Team:

1. **Sensor Overview & Programming**: Sets the stage for integrating sensors by establishing communication protocols.
2. **Ethernet Communication**: Demonstrates data transmission between the Jetson Nano and Pixhawk over TCP.
3. **Modular Code Structure**: Uses functions like `request_message_interval` and `get_requested_data` to promote modularity.
4. **Error Handling**: Includes try-except blocks to manage exceptions gracefully.
5. **PEP 8 Compliance**: Adheres to Python coding standards for readability and maintainability.

Future updates will focus on:

- Integrating additional sensors like IMUs, sonars, and depth sensors.
- Implementing control for actuators such as the T-200 thrusters and servo motors.
- Establishing a control interface using devices like a PS4 controller.
- Implementing safety features like a kill switch and water leak detection system.

## Installation

### Prerequisites

- Nvidia Jetson Nano
- Pixhawk flight controller
- Python 3 installed on the Jetson Nano
- MAVLink and pymavlink libraries

### Steps

1. **Clone the Repository**

   Clone this repository to your local machine.

2. **Install Required Python Packages**

   Install the necessary Python packages using pip.

3. **Connect the Pixhawk to the Jetson Nano**

   Ensure that the Pixhawk is connected via USB or the appropiate serial port to the Jetson Nano.

## Usage

1. **Navigate to the Project Directory**

   Go to the directory where the project is located.

2. **Run the Script**

   Execute the Python script using Python 3.

3. **Monitor Output**

   The script will:

   - Confirm connection to the Pixhawk.
   - Arm the vehicle.
   - Wait for the vehicle to confirm arming.
   - Request and log telemetry data.






















































































































































































































































































































































































