/**
    Header file for all functions that control the robot
**/
#ifndef ROBOMOVEMENT_H
#define ROBOMOVEMENT_H

/*! Drive given distance and direction using encoders.
*
* To drive forward 14 inches, ex:
* moveDistance(motorPercent, FORWARD);
*
* @param motorPower = motor power percent
* @param direction = direction to travel in
*/
void moveDistance(int motorPower, bool direction);


/*! Turn to the right given angle using encoders.
*
* To turn right 90 degrees, ex:
* turnRight(motorPercent, 90);
*
* @param motorPower = motor power percent
* @param angle = angle to turn
*/
void turnRight(int motorPower, int angle);


/*! Turn to the left given angle using encoders.
*
* To turn left 90 degrees, ex:
* turnRight(motorPercent, 90);
*
* @param motorPower = motor power percent
* @param angle = angle to turn
*/
void turnLeft(int motorPower, int angle);


/*! Waits until sees light turned on and reports color of light
*
* @return color of light
*/
int waitUntilLight();





//Compound movements

/*! Drive given direction until lined up with wall
*
* @param percent = motor power percent
* @param direction = direction to drive in
*/
void driveUntilWall(int motorPower, bool direction);

/*! Follow line on the course (either straight or curved)
*/
void followLine();

/*! Set arm in starting position to drop tray
*/
void setArmStart();

/*! Drop tray
*/
void dropTray();

/*! Set arm in position to slide ticket
*/
void setArmPositionTicketSlide();

/*! Slide the ticket
*/
void slideTicket();



#endif


