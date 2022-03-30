#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <time.h> 

#include "robomovement.h"
#include "roboconstants.h"
#include "roboports.h"


int main(void) {
<<<<<<< HEAD
    followLineUntilWall();
    return 0;
}
=======
//bottomServo.TouchCalibrate();

    int expectedCountsPerInch = 318/(WHEEL_DIAMETER * PI); //Input theoretical counts

    bottomServo.SetMin(BOTTOM_SERVO_MIN);
    bottomServo.SetMax(BOTTOM_SERVO_MAX);
    middleServo.SetMin(MIDDLE_SERVO_MIN);
    middleServo.SetMax(MIDDLE_SERVO_MAX);
    topServo.SetMin(TOP_SERVO_MIN);
    topServo.SetMax(TOP_SERVO_MAX);
    //Start with arm in upright starting position
    setArmStart();
    
    //Wait until light turns on
    int firstLight = waitUntilLight();

    //Print to LCD that sees light
    if (firstLight == BLUE_COLOR) {
        LCD.WriteLine("Light color: Blue");
    }
    else {
        LCD.WriteLine("Light color: Red");
    }

    //Move forward 13.5 inches
    moveDistance(35,FORWARD,13.5);
    // Turn 45 right
    Sleep(.5);
    turnRight(25,45);
    // Move forward 40 inches (up the ramp)
    moveDistance(40,FORWARD, 36);
    // turn left to bump into wall
    
}
>>>>>>> 41f53b10f7a6389138bdbee4b805720b8b62fd99
