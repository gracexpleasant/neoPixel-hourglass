
//int neoIndex = 0;
//unsigned long endTime;
//unsigned long startTime;
//int interval = 50;
//int caseValue = 0;

//---------------------------------------------

const int buzzerPin = 7;

//---------------------------------------------

//Accelerometer
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

int initialRoll;
int initialPitch;
int initialYaw;

//---------------------------------------------

// y orientation
int DOWN = 0;
int UP = 1;
int FLAT = 2;

// x orientation
int NOT_TILTED = 0;
int TILTED_RIGHT = 1;
int TILTED_LEFT = 2;

// the tilt previously recorded from the last frame cycle
int prevTilt = NOT_TILTED;

// the threshold for the accelerometer measurements to break before considering it a tilt
int threshold = 500;

//ws2812b
#include <FastLED.h>
#define LED_PIN     6
#define NUM_LEDS    67
CRGB leds[NUM_LEDS];
CRGB sandColor = CRGB(255, 185, 0);
CRGB emptyColor = CRGB(0, 0, 0);

int delayBetweenSandFrames = 50; // delay between sand falling
int delayBetweenCycleFrames = 500; // delay before the next frame in the entire animation

int firstSandDropDistanceFromCenterIndex = 2; // distance from the center of the led to light for the first sand drop location
int secondSandDropDistanceFromCenterIndex = 10; // distance from the center of the led to light for the second sand drop location
int thirdSandDropDistanceFromCenterIndex = 22; // distance from the center of the led to light for the third sand drop location

// index of the start from the top of the hour glass
int topStartIndex = 0;

// index of the end from the top of the hour glass
int topEndIndex = 32;

// index of the center of the hour glass
int centerIndex = 33;

// index of the start from the bottom of the hour glass
int bottomStartIndex = 34;

// index of the end from the bottom of the hour glass
int bottomEndIndex = 66;

// current count of the leds on the bottom of the hour glass
int countLedsOnBottom = 0;

// current count of the leds on the top of the hour glass. Assume we start with the hour glass in proper orientation not tilted
int countLedsOnTop = 25;

// a int array that maps the number of lit leds in a half glass to an array of values indicating
// whether or not to light a certain index from the middle of the hour glass. Remember we are in a snaking formation
// these are the coordinates on the top of the hour glass
int bottomPileLedsCoordinates[26][33] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0
  }, // 5
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0
  }, // 10
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1
  }, // 15
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  }, // 20
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 1, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  }, // 25
};

// a int array that maps the number of lit leds in a half glass to an array of values indicating
// whether or not to light a certain index from the middle of the hour glass. Remember we are in a snaking formation
// these are the coordinates when the glass is not tilted
int topPileLedsCoordinates[26][33] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
  {
    1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 0, 1,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    0, 0, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 0, 0, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  }, // 5
  {
    1, 1, 1,
    1, 0, 1, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  }, // 10
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1, 1,
    0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1, 1,
    1, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  }, // 15
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  }, // 20
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 0, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  },
  {
    1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
  }, // 25
};

// a int array that maps the number of lit leds in a half glass to an array of values indicating
// whether or not to light a certain index from the middle of the hour glass. Remember we are in a snaking formation
// these are the coordinates when the glass is tilted left
int tiltLeftBottomLedsCoordinates[26][33] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0
  }, // 5
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0
  }, // 10
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0
  },
  {
    0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0
  }, // 15
  {
    0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    1, 0, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 0,
    1, 1, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  }, // 20
  {
    0, 0, 1,
    1, 1, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
  },
  {
    0, 0, 1,
    1, 1, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 1,
    1, 1, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 1,
    1, 1, 0, 0,
    0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0
  },
  {
    0, 0, 1,
    1, 1, 0, 0,
    0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0
  }, // 25
};

// a int array that maps the number of lit leds in a half glass to an array of values indicating
// whether or not to light a certain index from the middle of the hour glass. Remember we are in a snaking formation
// these are the coordinates when the glass is tilted right
int tiltRightBottomLedsCoordinates[26][33] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1
  }, // 5
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1
  }, // 10
  {
    0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 0,
    1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 1,
    1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1
  }, // 15
  {
    0, 0, 0,
    0, 0, 0, 1,
    1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 1,
    1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 1,
    1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 0, 1,
    1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  },
  {
    0, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  }, // 20
  {
    1, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1
  },
  {
    1, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1
  },
  {
    1, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1
  },
  {
    1, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1
  },
  {
    1, 0, 0,
    0, 0, 1, 1,
    1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1
  }, // 25
};

//---------------------------------------------

void setup() {
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission

  //Serial.begin(9600);


  pinMode(buzzerPin, OUTPUT);

  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();

  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 0.09; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 4.71; // AccErrorY ~(-1.58)

  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 2.13; // GyroErrorX ~(-0.56)
  GyroY = GyroY + 0.11; // GyroErrorY ~(2)
  GyroZ = GyroZ + 2.04; // GyroErrorZ ~ (-0.8)

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  initialYaw =  initialYaw + GyroZ * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  initialRoll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  initialPitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  // Print the values on the serial monitor
  Serial.print(initialRoll);
  Serial.print("/");
  Serial.print(initialPitch);
  Serial.print("/");
  Serial.println(initialYaw);

  delay(20);

  //---------------------------------------------

  pinMode(buzzerPin, OUTPUT);

  //---------------------------------------------

  // init fast led
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  clearLeds();

}

//---------------------------------------------

void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 0.09; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 4.71; // AccErrorY ~(-1.58)

  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 2.13; // GyroErrorX ~(-0.56)
  GyroY = GyroY + 0.11; // GyroErrorY ~(2)
  GyroZ = GyroZ + 2.04; // GyroErrorZ ~ (-0.8)

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;



  //---------------------------------------------

  //mma.read();
  dropSand();
}

//---------------------------------------------



void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  //Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}

void dropSand() {
  int tilt = NOT_TILTED;
  if (roll < initialRoll) {
    tilt = TILTED_RIGHT;
    Serial.println("right tilt");
  } else if (roll > initialRoll) {
    tilt = TILTED_LEFT;
    Serial.println("left tilt");
  }

  int dir = FLAT;
  if (pitch > initialPitch) {
    dir = UP;
    Serial.println("up");
  } else if (pitch < initialPitch) {
    dir = DOWN;
    Serial.println("down");
  }
  dir = DOWN;
  dropSandPileAnimation(dir, tilt);
  prevTilt = tilt;
}

void dropSandPileAnimation(int dir, int tilt) {
  if (dir == DOWN) {
    dropSandFromTopPile(tilt);
  } else if (dir == UP) {
    dropSandFromBottomPile(tilt);
  }
}

void dropSandFromBottomPile(int tilt) {
  // if we are tilting the glass or we have sand that can drop, run the animation
  if (countLedsOnBottom > 0 || tilt != prevTilt) {
    if (countLedsOnBottom > 0) {
      countLedsOnTop += 1;
      countLedsOnBottom -= 1;
    }
    dropSandFromBottomPileAnimation(tilt);
  }
}

void dropSandFromTopPile(int tilt) {
  // if we are tilting the glass or we have sand that can drop, run the animation
  if (countLedsOnTop > 0 || tilt != prevTilt) {
    if (countLedsOnTop > 0) {
      countLedsOnTop -= 1;
      countLedsOnBottom += 1;
    }
    dropSandFromTopPileAnimation(tilt);
  }
}

void dropSandFromBottomPileAnimation(int tilt) {
  if (countLedsOnBottom > 0) {
    /// ----- START FIRST FRAME ----- ///
    // drop sand from top and set center index
    setLedsForBottomHalf(topPileLedsCoordinates[countLedsOnBottom]);
    leds[centerIndex] = sandColor;
    FastLED.show();
    delay(delayBetweenSandFrames);

    /// ----- START SECOND FRAME ----- ///
    // keep dropping sand
    leds[centerIndex] = emptyColor;
    boolean isFirstLedAlreadyLit = leds[centerIndex - firstSandDropDistanceFromCenterIndex] == sandColor;
    leds[centerIndex - firstSandDropDistanceFromCenterIndex] = sandColor;
    FastLED.show();
    delay(delayBetweenSandFrames);

    /// ----- START THIRD FRAME ----- ///
    // keep dropping sand if the led is not lit that we are going to fall into
    boolean isSecondLedAlreadyLit = leds[centerIndex - secondSandDropDistanceFromCenterIndex] == sandColor;
    if (!isFirstLedAlreadyLit) {
      leds[centerIndex - firstSandDropDistanceFromCenterIndex] = emptyColor;
      leds[centerIndex - secondSandDropDistanceFromCenterIndex] = sandColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }

    /// ----- START FOURTH FRAME ----- ///
    // keep dropping sand if the led is not lit that we are going to fall into
    boolean isThirdLedAlreadyLit = leds[centerIndex - thirdSandDropDistanceFromCenterIndex] == sandColor;
    if (!isSecondLedAlreadyLit) {
      leds[centerIndex - secondSandDropDistanceFromCenterIndex] = emptyColor;
      leds[centerIndex - thirdSandDropDistanceFromCenterIndex] = sandColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }

    /// ----- START FIFTH FRAME ----- ///
    // clear out the last sand drop location if it is not already lit
    if (!isThirdLedAlreadyLit) {
      leds[centerIndex - secondSandDropDistanceFromCenterIndex] = emptyColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }
  } else if (countLedsOnBottom == 0) {
    leds[bottomStartIndex] = emptyColor;
    tone(buzzerPin , 500);
  }

  /// ----- START SIXTH FRAME ----- ///
  // depending on tilt, draw sand on the bottom
  if (tilt == TILTED_LEFT) {
    setLedsForTopHalf(tiltLeftBottomLedsCoordinates[countLedsOnTop]);
  } else if (tilt == TILTED_RIGHT) {
    setLedsForTopHalf(tiltRightBottomLedsCoordinates[countLedsOnTop]);
  } else {
    setLedsForTopHalf(bottomPileLedsCoordinates[countLedsOnTop]);
  }
  FastLED.show();
  delay(delayBetweenCycleFrames);
}

void dropSandFromTopPileAnimation(int tilt) {
  if (countLedsOnTop > 0) {
    /// ----- START FIRST FRAME ----- ///
    // drop sand from top and set center index
    setLedsForTopHalf(topPileLedsCoordinates[countLedsOnTop]);
    leds[centerIndex] = sandColor;
    FastLED.show();
    delay(delayBetweenSandFrames);

    /// ----- START SECOND FRAME ----- ///
    // keep dropping sand
    leds[centerIndex] = emptyColor;
    boolean isFirstLedAlreadyLit = leds[centerIndex + firstSandDropDistanceFromCenterIndex] == sandColor;
    leds[centerIndex + firstSandDropDistanceFromCenterIndex] = sandColor;
    FastLED.show();
    delay(delayBetweenSandFrames);

    /// ----- START THIRD FRAME ----- ///
    // keep dropping sand if the led is not lit that we are going to fall into
    boolean isSecondLedAlreadyLit = leds[centerIndex + secondSandDropDistanceFromCenterIndex] == sandColor;
    if (!isFirstLedAlreadyLit) {
      leds[centerIndex + firstSandDropDistanceFromCenterIndex] = emptyColor;
      leds[centerIndex + secondSandDropDistanceFromCenterIndex] = sandColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }

    /// ----- START FOURTH FRAME ----- ///
    // keep dropping sand if the led is not lit that we are going to fall into
    boolean isThirdLedAlreadyLit = leds[centerIndex + thirdSandDropDistanceFromCenterIndex] == sandColor;
    if (!isSecondLedAlreadyLit) {
      leds[centerIndex + secondSandDropDistanceFromCenterIndex] = emptyColor;
      leds[centerIndex + thirdSandDropDistanceFromCenterIndex] = sandColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }

    /// ----- START FIFTH FRAME ----- ///
    // clear out the last sand drop location if it is not already lit
    if (!isThirdLedAlreadyLit) {
      leds[centerIndex + secondSandDropDistanceFromCenterIndex] = emptyColor;
      FastLED.show();
      delay(delayBetweenSandFrames);
    }
  } else if (countLedsOnTop == 0) {
    leds[topEndIndex] = emptyColor;
    tone(buzzerPin , 500);
  }

  /// ----- START SIXTH FRAME ----- ///
  // depending on tilt, draw sand on the bottom
  if (tilt == TILTED_RIGHT) {
    setLedsForBottomHalf(tiltLeftBottomLedsCoordinates[countLedsOnBottom]);
  } else if (tilt == TILTED_LEFT) {
    setLedsForBottomHalf(tiltRightBottomLedsCoordinates[countLedsOnBottom]);
  } else {
    setLedsForBottomHalf(bottomPileLedsCoordinates[countLedsOnBottom]);
  }
  FastLED.show();
  delay(delayBetweenCycleFrames);
}

void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = emptyColor;
  }
  Serial.println("test1");
}

// ledsToLight - the coordinates of the leds to light (1 for on, 0 for off)
void setLedsForBottomHalf(int ledsToLight[]) {
  uint16_t ms = millis();  // rainbow pattern
  for (int i = 0; i < centerIndex; i++) {
    Serial.println("test1");
    leds[bottomStartIndex + i] = ledsToLight[i] == 1 ? CHSV( ms / 41, 200, 255) : emptyColor;
  }
}

// ledsToLight - the coordinates of the leds to light (1 for on, 0 for off)
void setLedsForTopHalf(int ledsToLight[]) {
  uint16_t ms = millis();  // rainbow pattern
  for (int i = 0; i < centerIndex; i++) {
    Serial.println("test2");
    leds[topEndIndex - i] = ledsToLight[i] == 1 ? CHSV( ms / 53, 200, 255) : emptyColor;
  }
}
