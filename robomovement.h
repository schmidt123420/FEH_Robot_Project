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
* @param distance = distance to travel in inches
*/
void moveDistance(int motorPower, bool direction, float distance);


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

/*! 
*  Drives until robot is above a black line
*/
void driveUntilLine(int motorPower, bool direction);

/*! Follow line on the course (either straight or curved)
*   for given distance
*
*   @param distance = distance to travel
*/
void followLineForDistance(float distance);

/*! Follow line on the course (either straight or curved)
*   until robot meets a wall and line up with the wall
*/
void followLineUntilWall();

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


