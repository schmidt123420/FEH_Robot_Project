/*
* Code for checkpoint test 1
*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

//Define constants
#define PI 3.141592653589793
#define BOT_WIDTH 9 //width of crayola bot
#define WHEEL_DIAMETER 3 //diameter of robot wheel
#define BLUE_COLOR 0
#define RED_COLOR 1
#define LEFT_OPT_THRESHOLD 3.1
#define RIGHT_OPT_THRESHOLD 2.7
#define MIDDLE_OPT_THRESHOLD 2.9
#define LEFT_TUNING_CONST 0.9
#define RIGHT_TUNING_CONST 0.78

//Define inputs and outputs
AnalogInputPin cdsCell(FEHIO::P0_0);
// FEHServo servo(FEHServo::Servo0); //declare servo arm
// DigitalInputPin microSwitchFrontRight(FEHIO::P0_1);
// DigitalInputPin microSwitchFrontLeft(FEHIO::P3_7);
// DigitalInputPin microSwitchBackRight(FEHIO::P0_2);
// DigitalInputPin microSwitchBackLeft(FEHIO::P3_6);
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P3_5);
FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor3, 9.0);

//Optosensors for checking light
AnalogInputPin right_opt(FEHIO::P1_0);
AnalogInputPin middle_opt(FEHIO::P1_1);
AnalogInputPin left_opt(FEHIO::P1_2);


/*! Drive forward given distance using encoders.
*
* To drive forward 14 inches, ex:
* move_forward(motor_percent, 14 * counts);
*
* @param percent = motor power percent
* @param counts = encoder counts for distance to travel
*/
void move_forward(int percent, int counts)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent. multiply by -1 because set up motors backwards
    rightMotor.SetPercent(-1 * percent);
    leftMotor.SetPercent(-1 * percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts);

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

/*! Drive backward given distance using encoders.
*
* To drive backward 14 inches, ex:
* move_backward(motor_percent, 14 * counts);
*
* @param percent = motor power percent
* @param counts = encoder counts for distance to travel
*/
void move_backward(int percent, int counts)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent. multiply by -1 because set up motors backwards
    rightMotor.SetPercent(percent);
    leftMotor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts);

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

/*! Turn to the right given angle using encoders.
*
* To turn right 90 degrees, ex:
* turn_right(20, 90);
*
* @param motorPower = motor power percent
* @param angle = angle to turn
*/
void turn_right(int motorPower, int angle) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent. switched multiply by -1 because motors backwards.
    rightMotor.SetPercent(motorPower);
    leftMotor.SetPercent(-1 * motorPower);

    //Calculate counts to turn
    int counts = (318*PI*BOT_WIDTH*angle)/(360*PI*WHEEL_DIAMETER);
    counts = counts * RIGHT_TUNING_CONST * 1.14;

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts) {
        //do nothing
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

/*! Turn to the left given angle using encoders.
*
* To turn left 90 degrees, ex:
* turn_left(20, 90);
*
* @param motorPower = motor power percent
* @param angle = angle to turn
*/
void turn_left(int motorPower, int angle) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent. negative because motors hooked up backwards
    rightMotor.SetPercent(-1 * motorPower);
    leftMotor.SetPercent(1 * motorPower);

    //Calculate counts to turn
    int counts = (318*PI*BOT_WIDTH*angle)/(360*PI*WHEEL_DIAMETER);

    if (angle == 45) {
        counts = counts * (LEFT_TUNING_CONST - 0.075) * 1.1;
    }
    else if (angle == 90) {
        counts = counts * (LEFT_TUNING_CONST - 0.075) * 1.1;
    }
    

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts) {
        //do nothing
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}



/*! 
*  Drives until robot is above a black line
*/
void driveForwardUntilBlackLine(int motorPower) {
    //Set both motors to desired percent. multiplied by -1 because motors backwards
    rightMotor.SetPercent(-1 * motorPower);
    leftMotor.SetPercent(-1 * motorPower);

    //Drive until reach black line
    while (left_opt.Value() <= LEFT_OPT_THRESHOLD && right_opt.Value() <= RIGHT_OPT_THRESHOLD && middle_opt.Value() <= MIDDLE_OPT_THRESHOLD) {
        //if right is on black line, turn off right motor
        if (right_opt.Value() > RIGHT_OPT_THRESHOLD) {
            rightMotor.Stop();
        }
        //if left is on black line, turn off left motor
        if (left_opt.Value() > LEFT_OPT_THRESHOLD) {
            leftMotor.Stop();
        }

        //if both stopped exit loop
        if (right_opt.Value() > RIGHT_OPT_THRESHOLD && left_opt.Value() > LEFT_OPT_THRESHOLD) {
            break;
        }
    }
    rightMotor.Stop();
    leftMotor.Stop();
}


enum LineStates {
    MIDDLE,
    RIGHT,
    LEFT
};

/*! 
*   Follows line until end of line
*/
void followLineToEnd() {
    int state = MIDDLE; // Set the initial state
    bool atEndOfLine = false;
    while (!atEndOfLine) { //follow line until end of line
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight. multiplied by -1 because motors backwards
                leftMotor.SetPercent(-1 * 25);
                rightMotor.SetPercent(-1 * 25);
                /* Drive */

                //No optosensor sees the line, have reached the end of the line
                if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    atEndOfLine = true;
                    break;
                }

                //if loes contact with left and middle, turn right 
                else if (left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }
                
                //if lose contact with right and middle, turn left
                else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }
                 
                 /* If Right sensor is on line */ 
                else if (right_opt.Value() > RIGHT_OPT_THRESHOLD) {
                    state = RIGHT; // update a new state
                }

                /* Code for if left sensor is on the line */
                else if (left_opt.Value() > LEFT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;

            // If the right sensor is on the line...
            case RIGHT:
                // Set motor powers for right turn
                leftMotor.SetPercent(-25);
                rightMotor.Stop();
                /* Drive */
                
                //No optosensor sees the line, have reached the end of the line
                if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    atEndOfLine = true;
                    break;
                }

                /*IF I no longer need to turn right... */
                else if(middle_opt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if left and not right, turn left
                else if (left_opt.Value() > LEFT_OPT_THRESHOLD && right_opt.Value() < RIGHT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                // //Code for if no optosensor sees the line
                // else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                //     state = LEFT;
                // }

                break;
                
            // If the left sensor is on the line...
            case LEFT:
                /* Mirror operation of RIGHT state */
                // Set motor powers for left turn
                leftMotor.Stop();
                rightMotor.SetPercent(-25);
                /* Drive */

                //No optosensor sees the line, have reached the end of the line
                if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    atEndOfLine = true;
                    break;
                }

                /*IF I no longer need to turn left... */
                else if (middle_opt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if right and not left, turn right
                else if (right_opt.Value() > RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                // //Code for if no optosensor sees the line
                // else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < 1.8 && middle_opt.Value() < 2) {
                //     state = RIGHT;
                // }

                break;
            
            default: // Error. Something is very wrong.
                break;
        }
    }
}

void followCurvedLine() {
    int state = MIDDLE; // Set the initial state
    while (true) { // I will follow this line forever!
        LCD.WriteLine("State: " + state);
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight
                leftMotor.SetPercent(25);
                rightMotor.SetPercent(25);
                /* Drive */

                //if loes contact with left and middle, turn right 
                if (left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }
                
                //if lose contact with right and middle, turn left
                else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }
                 
                 /* If Right sensor is on line */ 
                else if (right_opt.Value() > RIGHT_OPT_THRESHOLD) {
                    state = RIGHT; // update a new state
                }

                /* Code for if left sensor is on the line */
                else if (left_opt.Value() > LEFT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;

            // If the right sensor is on the line...
            case RIGHT:
                // Set motor powers for right turn
                leftMotor.SetPercent(25);
                rightMotor.Stop();
                /* Drive */
                
                /*IF I no longer need to turn right... */
                if(middle_opt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if left and not right, turn left
                else if (left_opt.Value() > LEFT_OPT_THRESHOLD && right_opt.Value() < RIGHT_OPT_THRESHOLD) {
                    state = LEFT;
                }

                //Code for if no optosensor sees the line
                else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = LEFT;
                }

                break;
                
            // If the left sensor is on the line...
            case LEFT:
                /* Mirror operation of RIGHT state */
                // Set motor powers for left turn
                leftMotor.Stop();
                rightMotor.SetPercent(25);
                /* Drive */

                
                /*IF I no longer need to turn left... */
                if(middle_opt.Value() > MIDDLE_OPT_THRESHOLD) {
                    /* update a new state */
                    state = MIDDLE;
                }

                //if right and not left, turn right
                else if (right_opt.Value() > RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                //Code for if no optosensor sees the line
                else if (right_opt.Value() < RIGHT_OPT_THRESHOLD && left_opt.Value() < LEFT_OPT_THRESHOLD && middle_opt.Value() < MIDDLE_OPT_THRESHOLD) {
                    state = RIGHT;
                }

                break;
            
            default: // Error. Something is very wrong.
                break;
        }
    }
}


/*! Waits until sees light turned on and reports color of light
*
* @return color of light
*/
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

int main(void) {
    int motorPercent = 25;
    int expectedCountsPerInch = 318/(WHEEL_DIAMETER * PI); //Input theoretical counts

    // turn_left(25, 90);
    // turn_left(25, 45);
    // turn_right(25, 90);

    // // Sleep(2.0);
    // // move_forward(motorPercent, 30 * expectedCountsPerInch);
    // // move_backward(motorPercent, 30 * expectedCountsPerInch);

    //Wait until light turns on
    int firstLight = waitUntilLight();

    //Print to LCD that sees light and move forward 15in
    if (firstLight == BLUE_COLOR) {
        LCD.WriteLine("Light color: Blue");
    }
    else {
        LCD.WriteLine("Light color: Red");
    }
    
    move_forward(motorPercent, 16 * expectedCountsPerInch);
    turn_left(motorPercent, 45);
    move_forward(motorPercent, 7.5 * expectedCountsPerInch);
    turn_left(motorPercent, 90);
    move_forward(motorPercent, 1 * expectedCountsPerInch);

    Sleep(1.0);

    //Wait until light turns on
    int secondLight = waitUntilLight();

    //Print to LCD that sees light and move forward 15in
    if (secondLight == BLUE_COLOR) {
        LCD.WriteLine("2nd Light color: Blue");
        turn_left(motorPercent, 2.9);
        move_forward(motorPercent, 4.25 * expectedCountsPerInch);

        //go backwards
        turn_right(motorPercent, 5);
        move_backward(motorPercent, 4.25 * expectedCountsPerInch);
    }
    else {
        LCD.WriteLine("2nd Light color: Red");
        turn_right(motorPercent, 5);
        move_forward(motorPercent, 4.25 * expectedCountsPerInch);

        //go backwards
        turn_left(motorPercent, 5);
        move_backward(motorPercent, 4.25 * expectedCountsPerInch);
    }

    turn_left(motorPercent, 90);
    move_forward(motorPercent, 9 * expectedCountsPerInch);
    turn_left(motorPercent, 90);
    move_forward(motorPercent, 25 * expectedCountsPerInch);
    move_backward(motorPercent, 25 * expectedCountsPerInch);
}
