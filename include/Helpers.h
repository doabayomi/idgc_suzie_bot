#if !defined(HELPER_H_)
#define HELPER_H_

#include <Arduino.h>

#define IN_1 8
#define IN_2 9
#define IN_3 10
#define IN_4 11
#define TRIG_PIN 3
#define ECHO_PIN 4

/**
 * The EN_A and EN_B pins might not be used in the final
 * code as the jumpers already supply voltage to the car.
*/
#define EN_A 5
#define EN_B 6

#define ROTATION_TIME 800
#define THRESHOLD_DISTANCE 30
#define REVERSE_TIME 1000

/**
 * Creating callibration functions so that each arduino does not worry
 * about it's state of calculation when it is working
*/
long getTriggerTimingError()
{
    unsigned long start_time = micros();
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    unsigned long end_time = micros();
    unsigned long duration = end_time - start_time;
    return (duration);
}


/**
 * @brief reading distance while accounting for timeouts and delays
 * @param trigError 
 * @return Approximate distance in front of the sensor in centimetres.
 */
float readDistance(long trigError)
{
    // Send a pulse to trigger the sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the time it takes for the echo to return
    unsigned long start_time = micros();
    while (digitalRead(ECHO_PIN) == LOW) {
        if (micros() - start_time > 10000) { // Timeout after 10ms
        return -1.0; // Handle timeout error
        }
    }
    start_time = micros();
    while (digitalRead(ECHO_PIN) == HIGH) {
        if (micros() - start_time > 10000) { // Timeout after 10ms
        return -1.0; // Handle timeout error
        }
    }

    unsigned long end_time = micros();
    unsigned long duration = end_time - start_time;

    // Account for the 20 microsecond delay
    duration -= trigError;

    // Convert the time to distance (in centimeters)
    float distance = (duration / 2) * 0.0343;

    return distance;
}

/**
 * @brief Moves the vehicle forward continuously
 */
void moveForward()
{
    // Going int rhe forward direction in the right and left side.
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_3, HIGH);

    digitalWrite(IN_2, LOW);
    digitalWrite(IN_4, LOW);
}

/**
 * @brief Moves the vehicle back continuously
 */
void moveBackwards()
{
    // Going in the backward direction in the right and left side
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_4, HIGH);

    digitalWrite(IN_1, LOW);
    digitalWrite(IN_3, LOW);
}

/**
 * @brief Moves the vehicle left continuously
 */
void moveLeft()
{
    // Continues moving left
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_4, LOW);

    digitalWrite(IN_1, LOW);
    digitalWrite(IN_3, HIGH);
}

/**
 * @brief Moves the vehicle right continuously
 */
void moveRight()
{
    // Continues moving right
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_4, HIGH);

    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_3, LOW);

}

/**
 * @brief Stops all motors in the vehicle
 */
void stopMotors()
{
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_4, LOW);

    digitalWrite(IN_1, LOW);
    digitalWrite(IN_3, LOW);
}



// Custom movement functions based on move Functions
/**
 * @brief Turns the vehicle left at a right angle
 */
void turnLeft()
{
    moveLeft();
    delay(ROTATION_TIME);
    stopMotors();
}

/**
 * @brief Turns the vehicle right at a right angle
 */
void turnRight()
{
    moveRight();
    delay(ROTATION_TIME);
    stopMotors();
}

/**
 * @brief Rotates the vehicle to a specific angle.
 * @param angle 
 */
void rotateTo(int angle)
{
    /**
     * Assuming I have gotten an estimate for ROTATION_TIME which is to get 
     * right angles...
    */
    int timeToMove = (ROTATION_TIME / 90) * angle;
    moveRight();
    delay(timeToMove);
    stopMotors();
}

/**
 * @brief Briefly moves the car backwards with the aim of creating space.
 */
void reverse()
{
    moveBackwards();
    delay(REVERSE_TIME);
    stopMotors();
}

void setSpeed(int speed)
{
    if (speed > 255 || speed < 10)
    {
        analogWrite(EN_A, 255);
        analogWrite(EN_B, 255);
    }
    else
    {
        analogWrite(EN_A, speed);
        analogWrite(EN_B, speed);
    }
}

#endif // HELPER_H_
