# Drawing System

Welcome to the Drawing System project, an Arduino-based system designed for precise control of a drawing tool, such as a pencil. The system is equipped with servo motors for accurate positioning. Additionally, a Python script is provided to facilitate image processing for drawing intricate patterns.

https://github.com/HussAreus/Lego-Bob-Ross/assets/93789780/9da389db-d18d-4629-adc8-274f061d8711

## Arduino Code (drawing_system.ino)

# Overview

The Arduino code orchestrates the functionality of the drawing system. Utilizing servo motors, it enables the system to draw predefined shapes. The code includes features for starting, stopping, and loading drawings.

## Components 

2 servo motors   
1 arduino uno  
A lot of specific legos   

### Libraries:

Stepper.h   
Servo.h    
Vector.h   
math.h   

### Servo Motors:  
SERVOX, SERVOY, SERVOZ: Pins for servo motors controlling the X, Y, and Z axes.  

### Button Inputs:
START_STOP_BTN: Initiates or halts drawing.  
LOAD_BTN: Loads predefined drawings.  

### Classes
Vector2D: Represents a 2D vector with x and y coordinates.  
Needle: Manages pencil/needle movement and drawing functionality.  
Shape: Defines a shape using a collection of points.  

### Functions
rescaleImage(Vector<Shape*> shapes): Scales shapes based on minimum and maximum coordinates.  
Button functions: startStop(), load() for handling user input.  

### Setup
Initializes serial communication and servo motors.  
Sets up interrupts for start/stop and load buttons.  

### Loop
Main program loop:  
In DRAW or LOAD state, loads predefined shapes and executes drawing.  
In WAIT state, resets the needle.  
Python Code (image_processing.py)  
Overview  
The Python script complements the Arduino functionality by processing images and identifying lines. Using OpenCV for image processing, it applies edge detection, performs Hough line transform, and filters lines based on proximity.  

### Dependencies
OpenCV (cv2)  
NumPy (numpy)  
Functions  
Image Processing:  

Reads an image and converts it to grayscale.  
Applies edge detection using the Canny algorithm.  
Performs Hough line transform to detect lines.  
Line Filtering:  

Filters similar lines based on a specified tolerance.  
Draws filtered lines on a copy of the original image.  
Writes filtered line coordinates to points.txt.  
### Notes
Calibrate servo and stepper motors for precise movement.    
Customize loadImages function to load relevant shapes.        
Adjust parameters (delays, servo angles, scaling) as needed.   
