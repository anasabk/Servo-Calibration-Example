# Servo_Calibration_ESP32
This program was created as an example of servo calibrartion procedure. The program takes commands from the monitor to send PWM signals to the servo motor. 
Here is how the input is taken:
* Entering 'c' means that the next input to be sent to the servo is a degree, and the calibrated model is going to be used. (the calibration values should be manually added to the code)
* Entering 'n' means that the next input to be sent to the servo is a PWM signal, and the non-calibrated model is going to be used.
* Entering 'k' means that the current input is transferred to the servo.
* Entering a number will place it as the least significant digit of the current input. (ex. number = 2, input = 13 => input = 132)
