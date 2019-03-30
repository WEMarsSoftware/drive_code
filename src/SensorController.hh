// SensorController.hh

#ifndef SENSOR_CONTROLLER
#define SENSOR_CONTROLLER

#include "Arduino.h"
#include "RotaryEncoder.h"
#include "SPI.h"

// Note: different than Arduino
const int MAX_ANALOG_IN = 4095;
const int MIN_ANALOG_IN = 0;

// CURRENT SENSORS
// varies linearly with DC in
const int NUM_CHASSIS_MOTORS = 6;

const int MAX_CURRENT_IN = 33;

const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;

// code running on Core #0
class SensorController
{
public:
	// let WiFi callback access these directly
	static int currentValues[NUM_CHASSIS_MOTORS];
	static int speedValues[NUM_CHASSIS_MOTORS];

	// RotaryEncoder library will count number of ticks in specified time length
	// specified in constructor (last parameter in microseconds)
	static RotaryEncoder* encoders[NUM_CHASSIS_MOTORS];
	static int deltaTicks[NUM_CHASSIS_MOTORS];

	// pin assignments
	static int A_PINS[NUM_CHASSIS_MOTORS];
	static int B_PINS[NUM_CHASSIS_MOTORS];
	static int CURRENT_IN[NUM_CHASSIS_MOTORS];

  // SPI constants
  static const int HSPI_CLK;
  static const int HSPI_MISO;
  static const int HSPI_MOSI;
  static const int HSPI_CS_CURR;
  static const int HSPI_CS_IO;
  
  static const int VSPI_CLK;
  static const int VSPI_MISO;
  static const int VSPI_MOSI;
  static const int VSPI_CS_POT;
  static const int VSPI_EOC_POT;

  static const int CAN_R;
  static const int CAN_D;
  
	// constants
  static const int CORE_LOOP_DELAY;
  static const int ENCODER_TIME;

	// main infinite loop to update data points
	static void sensorsCoreLoop();

	// attach encoders to pins
	static void setupSensors(void* args);
  static void vspiCommand();
  static void hspiCommand();
};

// link statics
int SensorController::currentValues[NUM_CHASSIS_MOTORS] = {};
int SensorController::speedValues[NUM_CHASSIS_MOTORS] = {};
RotaryEncoder* SensorController::encoders[NUM_CHASSIS_MOTORS];

// pin assignments temporary
int SensorController::A_PINS[NUM_CHASSIS_MOTORS] = {36, 34, 32, 25, 26, 19};
int SensorController::B_PINS[NUM_CHASSIS_MOTORS] = {39, 35, 33, 23, 27, 18};
int SensorController::CURRENT_IN[NUM_CHASSIS_MOTORS] = {9, 10, 11, 19, 18, 5};
int SensorController::deltaTicks[NUM_CHASSIS_MOTORS] = {};

// SPI constants
const int HSPI_CLK = 14;
const int HSPI_MISO = 12;
const int HSPI_MOSI = 13;
const int HSPI_CS_CURR = 1;
const int HSPI_CS_IO = 3;

const int VSPI_CLK = 18;
const int VSPI_MISO = 19;
const int VSPI_MOSI = 23;
const int VSPI_CS_POT = 26;
const int VSPI_EOC_POT = 27;

const int CAN_R = 22;
const int CAN_D = 21;

// constants
const int SensorController::CORE_LOOP_DELAY = 10;
const int SensorController::ENCODER_TIME = 1000;

// temp array for reading encoder ticks
int deltaTicks[NUM_CHASSIS_MOTORS] = {};
const int CORE_LOOP_DELAY = 10;
const int ENCODER_TIME = 1000;

void SensorController::sensorsCoreLoop()
{
	while (true)
	{
  /*
		// update encoder and current sensor data
		for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
		{
			deltaTicks[i] = encoders[i]->readEncoder();
			speedValues[i] = (double)(deltaTicks[i])/ENCODER_TIME;

			currentValues[i] = analogRead(CURRENT_IN[i]);
		}
   */
    vspiCommand();
    hspiCommand();
		delay(CORE_LOOP_DELAY);
  } 
}

void SensorController::setupSensors(void* args)
{
	// create RotaryEncoder objects
	/*for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
	{
		//encoders[i] = new RotaryEncoder(A_PINS[i], B_PINS[i], 1, 1, ENCODER_TIME);
	}*/

   //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);
  
  //clock miso mosi ss

  //initialise
  vspi->begin(VSPI_CLK, VSPI_MISO, VSPI_MOSI, VSPI_CS_POT);
  vspi->setHwCs(false);
  hspi->begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, HSPI_CS_CURR);
  hspi->setHwCs(false);

  //set up slave select pins as outputs
  pinMode(VSPI_CS_POT, OUTPUT);
  pinMode(HSPI_CS_CURR, OUTPUT);
  pinMode(HSPI_CS_IO, OUTPUT);
  
	// don't let this task end
	sensorsCoreLoop();
}


void SensorController::vspiCommand() {
  byte data = 0b01010101; // junk data to illustrate usage

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(VSPI_CS_POT, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(data);  
  digitalWrite(VSPI_CS_POT, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
}

void SensorController::hspiCommand() {
  byte stuff = 0b11001100;
  
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(HSPI_CS_CURR, LOW);
  hspi->transfer(stuff);
  digitalWrite(HSPI_CS_CURR, HIGH);
  hspi->endTransaction();
}

#endif
