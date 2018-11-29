#include "Arduino.h"
#include "esp32-hal-ledc.h"

const int PERCENTAGE_0 = 0;
const int PERCENTAGE_100 = 100;

const int MIN_PWM_OUT = 0;
const int MAX_PWM_OUT = 255;

const int VICTOR_PWM_IN_FREQUENCY = 400;
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
 * @param left - 1 for left drivetrain, 0 for right
 * @param channel - channel to set to left or right
 */ 
void setDriveChannel(int left, int channel) {
	if (left) {
		for (int i = 0; i < MOTORS_PER_SIDE; i++) {
			if (leftDriveTrain[i] != -1) {
				leftDriveTrain[i] = channel;
				return;
			}
		}
	} else {
		for (int i = 0; i < MOTORS_PER_SIDE; i++) {
			if (rightDriveTrain[i] != -1) {
				rightDriveTrain[i] = channel;
				return;
			}
		}
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
	// (value, fromLow, fromHigh, toLow, toHigh)
	map(left, PERCENTAGE_0, PERCENTAGE_100, MIN_PWM_OUT, MAX_PWM_OUT);
	map(right, PERCENTAGE_0, PERCENTAGE_100, MIN_PWM_OUT, MAX_PWM_OUT);

	for (int i = 0; i < MOTORS_PER_SIDE; i++) {
		ledcWrite(leftDriveTrain[i], left);
		ledcWrite(rightDriveTrain[i], right);
	}
}






