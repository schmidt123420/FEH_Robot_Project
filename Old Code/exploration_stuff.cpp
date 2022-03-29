// #include <FEHLCD.h>
// #include <FEHIO.h>
// #include <FEHUtility.h>
// #include <FEHServo.h>
// #include <FEHMotor.h>

// //Exploration 1
// AnalogInputPin cdsCell(FEHIO::P0_0);
// FEHServo servo(FEHServo::Servo0); //declare servo arm
// DigitalInputPin microSwitchFrontRight(FEHIO::P0_1);
// DigitalInputPin microSwitchFrontLeft(FEHIO::P3_7);
// DigitalInputPin microSwitchBackRight(FEHIO::P0_2);
// DigitalInputPin microSwitchBackLeft(FEHIO::P3_6);
// FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
// FEHMotor rightMotor(FEHMotor::Motor3, 9.0);

// //Exploration 2
// AnalogInputPin right_opt(FEHIO::P0_0);
// AnalogInputPin middle_opt(FEHIO::P0_1);
// AnalogInputPin left_opt(FEHIO::P0_2);

// //Exploration 1
// void driveForward() {
//     rightMotor.SetPercent(25);
//     leftMotor.SetPercent(25);

//     while (microSwitchFrontRight.Value() || microSwitchFrontLeft.Value()) {
//         if (microSwitchFrontRight.Value() && microSwitchFrontLeft.Value()) {
//             //do nothing
//         }
//         else if (!microSwitchFrontLeft.Value()) {
//             leftMotor.Stop();
//         }
//         else if (!microSwitchFrontRight.Value()){
//             rightMotor.Stop();
//         }
        
//     }
//     leftMotor.Stop();
//     rightMotor.Stop();

// }

// //Exploration 1
// void turnBackwards90(char direction) {
//     if (direction == 'r') {
//         rightMotor.SetPercent(-25);
//         // while they haven't both been pressed
//         while (microSwitchBackRight.Value() || microSwitchBackLeft.Value()) {
//             //if right button has been clicked but left hasn't turn off right motor
//             //and turn on left motor
//             if (!microSwitchBackRight.Value() && microSwitchBackLeft.Value()) {
//                 rightMotor.Stop();
//                 leftMotor.SetPercent(-25);
//             }
//         }
//     }
//     else {
//         leftMotor.SetPercent(-25);
//         // while they haven't both been pressed
//         while (microSwitchBackRight.Value() || microSwitchBackLeft.Value()) {
//             //if left button has been clicked but right hasn't turn off left motor
//             //and turn on right motor
//             if (!microSwitchBackLeft.Value() && microSwitchBackRight.Value()) {
//                 leftMotor.Stop();
//                 rightMotor.SetPercent(-25);
//             }
//         }
//     }    
// }

// //Exploration 1
// // int main(void)
// // {
// //     servo.SetMin(500);
// //     servo.SetMax(2408);

// //     float xPos, yPos;
// //     while (!LCD.Touch(&xPos, &yPos)) {
// //         //wait for input
// //     }

// //     driveForward();
// //     turnBackwards90('r');
// //     driveForward();
// //     turnBackwards90('l');
// //     driveForward();

// //     // while (1) {
// //     //     LCD.WriteLine(cdsCell.Value());
// //     //     Sleep(1.0);
// //     //     float cdsValue = cdsCell.Value();
// //     //     float degree = 180/3.3 * cdsValue;
// //     //     servo.SetDegree(degree);
// //     // }

// // 	return 0;
// // }

// //Exploration 2
// void followCurvedLine() {
//         int state = MIDDLE; // Set the initial state
//     while (true) { // I will follow this line forever!
//         LCD.WriteLine("State: " + state);
//         switch(state) {
//             // If I am in the middle of the line...
//             case MIDDLE:
//                 // Set motor powers for driving straight
//                 leftMotor.SetPercent(25);
//                 rightMotor.SetPercent(25);
//                 /* Drive */

//                 //if loes contact with left and middle, turn right 
//                 if (left_opt.Value() < 1.8 && middle_opt.Value() < 2) {
//                     state = RIGHT;
//                 }
                
//                 //if lose contact with right and middle, turn left
//                 else if (right_opt.Value() < 2 && middle_opt.Value() < 2) {
//                     state = LEFT;
//                 }
                 
//                  /* If Right sensor is on line */ 
//                 else if (right_opt.Value() > 2.2) {
//                     state = RIGHT; // update a new state
//                 }

//                 /* Code for if left sensor is on the line */
//                 else if (left_opt.Value() > 2) {
//                     state = LEFT;
//                 }

//                 break;

//             // If the right sensor is on the line...
//             case RIGHT:
//                 // Set motor powers for right turn
//                 leftMotor.SetPercent(25);
//                 rightMotor.Stop();
//                 /* Drive */
                
//                 /*IF I no longer need to turn right... */
//                 if(middle_opt.Value() > 2.2) {
//                     /* update a new state */
//                     state = MIDDLE;
//                 }

//                 //if left and not right, turn left
//                 else if (left_opt.Value() > 2 && right_opt.Value() < 2) {
//                     state = LEFT;
//                 }

//                 //Code for if no optosensor sees the line
//                 else if (right_opt.Value() < 2 && left_opt.Value() < 1.8 && middle_opt.Value() < 2) {
//                     state = LEFT;
//                 }

//                 break;
                
//             // If the left sensor is on the line...
//             case LEFT:
//                 /* Mirror operation of RIGHT state */
//                 // Set motor powers for left turn
//                 leftMotor.Stop();
//                 rightMotor.SetPercent(25);
//                 /* Drive */

                
//                 /*IF I no longer need to turn left... */
//                 if(middle_opt.Value() > 2.2) {
//                     /* update a new state */
//                     state = MIDDLE;
//                 }

//                 //if right and not left, turn right
//                 else if (right_opt.Value() > 2.2 && left_opt.Value() < 1.8) {
//                     state = RIGHT;
//                 }

//                 //Code for if no optosensor sees the line
//                 else if (right_opt.Value() < 2 && left_opt.Value() < 1.8 && middle_opt.Value() < 2) {
//                     state = RIGHT;
//                 }

//                 break;
            
//             default: // Error. Something is very wrong.
//                 break;
//         }
//     }
// }
// enum LineStates {
//     MIDDLE,
//     RIGHT,
//     LEFT
// };

// int main() {
//     //followCurvedLine();
// }




//Shaft Encoding Starts Here!!!!!!!!!!!!!!!!!!!!!
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

#define PI 3.141592653589793
#define botWidth 7
#define wheelDiameter 2.5

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right(int motorPower, int angle) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(-1 * motorPower);
    left_motor.SetPercent(motorPower);


    //Calculate counts to turn
    int counts = (318*PI*botWidth*angle)/(360*PI*wheelDiameter);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts + 40) {
        //do nothing
    }

    //<ADD CODE HERE>

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int motorPower, int angle) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(motorPower);
    left_motor.SetPercent(-1 * motorPower);


    //Calculate counts to turn
    int counts = (318*PI*botWidth*angle)/(360*PI*wheelDiameter);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts + 40) {
        //do nothing
    }

    //<ADD CODE HERE>

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}



int main(void)
{
    int motor_percent = 40; //Input power level here
    int expected_counts_per_inch = 318/(2.5 * PI); //Input theoretical counts here

    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    //move forward 14 inches
    move_forward(motor_percent, 14 * expected_counts_per_inch); //see function

    //turn left 90 degrees
    turn_left(20, 90);

    //move forward 10 inches
    move_forward(motor_percent, 10 * expected_counts_per_inch); //see function

    //turn right 90 degrees
    turn_right(20, 90);

    //move forward 4 inches
    move_forward(motor_percent, 4 * expected_counts_per_inch); //see function


    Sleep(2.0); //Wait for counts to stabilize
    //Print out data
    LCD.Write("Theoretical Counts: ");
    LCD.WriteLine(expected_counts_per_inch);
    LCD.Write("Motor Percent: ");
    LCD.WriteLine(motor_percent);
    LCD.Write("Actual LE Counts: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("Actual RE Counts: ");
    LCD.WriteLine(right_encoder.Counts());

    return 0;
}

