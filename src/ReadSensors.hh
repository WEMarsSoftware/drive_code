// ReadSensors.hh

#ifndef READ_SENSORS
#define READ_SENSORS

#include "Arduino.h"

// Note: different than Arduino
const int MAX_ANALOG_IN = 4095;
const int MIN_ANALOG_IN = 0;

// CURRENT SENSORS
// varies linearly with DC in
const int NUM_CHASSIS_MOTORS = 6;

// Must be pin > 31 when WiFi on
const int CURRENT_SENSOR_VIN = 35;

const int MAX_CURRENT_IN = 33;
int currentValues[NUM_CHASSIS_MOTORS] = {};

// [0] is LSb and [2] is MSb
const int CURRENT_MUX_SELECT_VOUT = {21, 19, 18};
const int ROTARY_MUX_SELECT_A = {13, 12, 14};
const int ROTARY_MUX_SELECT_B = {16, 17, 5};
uint8_t muxSelects[3] = {};

// input from muxes
const int ROTARY_ENCODER_PIN_A = 36;
const int ROTARY_ENCODER_PIN_B = 39;
int rotaryValues[NUM_CHASSIS_MOTORS] = {};

void inline updateCurrentMuxSelect() {
	digitalWrite(CURRENT_MUX_SELECT_VOUT[0], muxSelects[0]);
	digitalWrite(CURRENT_MUX_SELECT_VOUT[1], muxSelects[1]);
	digitalWrite(CURRENT_MUX_SELECT_VOUT[2], muxSelects[2]);
}

void inline updateRotaryMuxSelect() {
	digitalWrite(ROTARY_MUX_SELECT_A[0], muxSelects[0]);
	digitalWrite(ROTARY_MUX_SELECT_A[1], muxSelects[1]);
	digitalWrite(ROTARY_MUX_SELECT_A[2], muxSelects[2]);
	digitalWrite(ROTARY_MUX_SELECT_B[0], muxSelects[0]);
	digitalWrite(ROTARY_MUX_SELECT_B[1], muxSelects[1]);
	digitalWrite(ROTARY_MUX_SELECT_B[2], muxSelects[2]);
}
/* 
 * After each call to updateMuxSelect() we now have routed a current sensor
 * read-out signal to CURRENT_SENSOR_VIN pin
 */
uint8_t currentNum = 0;

void updateCurrentNum() {
	currentNum++;
	if (currentNum == 5) { currentNum = 0; }
}
void readCurrentVal() {
	updateCurrentMuxSelect();
	currentValues[currentNum] = analogRead(CURRENT_SENSOR_VIN);
	updateCurrentNum();
}

/**
 * After we update rotary mux select outputs,
 * we have directed the A and B channels for one sensor to pin 36 and 39
 *
 * Note: this is supposed to be done with interrupts.. but that doesn't
 * work when WiFi services are running (which is all the time).
 * This will give us some info but won't tell us speed directly.
 *
 * Model: US Digital E4T Miniature Optical Kit Encoder
 */
void readRotaryPos() {
	updateRotaryMuxSelect();
	rotaryValues[currentNum] = (analogRead(ROTARY_ENCODER_PIN_A)+analogRead(ROTARY_ENCODER_PIN_B));
	updateCurrentNum();
}
/**
 * Inputs are routed to a 3x8 multiplexer.
 * After each call to updateMuxSelect() we now have routed a current sensor
 * read-out signal to CURRENT_SENSOR_VIN pin
 * @return pointer to currentValues array
 */
int* inline getCurrentValues() {
	// signal 000
	muxSelects[0] = muxSelects[1] = muxSelects[2] = 0;
	readCurrentVal();

	// signal 001
	muxSelects[2] = 1;
	readCurrentVal();

	// signal 010
	muxSelects[1] = 1;
	muxSelects[2] = 0;
	readCurrentVal();

	// signal 011
	muxSelects[2] = 1;
	readCurrentVal();

	// signal 100
	muxSelects[0] = 1;
	muxSelects[1] = muxSelects[2] = 0;
	readCurrentVal();

	// signal 101
	muxSelects[2] = 1;
	readCurrentVal();
	
	// map to a number between 0 and 4095 to 0->33 Amps
	for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
	{
		map(currentValues[i], MIN_ANALOG_IN, MAX_ANALOG_IN, MIN_ANALOG_IN, MAX_CURRENT_IN);
	}
	return currentValues;
}

/**
 * Read position signals from 6 rotary encoders on chassis motors.
 * @return pointer to global array containing results
 */ 
int* inline getRotaryPositions() {
	// signal 000
	muxSelects[0] = muxSelects[1] = muxSelects[2] = 0;
	readCurrentVal();

	// signal 001
	muxSelects[2] = 1;
	readCurrentVal();

	// signal 010
	muxSelects[1] = 1;
	muxSelects[2] = 0;
	readCurrentVal();

	// signal 011
	muxSelects[2] = 1;
	readCurrentVal();

	// signal 100
	muxSelects[0] = 1;
	muxSelects[1] = muxSelects[2] = 0;
	readCurrentVal();

	// signal 101
	muxSelects[2] = 1;
	readCurrentVal();
	
	return rotaryValues;
}


#endif
