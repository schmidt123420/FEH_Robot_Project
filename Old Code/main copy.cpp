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
#define BOTTOM_SERVO_MIN 500
#define BOTTOM_SERVO_MAX 1583
#define MIDDLE_SERVO_MIN 872
#define MIDDLE_SERVO_MAX 1950
#define TOP_SERVO_MIN 777
#define TOP_SERVO_MAX 1638

AnalogInputPin cdsCell(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P3_5);
FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor3, 9.0);

FEHServo bottomServo(FEHServo::Servo0);
FEHServo middleServo(FEHServo::Servo5);
FEHServo topServo(FEHServo::Servo7);



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
    topServo.SetDegree(160);
    Sleep(0.5);
    middleServo.SetDegree(10);
    Sleep(0.5);
    bottomServo.SetDegree(180);
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


int main(void) {
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
    // Sleep(3.0);
    // dropTray();
    // Sleep(3.0);
    // slideTicket();


    // middleServo.SetDegree(25);
    // Sleep(2.0);
    // bottomServo.SetDegree(170);
    // Sleep(2.0);
    // topServo.TouchCalibrate();


    // bottomServo.SetDegree(145);
    // Sleep(2.0);
    // middleServo.SetDegree(15);
    // Sleep(2.0);
    // topServo.SetDegree(90);

    //Wait until light turns on
    int firstLight = waitUntilLight();

    //Print to LCD that sees light
    if (firstLight == BLUE_COLOR) {
        LCD.WriteLine("Light color: Blue");
    }
    else {
        LCD.WriteLine("Light color: Red");
    }

    //Drive straight for like 10 inches
    move_forward(40, 12 * expectedCountsPerInch);

    //Turn right 45 degrees
    turn_right(25, 50);
    Sleep(1.0);
    
    //Drive straight up ramp for like 20 inches
    move_forward(40, 37 * expectedCountsPerInch);

    //Turn left 90 degrees
    turn_left(25, 90);

    //Drive straight for like 5 inches
    move_forward(40, 5.5 * expectedCountsPerInch);

    //Turn left 90 degrees and move forward like 1 inch
    turn_left(25, 90);
    move_forward(25, 1.5 * expectedCountsPerInch);

    //Drop tray in sink
    Sleep(3.0);
    dropTray();
    Sleep(2.0);
    //Turn left 90 degrees
    turn_left(25, 90);
    Sleep(1.0);
    
    //Drive straight until close to ticket
    driveForwardUntilWall();

    //Drive backwards
    move_backward(25, 4.5 * expectedCountsPerInch);

    //Turn right 85 degrees
    turn_right(25, 58);

    //Set arm into posiiton to slide ticket
    setArmPositionTicketSlide();

    //Drive straight into position to turn ticket
    move_forward(25, 8.75 * expectedCountsPerInch);

    //Slide ticket
    slideTicket();

    //Drive backwards into hot plate
    

    //Turn right 45 degrees and drive straight back
    turn_right(25, 25);
    move_backward(25, 40 * expectedCountsPerInch);


}


