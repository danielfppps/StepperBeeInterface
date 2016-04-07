# StepperBeeInterface

This is a simple project that aims to provide a general and open source interface (driver) to use the StepperBee and StepperBee plus motor controllers on windows and linux. You can read more about these motor controllers here (https://www.pc-control.co.uk/stepperbee_info.htm). To compile this library you will need the libusb libraries to be installed on your computer. You can use the following code to compile the library and sample program:

```
g++ -c -Wall -Werror -fpic stepper_bee_interface.c
g++ -shared -o libstepper_bee_interface.so stepper_bee_interface.o -lusb-1.0
sudo cp libstepper_bee_interface.so /usr/lib
g++ -Wall -o test_program test_program.c -lstepper_bee_interface
```

This code compiles the library and the test_program which performs two basic tests (runs motors for 200 steps in one direction and then 200 steps in the other direction while printing status and remaining steps). The library contains the following functions:

```
init_controller : Initializes the USB interface.
close_controller : Closes the USB interface.
get_digital_input_status: Writes the status of the 5 digital inputs.
move_motor: Moves the selected motor across the selected number of steps with the specified parameters. 
get_motor_status: Returns motor status and remaining number of steps for the two motors.
set_modes: Allows you to deactivate power and change the motor mode between full and half wave.
```

Tips:
```
Motor selection (0 = Motor 1, 1 = Motor 2)
Direction selection (0 = Forward, 1= Reverse)
Mode selection (0 = half-wave, 1 = full-wave)
Motor state (0 = off, 1 = on)
```

