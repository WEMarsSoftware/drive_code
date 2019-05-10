#include "Arduino.h"
#include "esp32-hal-ledc.h"
#include "PreprocessorOptions.hh"

#ifndef ELECTRICAL_HH
#define ELECTRICAL_HH

const int PERCENTAGE_0 = -100;
const int PERCENTAGE_100 = 100;

// 63 is full reverse, 153 is neutral, 243 is max forwards
const int MIN_PWM_OUT = 63;
const int MAX_PWM_OUT = 243;

const int VICTOR_PWM_IN_FREQUENCY = 400;
//const int VICTOR_PWM_IN_FREQUENCY = 5000;
const int DEFAULT_BIT_RESOLUTION = 8;

int leftDriveTrain[] = {-1, -1, -1};
int rightDriveTrain[] = {-1, -1, -1};
const int MOTORS_PER_SIDE = 3;

/**
 * Our own wrapper around ledc setup and attach.
 * This should be called in main setup() once per pin.
 *
 * Note that for the Victor motor controllers,
 * input frequencies should be between 500 and 20 Hz.
 * 
 * @param freq PWM frequency in Hz
 */
void setup(int pin, int channel, int freq=VICTOR_PWM_IN_FREQUENCY, int bits=DEFAULT_BIT_RESOLUTION) {
	ledcAttachPin(pin, channel);
	ledcSetup(channel, freq, bits);
}

/**
 * Attach channels to LEFT or RIGHT drivetrains.
 * This will just overwrite the -1 defaults.
 *
 * @param pin - 0, 1, 2 for left drive train and 3, 4, 5 for right drive train
 * @param channel - channel to set to left or right
 */ 
void setDriveChannel(unsigned int pin, int channel) {
  if (pin <= 2) {
    leftDriveTrain[pin] = channel;
  }
  else if (pin <= 5) {
    rightDriveTrain[5-pin] = channel;
  }
}

/**
 * Given "percentage power" (int's) for LEFT and RIGHT drivetrains,
 * send out PWM signals on these lines.
 *
 * This will be called every time the esp receives a properly 
 * formatted HTTP GET (see CommunicationStuff.hh).
 * 
 *
 * This assumes all pins have gone through proper ledcAttach() and ledcSetup()
 * Voltages given to ledcWrite should be mapped between 0 and 255
 */ 
void moveMotors(int left, int right){

#ifdef DEBUG
  Serial.println("MOVING MOTORS");
#endif
  
	// (value, fromLow, fromHigh, toLow, toHigh)
	left = map(left, PERCENTAGE_0, PERCENTAGE_100, MIN_PWM_OUT, MAX_PWM_OUT);
	right = map(right, PERCENTAGE_0, PERCENTAGE_100, MIN_PWM_OUT, MAX_PWM_OUT);
 
#ifdef DEBUG
  Serial.println(left);
  Serial.println(right);
#endif
  
	for (int i = 0; i < MOTORS_PER_SIDE; i++) {
		ledcWrite(leftDriveTrain[i], left);
		ledcWrite(rightDriveTrain[i], right);

#ifdef DEBUG
    Serial.println("Left Channel " + leftDriveTrain[i]);
    Serial.println("Value: " + left);
    Serial.println("Right Channel " + rightDriveTrain[i]);
    Serial.println("Value: " + right);
#endif
	}

}

#endif
