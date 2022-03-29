//RPS Exploration

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>
#include <cmath>

// RPS Delay time
#define RPS_WAIT_TIME_IN_SEC 0.33

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

// Defines for pulsing the robot
#define PULSE_TIME 0.15
#define PULSE_POWER 15

// Define for the motor power
#define POWER 40

// Orientation of QR Code
#define PLUS 0
#define MINUS 1

//Constants for light color seen by CdS cell
#define BLUE_COLOR 0
#define RED_COLOR 1

//Counts per inch for shaft encoding
#define PI 3.141592653589793
#define WHEEL_DIAMETER 3 //diameter of robot wheel
#define EXPECTED_COUNTS_PER_INCH 318/(WHEEL_DIAMETER * PI)
#define BOT_WIDTH 9 //width of crayola bot

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P3_5);
FEHMotor right_motor(FEHMotor::Motor3, 9.0);
FEHMotor left_motor(FEHMotor::Motor2, 9.0);
AnalogInputPin cdsCell(FEHIO::P0_0);

/*! Waits until sees light turned on and reports color of light
*
* @return color of light
*/
int waitUntilLight() {
    float cdsValue = cdsCell.Value();
    while (cdsValue > 2.7) {
        //wait until light turns on
        cdsValue = cdsCell.Value();
    }
    
    //If light is blue, return blue. Otherwise return red
    if (cdsValue > 1.219) {
        return BLUE_COLOR;
    }
    else {
        return RED_COLOR;
    }
}

/*! Drive forward given distance using encoders.
*
* To drive forward 14 inches, ex:
* move_forward(motor_percent, 14 * counts);
*
* @param percent = motor power percent
* @param counts = encoder counts for distance to travel
*/
void drive_forward_amount(int percent, int counts)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent. multiply by -1 because set up motors backwards
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(-1 * percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
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
    right_motor.SetPercent(motorPower);
    left_motor.SetPercent(-1 * motorPower);

    //Calculate counts to turn
    int counts = (318*PI*BOT_WIDTH*angle)/(360*PI*WHEEL_DIAMETER);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts) {
        //do nothing
    }

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(-1 * percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Move forward using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void move_forward(int percent, int counts) //using encoders
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // //print  values
    // LCD.WriteLine(left_encoder.Counts());
    // LCD.WriteLine(right_encoder.Counts());

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts) 
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/* 
 * Use RPS to move to the desired x_coordinate based on the orientation of the QR code
 */
void check_x(float x_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code 
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.X() > 0 && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
    {
        if (orientation == MINUS)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (orientation == PLUS)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

/* 
 * Use RPS to move to the desired y_coordinate based on the orientation of the QR code
 */
void check_y(float y_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Y() > 0 && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
    {
        if (orientation == MINUS)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (orientation == PLUS)
        {
            // Pulse the motors for a short duration in the correct direction
           pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

/* 
 * Use RPS to move to the desired heading
 */
void check_heading(float heading)
{
    //You will need to fill out this one yourself and take into account
    //checking for proper RPS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    /*
        SUGGESTED ALGORITHM:
        1. Check the current orientation of the QR code and the desired orientation of the QR code
        2. Check if the robot is within the desired threshold for the heading based on the orientation
        3. Pulse in the correct direction based on the orientation
    */
    float currentHeading = RPS.Heading();

    while (RPS.Y() > 0 && (currentHeading > heading + 3 || currentHeading < heading - 3)) {
        //If -360 < curr - desired <= -180 , clockwise
        if (currentHeading - heading <= -180) {
            pulse_counterclockwise(-PULSE_POWER, PULSE_TIME);
        }
        //If -180 < curr - desired <= 0 , counterclockwise
        else if (currentHeading - heading <= 0) {
            pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
        }
        //If 0 < curr - desired <= 180 , clockwise
        else if (currentHeading - heading <= 180) {
            pulse_counterclockwise(-PULSE_POWER, PULSE_TIME);
        }
        //If 180 < curr - desired <= 360 , counterclockwise
        else {
            pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
        }
        Sleep(0.5);   
        currentHeading = RPS.Heading();
    }
}

int main(void)
{
    // float touch_x,touch_y;
    // float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    // float A_heading, B_heading, C_heading, D_heading;
    // int B_C_counts, C_D_counts, turn_90_counts;
    float touch_x, touch_y;
    float vanillaX = 16.599, vanillaY = 49.599, vanillaHeading = 134.300;
    //float twistX = 19.800, twistY = 52.500, twistHeading = 134.300;
    float twistX = 27.5, twistY = 50.699, twistHeading = 90;
    float chocolateX = 22.699, chocolateY = 55.599, chocolateHeading = 134.300;

    RPS.InitializeTouchMenu();
    LCD.WriteLine("Initialized RPS, will now drive forward");

    // RPS.InitializeTouchMenu();

    // LCD.WriteLine("RPS & Data Logging Test");
    // LCD.WriteLine("Press Screen To Start");
    // while(!LCD.Touch(&touch_x,&touch_y));
    // while(LCD.Touch(&touch_x,&touch_y));

    //Wait until light turns on and drive forward
    //int firstLight = waitUntilLight();
    drive_forward_amount(40, 13 * EXPECTED_COUNTS_PER_INCH);
    LCD.WriteLine("Drove forward, will now turn right");
    Sleep(2.0);
    turn_right(25, 40);
    LCD.WriteLine("Turned right, will now drive up ramp");
    Sleep(2.0);
    drive_forward_amount(40, 30 * EXPECTED_COUNTS_PER_INCH);
    LCD.WriteLine("Drove up ramp, will now drive to lever");
    Sleep(2.0);

    // //Drive to vanilla lever
    // check_y(vanillaY, PLUS);
    // check_x(vanillaX, PLUS);
    // check_heading(vanillaHeading);

    //Drive to twist lever
    check_x(twistX, PLUS);
    check_y(twistY, PLUS);
    check_heading(twistHeading);

    LCD.Write("X Coordinate: ");
    LCD.WriteLine(RPS.X()); //update the x coordinate
    LCD.Write("Y Coordinate: ");
    LCD.WriteLine(RPS.Y()); //update the y coordinate
    LCD.Write("Heading: ");
    LCD.WriteLine(RPS.Heading()); //update the heading


    // //Wait for touchscreen to be pressed and display RPS data
    // while(true)
    // {
    //     LCD.WriteRC(RPS.X(),2,12); //update the x coordinate
    //     LCD.WriteRC(RPS.Y(),3,12); //update the y coordinate
    //     LCD.WriteRC(RPS.Heading(),4,12); //update the heading

    //     Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
    // }

    // // COMPLETE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    // FEHFile* fptr = SD.FOpen("RPS_TEST.txt", "r");
    // SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    // SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    // SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    // SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    // SD.FClose(fptr);

    // // WRITE CODE HERE TO SET THE HEADING DEGREES AND COUNTS VALUES
    // A_heading = 90.0;
    // B_heading = 180.0;
    // C_heading = 270.0;
    // D_heading = 0.0;

    // B_C_counts = 16 * COUNTS_PER_INCH;
    // C_D_counts = 10 * COUNTS_PER_INCH;

    // turn_90_counts = 90 * COUNTS_PER_DEGREE;

    // // A --> B
    // check_y(B_y, PLUS);
    // check_heading(B_heading);

    // // B --> C
    // move_forward(POWER, B_C_counts);
    // check_x(C_x, MINUS);
    // turn_counterclockwise(POWER, turn_90_counts);
    // check_heading(C_heading);

    // // C --> D
    // move_forward(POWER, C_D_counts);
    // check_y(D_y, MINUS);
    // turn_counterclockwise(POWER, turn_90_counts);
    // check_heading(D_heading);

    return 0;
}