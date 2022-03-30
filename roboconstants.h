/**
    Header file for all constants used
**/
#ifndef CONSTANTS_H
#define CONSTANTS_H


/*
    Define constants
*/
#define PI 3.141592653589793
#define BOT_WIDTH 9 //width of crayola bot
<<<<<<< HEAD
#define WHEEL_DIAMETER 3 //diameter of robot wheel
=======
#define WHEEL_DIAMETER 3.0 //diameter of robot wheel
>>>>>>> 41f53b10f7a6389138bdbee4b805720b8b62fd99
#define BLUE_COLOR 0
#define RED_COLOR 1
#define FORWARD true
#define BACKWARD false
#define LEFT_TUNING_CONST 0.9
#define RIGHT_TUNING_CONST 0.78
#define BOTTOM_SERVO_MIN 1060
#define BOTTOM_SERVO_MAX 2070
#define MIDDLE_SERVO_MIN 872
#define MIDDLE_SERVO_MAX 1950
#define TOP_SERVO_MIN 777
#define TOP_SERVO_MAX 1638
#define RIGHT_OPT_THRESHOLD 2.3
#define MIDDLE_OPT_THRESHOLD 2.3
#define LEFT_OPT_THRESHOLD 2.4
<<<<<<< HEAD
#define EXPECTED_COUNTS_PER_INCH 318.0/(WHEEL_DIAMETER * PI)
=======
#define EXPECTED_COUNTS_PER_INCH (318.0/(WHEEL_DIAMETER * PI))
>>>>>>> 41f53b10f7a6389138bdbee4b805720b8b62fd99

//Line states for line following algorithim
enum LineStates {
    MIDDLE,
    RIGHT,
    LEFT
};

#endif