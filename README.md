# Door Locking System ATmega16
Door Locking system using Atmega16 Micro Controller
# Door Security System


###### AVR-ATmega16 Door Security System with two ECU's

1. Interfacing Unit (IU) which is responsible for interfacing the user.
2. Processing Unit (PU) which is reponsible for handling all the logic.

## Routine
The User is asked to enter his initial password through out a 4x4 Keypad and all the visuals for easing
the user interaction are shown in a 16x2 LCD. Both connected to the 1st ECU (HMI).
The password is then sent to the 2nd ECU (PU) to handle all the logic of receiving it from the 1st ECU
by using UART serial communication protocol. Then, storing it in an external EEPROM which interfaces with the 2nd ECU using
I2C (Two wire Interface) serial communication protocol. Leaving both ECU's after in the Idle mode, waiting for the user
to either open the door (using DC-Motors) or change his password. Both cases requires the user to enter his already
stored password for verification. If the entered password is wrong for 2 times. The System shut downs itself
giving loud alarm that there might be a theft situation.

## Building the project
Through out your bash terminal, run the makefile with:
```bash
make -f makefile.mk
```
in both sources directories -> **Interfacing_Unit** / **Processing_Unit** 
It will generate two directories called **out** which has the executable files.

## Simulating
Any late version of Proteus would do. click on each controller and insert the executable files respectively.
