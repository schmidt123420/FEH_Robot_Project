/**
    Implementations for all functions that control the robot
**/

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


void moveDistance(int motorPower, bool direction, float distance) {
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set distance as inches to drive * EXPECTED_COUNTS_PER_INCH
    distance = distance * EXPECTED_COUNTS_PER_INCH;

    //Set both motors to desired percent
    //multiply by -1 because set up motors backwards
    if (direction == FORWARD) {
        rightMotor.SetPercent(-1 * motorPower);
        leftMotor.SetPercent(-1 * motorPower);
    }
    else {
        rightMotor.SetPercent(motorPower);
        leftMotor.SetPercent(motorPower);
    }

    //While the average of the left and right encoder is less than distance,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < distance);

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

void turnRight(int motorPower, int angle) {
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent. switched multiply by -1 because motors backwards.
    rightMotor.SetPercent(motorPower);
    leftMotor.SetPercent(-1 * motorPower);

    //Calculate counts to turn
    int counts = (318*PI*BOT_WIDTH*angle)/(360*PI*WHEEL_DIAMETER);

    /*
    While the average of the left and right encoder is less 
    than counts keep running the robot
    */
    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts) {
        //do nothing
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

void turnLeft(int motorPower, int angle) {
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent. switched multiply by -1 because motors backwards.
    rightMotor.SetPercent(-1 * motorPower);
    leftMotor.SetPercent(motorPower);

    //Calculate counts to turn
    int counts = (318*PI*BOT_WIDTH*angle)/(360*PI*WHEEL_DIAMETER);

    /*
    While the average of the left and right encoder is less 
    than counts keep running the robot
    */
    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts) {
        //do nothing
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}


int waitUntilLight() {
    float cdsValue = cdsCell.Value();
    while (cdsValue > 1.75) {
        //wait until light turns on
        cdsValue = cdsCell.Value();
    }
    LCD.Write("CDS Value: ");
    LCD.WriteLine(cdsValue);
    LCD.WriteLine("");

    //If light is blue, return blue. Otherwise return red
    if (cdsValue > 1.14) {
        return BLUE_COLOR;
    }
    else {
        return RED_COLOR;
    }
}

void driveUntilWall(int motorPower, bool direction) {
    //If forward set motor percentage as -motorPower
    if (direction == FORWARD) {
        rightMotor.SetPercent(-1 * motorPower);
        leftMotor.SetPercent(-1 * motorPower);

        //Keep moving until both switches have been pressed, adjust if one has been pressed
        while (microSwitchFrontRight.Value() || microSwitchFrontLeft.Value()) {
            if (microSwitchFrontRight.Value() && microSwitchFrontLeft.Value()) {
                //do nothing
            }
            else if (!microSwitchFrontLeft.Value()) {
                leftMotor.Stop();
            }
            else if (!microSwitchFrontRight.Value()) {
                rightMotor.Stop();
            }
        }
    }
    else {
        rightMotor.SetPercent(motorPower);
        leftMotor.SetPercent(motorPower);

        //Keep moving until both switches have been pressed, adjust if one has been pressed
        while (microSwitchBackRight.Value() || microSwitchBackLeft.Value()) {
            if (microSwitchBackRight.Value() && microSwitchBackLeft.Value()) {
                //do nothing
            }
            else if (!microSwitchBackLeft.Value()) {
                leftMotor.Stop();
            }
            else if (!microSwitchBackRight.Value()) {
                rightMotor.Stop();
            }
        }
    }

    rightMotor.Stop();
    leftMotor.Stop();
}

void driveUntilLine(int motorPower, bool direction) {
    if (direction == FORWARD) {
        //Set both motors to desired percent. multiplied by -1 because motors backwards
        rightMotor.SetPercent(-1 * motorPower);
        leftMotor.SetPercent(-1 * motorPower);
    }
    else {
        rightMotor.SetPercent(motorPower);
        leftMotor.SetPercent(motorPower);
    }

    //Drive until reach black line
    while (leftOpt.Value() <= LEFT_OPT_THRESHOLD && rightOpt.Value() <= RIGHT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
        //if right is on black line, turn off right motor
        if (rightOpt.Value() > RIGHT_OPT_THRESHOLD) {
            rightMotor.Stop();
        }
        //if left is on black line, turn off left motor
        else if (leftOpt.Value() > LEFT_OPT_THRESHOLD) {
            leftMotor.Stop();
        }
        //if both stopped exit loop
        else if (rightOpt.Value() > RIGHT_OPT_THRESHOLD && leftOpt.Value() > LEFT_OPT_THRESHOLD) {
            break;
        }
    }
    rightMotor.Stop();
    leftMotor.Stop();
}


void followLineForDistance(float distance) {
    int state = MIDDLE; // Set the initial state

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set distance as inches to drive * EXPECTED_COUNTS_PER_INCH
    distance = distance * EXPECTED_COUNTS_PER_INCH;

    /*
        While the average of the left and right encoders is less than
        distance, keep following the line
    */
    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < distance) {
        LCD.WriteLine("State: " + state);
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight
                leftMotor.SetPercent(-25);
                rightMotor.SetPercent(-25);
                /* Drive */

                //if loes contact with left and middle, turn right 
                if (leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }
                
                //if lose contact with right and middle, turn left
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }
                 
                 /* If Right sensor is on line */ 
                else if (rightOpt.Value() > RIGHT_OPT_THRESHOLD) {
                    state = RIGHT; // update a new state
                }

                /* Code for if left sensor is on the line */
                else if (leftOpt.Value() > LEFT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;

            // If the right sensor is on the line...
            case RIGHT:
                // Set motor powers for right turn
                leftMotor.SetPercent(-25);
                rightMotor.Stop();
                /* Drive */
                
                /*IF I no longer need to turn right... */
                if(middleOpt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if left and not right, turn left
                else if (leftOpt.Value() > LEFT_OPT_THRESHOLD && rightOpt.Value() <= RIGHT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                //Code for if no optosensor sees the line
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;
                
            // If the left sensor is on the line...
            case LEFT:
                /* Mirror operation of RIGHT state */
                // Set motor powers for left turn
                leftMotor.Stop();
                rightMotor.SetPercent(-25);
                /* Drive */
                
                /*IF I no longer need to turn left... */
                if(middleOpt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if right and not left, turn right
                else if (rightOpt.Value() > RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                //Code for if no optosensor sees the line
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                break;
            
            default: // Error. Something is very wrong.
                break;
        }
    }

    //Stop both motors
    leftMotor.Stop();
    rightMotor.Stop();
}

void followLineUntilWall() {
    int state = MIDDLE; // Set the initial state

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    /*
        Keep following the line until a switch has been pressed
    */
    while (microSwitchFrontRight.Value() && microSwitchFrontLeft.Value()) {
        LCD.WriteLine("State: " + state);
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight
                leftMotor.SetPercent(-25);
                rightMotor.SetPercent(-25);
                /* Drive */

                //if loes contact with left and middle, turn right 
                if (leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }
                
                //if lose contact with right and middle, turn left
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }
                 
                 /* If Right sensor is on line */ 
                else if (rightOpt.Value() > RIGHT_OPT_THRESHOLD) {
                    state = RIGHT; // update a new state
                }

                /* Code for if left sensor is on the line */
                else if (leftOpt.Value() > LEFT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;

            // If the right sensor is on the line...
            case RIGHT:
                // Set motor powers for right turn
                leftMotor.SetPercent(-25);
                rightMotor.Stop();
                /* Drive */
                
                /*IF I no longer need to turn right... */
                if(middleOpt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if left and not right, turn left
                else if (leftOpt.Value() > LEFT_OPT_THRESHOLD && rightOpt.Value() <= RIGHT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                //Code for if no optosensor sees the line
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;
                
            // If the left sensor is on the line...
            case LEFT:
                /* Mirror operation of RIGHT state */
                // Set motor powers for left turn
                leftMotor.Stop();
                rightMotor.SetPercent(-25);
                /* Drive */
                
                /*IF I no longer need to turn left... */
                if(middleOpt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if right and not left, turn right
                else if (rightOpt.Value() > RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                //Code for if no optosensor sees the line
                else if (rightOpt.Value() <= RIGHT_OPT_THRESHOLD && leftOpt.Value() <= LEFT_OPT_THRESHOLD && middleOpt.Value() <= MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                break;
            
            default: // Error. Something is very wrong.
                break;
        }
    }

    //Call driveUntilWall to line up with wall
    driveUntilWall(25, FORWARD);
}


void setArmStart() {
    bottomServo.SetMin(BOTTOM_SERVO_MIN);
    bottomServo.SetMax(BOTTOM_SERVO_MAX);
    middleServo.SetMin(MIDDLE_SERVO_MIN);
    middleServo.SetMax(MIDDLE_SERVO_MAX);
    topServo.SetMin(TOP_SERVO_MIN);
    topServo.SetMax(TOP_SERVO_MAX);

    topServo.SetDegree(180);
    Sleep(0.5);
    middleServo.SetDegree(30);
    Sleep(0.5);
    bottomServo.SetDegree(150);
    Sleep(0.5);
}

void dropTray() {

}

void setArmPositionTicketSlide() {

}

void slideTicket() {

}