#include <Arduino.h>
#include <../include/Helpers.h>

long timingError = 0;

void setup() {
  // Setting up the pins
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);

  // Accounting for an error in timing
  timingError = getTriggerTimingError();

  // Setting the speed of the vehicle
  setSpeed(150);

  // Wait a while so that we can have time
  delay(3000);
}

void loop() {
  float distance, leftDistance, rightDistance;

  distance = readDistance(timingError);
  // First checking if the distance read is valid
  if (distance > 0)
  {
    // Confirm if the distance is within threshold distance
    if (distance < THRESHOLD_DISTANCE)
    {
      // Stop the vehicle and reverse
      stopMotors();
      delay(800);
      reverse();

      // Turn to the left and measure the distance
      turnLeft();
      leftDistance = readDistance(timingError);

      // Confirming left distance validity and moving if there is space
      if (leftDistance > 0 && leftDistance > THRESHOLD_DISTANCE)
      {
        moveForward();
      }
      else
      {
        // Return to original position
        turnRight();
        delay(1000);

        // Turn to the right and measure the distance
        turnRight();
        rightDistance = readDistance(timingError);
        if (rightDistance > THRESHOLD_DISTANCE)
        {
          // Keep moving if there is space on the right
          moveForward();
        }
        else
        {
          // Rotate the car in the opposite direction and wait
          rotateTo(180);
          delay(700);
        }
      }
    }
    else
    {
      // Keep going if the distance is more than the threshold
      moveForward();
    }
  }
  else
  {
    // Wait for a while if there are bad readings
    stopMotors();
    delay(100);
  }
}
