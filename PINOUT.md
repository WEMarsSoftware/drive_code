# Pinout for ESP-32 running this project's drive code

# Motor PWM Signals
- Left Side Motors:
	- PIN 32, 33, 25
- Right Side Motors:
	- PINS 4, 2, 15

# Motor Current Sensors
- Input from 3x8 multiplexer
	- PIN 35
- Select pins for multiplexer
	- S0: PIN 21 (LSb)
	- S1: PIN 19
	- S2: PIN 18 (MSb)

# Wheel Rotary Encoders
- Input from two 3x8 multiplexers: Pins 36 and 39
- Select pins for left side motors
	- S0: PIN 13 (LSb)
	- S1: PIN 12
	- S2: PIN 14 (MSb)
- Select pins for right side motors
	- S0: PIN 16 (LSb)
	- S1: PIN 17
	- S2: PIN 5 (MSb)
