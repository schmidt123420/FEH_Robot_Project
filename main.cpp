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

/*
    Start -> Drop tray -> Position for Burger Flip
*/
void startAndDropTray() {
    //Start with arm in upright starting position
    setArmStart();
    
    //Wait until light turns on
    int firstLight = waitUntilLight();

    //Move forward 13.5 inches
    moveDistance(40,FORWARD,14); //USED TO BE 13.5
    // Turn 45 right
    Sleep(.5);
    turnRight(25,55);
    // Move forward 25 inches (up the ramp)
    moveDistance(40,FORWARD,25.5); //USED TO BE 25.5
    // turn right to bump into wall
    turnRight(25,90);
    driveUntilWall(40,FORWARD);

    // back up and turn around
    moveDistance(25,BACKWARD,4);
    turnLeft(25,175);

    //Line up with tray drop
    driveForwardUntilLeftBump(25);
    turnLeft(25, 6);
    // Sleep(0.5);
    dropTray();
    Sleep(0.5);
    turnRight(25, 6);
    // Sleep(0.5);
}

/*
    Burger Flip
*/
void burgerFlip() {
    // drive backward and turn right to be perpendicular to burger flip line
    driveUntilWall(25,BACKWARD);
    moveDistance(25,FORWARD,9.5); //USED TO BE 9
    turnRight(25,91);
    driveUntilLine(25,FORWARD);
    moveDistance(25,FORWARD,1.25);

    //turn left, set arm position, turn right for time to be lined up
    turnLeft(25, 25);
    setArmPositionBurgerFlip();
    turnRightForTime(0.7);
    turnLeft(25, 10); //USED TO BE 10
    Sleep(2.0);

    //raise arm enough so can move arm underneath hot plate
    topServo.SetDegree(80);
    middleServo.SetDegree(140);
    bottomServo.SetDegree(100);
    Sleep(2.0);

    moveDistance(25, FORWARD, 0.25); //USED TO BE 0.25

    //flip hot plate
    topServo.SetDegree(70); //USED TO BE 100
    middleServo.SetDegree(60); //USED TO BE 80
    // turnRightForTime(1.0);

    //turn right for time - change function later to allow different motor powers
    float timeNow = TimeNow();

    rightMotor.SetPercent(50);
    leftMotor.SetPercent(-50);

    while (TimeNow() - timeNow < 0.55) { //USED TO BE .5 WAS WORKING BUT MAYBE CUTTING IT CLOSE
        //do nothing
    }

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();


    // turnRight(25, 20);
    // topServo.SetDegree(70);
    // moveDistance(25,FORWARD, 3);
    // turnRight(25, 25);
    // turnRightForTime(0.5);
    Sleep(0.5); //may need to change back to 2
}

void flipIceCreamLever() {
    //Get arm in position to flip ice cream lever
    bottomServo.SetDegree(120);
    Sleep(0.5);
    topServo.SetDegree(90);
    Sleep(0.5);
    middleServo.SetDegree(70);
    Sleep(0.5);

    // followLineForDistance(6); //USED TO BE 7
    moveDistance(40, FORWARD, 3);

    //flip lever down
    middleServo.SetDegree(120);
    Sleep(0.5);

    //move backwards and wait 7 seconds
    moveDistance(25, BACKWARD, 5);
    // turnRight(25, 5);
    Sleep(7.0);

    //lower arm to position to flip lever back up and move forward
    middleServo.SetDegree(180);
    moveDistance(25, FORWARD, 5.5); //USED TO BE 4
    // Sleep(0.5);

    //flip lever up
    topServo.SetDegree(0);
    middleServo.SetDegree(70);
    Sleep(1.0);

}

void burgerFlipToIceCream() {
    moveDistance(25, BACKWARD, 4);
    Sleep(0.5);
    turnLeft(25, 115);
    Sleep(0.5);
}

void jukeboxStuff() {
    //check if can see light
    int light = waitUntilLight();
    if (light == RED_COLOR) {
        LCD.WriteLine("Sees red");

        //drive forward a bit and turn at an angle to hit button
        moveDistance(25, FORWARD, 3.2);
        // Sleep(0.5);
        turnRight(25, 93.5); //USED TO BE 92
        // Sleep(0.5);
        redButtonJukebox();
    }
    else if (light == BLUE_COLOR) {
        LCD.WriteLine("Sees blue");

        //drive backward a bit and turn right to be in line with button
        //moveDistance(25, BACKWARD, 1);
        // Sleep(0.5);
        turnRight(25, 85);
        // Sleep(0.5);

        //drive backward into button
        moveDistanceForTime(25, BACKWARD, 2.5);
    } 
    else {
        LCD.WriteLine("doesn't see red or blue, something wrong");
    }

    //Drive from jukebox to final button
    moveDistance(25, FORWARD, 5);
    // Sleep(0.5);
    turnRight(25, 86);
    // Sleep(0.5);
    driveUntilWall(40, FORWARD);
    moveDistance(25, BACKWARD, 9.5);
    // Sleep(0.5);
    turnLeft(25, 135);
    moveDistanceForTime(40, BACKWARD, 5);
}

void iceCreamToTicketSlide() {
    //back up, turn to face wall, and drive until wall
    moveDistance(40, BACKWARD, 11); //used to be 10, changed to 10.5 because changed 5 to 5.5 to flip ice cream lever up
    // Sleep(0.5);
    turnRight(25, 135);
    // Sleep(0.5);
    setArmStart();
    // Sleep(0.5);
    driveUntilWall(40, FORWARD);
    // Sleep(0.5);

    //back up
    moveDistance(40, BACKWARD, 11.5);
    // Sleep(0.5);

    //turn right and drive down ramp
    turnRight(25, 88.5); //USED TO BE 85
    // Sleep(0.5);
    moveDistance(40, FORWARD, 25);
    // Sleep(0.5);
}

/*
    Jukebox light -> Ticket slide -> End of Run
*/
void ticketSlide() {
    //drive a bit further
    moveDistance(40, FORWARD, 7);
    // Sleep(0.5);

    //turn left 90 degrees and drive to wall
    turnLeft(25, 90);
    // Sleep(0.5);
    driveUntilWall(40, FORWARD);
    // Sleep(0.5);

    //back up and turn left 90 degrees
    moveDistance(25, BACKWARD, 1);
    // Sleep(0.5);
    turnLeft(25, 85);
    // Sleep(0.5);

    //drive into ticket area and back up a bit
    driveUntilWall(40, FORWARD);
    moveDistance(25, BACKWARD, 7);
    // Sleep(0.5);


    //set arm position for ticket slide and slide ticket
    setArmPositionTicketSlide();
    // Sleep(0.5);
    turnRight(25, 17.5);
    // Sleep(0.5);
    moveDistance(25, FORWARD, 4.5);
    // Sleep(0.5);
    slideTicket();
}

void ticketSlideToJukebox() {
    //drive backward from ticket slide and turn left 90 degrees
    moveDistance(40, BACKWARD, 13);
    // Sleep(0.5);
    turnLeft(25, 86);
    // Sleep(0.5);

    //drive forward to line up with wall and drive backwards to be above jukebox light
    driveUntilWall(40, FORWARD);
    moveDistance(25, BACKWARD, 5.5);
    // Sleep(0.5);
}

//Code for going from jukebox light to ticket and sliding ticket
int main(void) {
    // RPS.InitializeTouchMenu();
    startAndDropTray();
    burgerFlip(); 
    burgerFlipToIceCream();
    flipIceCreamLever();
    iceCreamToTicketSlide();
    ticketSlide();
    ticketSlideToJukebox();
    jukeboxStuff();
}
