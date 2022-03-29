/*

* Testing servo motors

*/

 

#include <FEHLCD.h>

#include <FEHIO.h>

#include <FEHUtility.h>

#include <FEHServo.h>

#include <FEHMotor.h>

#include <FEHRPS.h>

#include <FEHServo.h>

#include <time.h>

DigitalInputPin microSwitchFrontRight(FEHIO::P2_0);

DigitalInputPin microSwitchFrontLeft(FEHIO::P2_2);

DigitalInputPin microSwitchBackRight(FEHIO::P2_1);

DigitalInputPin microSwitchBackLeft(FEHIO::P2_3);

 

//Define constants

#define PI 3.141592653589793

#define BOT_WIDTH 9 //width of crayola bot

#define WHEEL_DIAMETER 3 //diameter of robot wheel

#define BLUE_COLOR 0

#define RED_COLOR 1

#define LEFT_TUNING_CONST 0.9

#define RIGHT_TUNING_CONST 0.78

#define BOTTOM_SERVO_MIN 1060

#define BOTTOM_SERVO_MAX 2070

#define MIDDLE_SERVO_MIN 872

#define MIDDLE_SERVO_MAX 1950

#define TOP_SERVO_MIN 777

#define TOP_SERVO_MAX 1638

#define EXPECTED_COUNTS_PER_INCH 318/(WHEEL_DIAMETER * PI)

 

AnalogInputPin cdsCell(FEHIO::P0_0);

DigitalEncoder right_encoder(FEHIO::P0_1);

DigitalEncoder left_encoder(FEHIO::P3_5);

FEHMotor leftMotor(FEHMotor::Motor2, 9.0);

FEHMotor rightMotor(FEHMotor::Motor3, 9.0);

 

FEHServo bottomServo(FEHServo::Servo0);

FEHServo middleServo(FEHServo::Servo5);

FEHServo topServo(FEHServo::Servo7);

//BOTTOM SERVO HAS BEEN RECALIBRATED SINCE PT 2.

 

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

 

//Drive forward until hit wall

void driveForwardUntilWall() {

    rightMotor.SetPercent(-40);

    leftMotor.SetPercent(-40);

 

    while (microSwitchFrontRight.Value() || microSwitchFrontLeft.Value()) {

        if (microSwitchFrontRight.Value() && microSwitchFrontLeft.Value()) {

            //do nothing

        }

        else if (!microSwitchFrontLeft.Value()) {

            leftMotor.Stop();

        }

        else if (!microSwitchFrontRight.Value()){

            rightMotor.Stop();

        }

       

    }

    leftMotor.Stop();

    rightMotor.Stop();

 

}

 

void setArmStart() {

    topServo.SetDegree(180);

    Sleep(0.5);

    middleServo.SetDegree(30);

    Sleep(0.5);

    bottomServo.SetDegree(150);

    Sleep(0.5);

 

}

 

void dropTray() {

    bottomServo.SetDegree(165);

    topServo.SetDegree(40);

}

 

void setArmPositionTicketSlide() {

    bottomServo.SetDegree(180);

    Sleep(1.0);

    middleServo.SetDegree(180);

    Sleep(1.0);

    topServo.SetDegree(100);

}

void slideTicket() {

    bottomServo.SetDegree(60);

    Sleep(1.0);

    bottomServo.SetDegree(160);

}

void setArmPositionBurgerFlip2(){

    bottomServo.SetDegree(120);

    Sleep(.5);

    middleServo.SetDegree(180);

    Sleep(.5);

    topServo.SetDegree(60);

}

void burgerFlip2(){

    middleServo.SetDegree(120);

    topServo.SetDegree(60);

}

void burgerFlipNewIdea(int percent) {
    // move_backward(25,3*EXPECTED_COUNTS_PER_INCH);
    setArmPositionBurgerFlip2();
    // Sleep(1.0);
    // turn_left(25, 7.5);
    // Sleep(1.0);
    move_forward(25, 5.3 * EXPECTED_COUNTS_PER_INCH);
    Sleep(2.0);
    turn_right(25, 15);
    Sleep(2.0);
    

    //raise arm enough so can move arm underneath hot plate
    topServo.SetDegree(80);
    middleServo.SetDegree(140);
    bottomServo.SetDegree(100);

    //flip hot plate
    topServo.SetDegree(100);
    middleServo.SetDegree(80);
    turn_right(25, 20);
    topServo.SetDegree(70);
    move_forward(25, 3 * EXPECTED_COUNTS_PER_INCH);
    turn_right(25, 25);

    //Flip hot plate back
    Sleep(4.0);
    move_forward(25, 1.0 * EXPECTED_COUNTS_PER_INCH);
    turn_left(25, 45);
    move_backward(40, 1.0 * EXPECTED_COUNTS_PER_INCH);
}

void dropLever() {
    middleServo.SetDegree(140);
}

void travelToIceCream() {
    move_backward(40, 3 * EXPECTED_COUNTS_PER_INCH);
    Sleep(1.0);
    setArmStart();
    turn_right(25, 90);
    Sleep(1.0);
    driveForwardUntilWall();
    Sleep(1.0);
    move_backward(40, 3 * EXPECTED_COUNTS_PER_INCH);
    Sleep(1.0);
    turn_right(25, 190);
    Sleep(1.0);
    move_forward(40, 12 * EXPECTED_COUNTS_PER_INCH);
    Sleep(2.0);

    //drop lever
    turn_right(25, 15);
    Sleep(2.0);
    move_forward(25, 2 * EXPECTED_COUNTS_PER_INCH);
    Sleep(2.0);
    dropLever();  
}

 

int main(void){

    //bottomServo.TouchCalibrate();

 

    int motorPercent = 40;

 

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

 

    //Drive straight from start
    Sleep(2.0);
    move_forward(40, 13.5 * EXPECTED_COUNTS_PER_INCH);

 

    //Turn right 45 degrees

    turn_right(25, 50);

    Sleep(1.0);

   

    //Drive straight up ramp for 37 inches

    move_forward(40, 35 * EXPECTED_COUNTS_PER_INCH);

    //Line up in front of left side of burger flip
    turn_right(25, 90);
    move_forward(40, 4.35 * EXPECTED_COUNTS_PER_INCH);
    turn_left(25, 90);

    burgerFlipNewIdea(25);

    travelToIceCream();

}