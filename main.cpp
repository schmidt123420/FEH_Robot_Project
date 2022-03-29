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
//bottomServo.TouchCalibrate();

    int motorPercent = 40;
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

    // Move forward 13.5 inches
    moveDistance(35,1,13.5*EXPECTED_COUNTS_PER_INCH);
    // Turn 45 right
    Sleep(.5);
    turnRight(25,45);
    // Move forward 35 inches (up the ramp)
    moveDistance(45,1, 35*EXPECTED_COUNTS_PER_INCH);
    turnLeft(25,90);
    moveDistance(25,1,2*EXPECTED_COUNTS_PER_INCH);
    Sleep(.5);
    turnLeft(25,10);
    Sleep(.5);
    // int t = TimeNow();
    // while(t<TimeNow()+2.0){
    //      driveUntilLine();
    //  }
    followLineForDistance(2);
    dropTray();
}
