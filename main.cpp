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
    moveDistance(35,FORWARD,13.5);
    // Turn 45 right
    Sleep(.5);
    turnRight(25,45);
    // Move forward 25 inches (up the ramp)
    moveDistance(35,FORWARD,25);
    // turn left to bump into wall
    turnRight(25,90);
    driveUntilWall(40,FORWARD);

    // back up and turn around
    moveDistance(25,BACKWARD,4);
    turnLeft(25,175);

    //Line up with tray drop
    driveForwardUntilLeftBump(25);
    turnLeft(25, 10);
    Sleep(0.5);
    dropTray();
    Sleep(1.0);
    turnRight(25, 10);
    Sleep(0.5);
    // drive backward and turn right to be perpendicular to burger flip line
    driveUntilWall(25,BACKWARD);
    moveDistance(25,FORWARD,9);
    turnRight(25,90);
    setArmPositionBurgerFlip();
    driveUntilLine(25,FORWARD);
    Sleep(2.0);
    //moveDistance(25,FORWARD,1.5);
}

/*
    Burger Flip
*/
void burgerFlip() {
    moveDistance(25,FORWARD,1.5);
    Sleep(5.0);
    //raise arm enough so can move arm underneath hot plate
    topServo.SetDegree(80);
    middleServo.SetDegree(140);
    bottomServo.SetDegree(100);

    //flip hot plate
    topServo.SetDegree(100);
    middleServo.SetDegree(80);
    turnRight(25, 20);
    topServo.SetDegree(70);
    moveDistance(25,FORWARD, 3);
    turnRight(25, 25);
    Sleep(2.0);

    // //Flip hot plate back
    // Sleep(4.0);
    // moveDistance(25,FORWARD, 1);
    // turnLeft(25, 45);
    // moveDistance(40,BACKWARD, 1);
}

void flipIceCreamLever() {
    //Get arm in position to flip ice cream lever
    bottomServo.SetDegree(150);
    Sleep(0.5);
    topServo.SetDegree(90);
    Sleep(0.5);
    middleServo.SetDegree(70);
    Sleep(4.0);

    //flip lever down
    middleServo.SetDegree(120);
    Sleep(2.0);

    //move backwards and wait 7 seconds
    moveDistance(25, BACKWARD, 5);
    Sleep(7.0);

    //lower arm to position to flip lever back up and move forward
    middleServo.SetDegree(180);
    moveDistance(25, FORWARD, 5);
    Sleep(1.0);

    //flip lever up
    topServo.SetDegree(0);
    middleServo.SetDegree(70);
    Sleep(1.0);

}

void burgerFlipToIceCream() {
    //straighten out
    turnLeft(25, 45);

    //drive backwards until optosensors above line to straighten out
    driveUntilLine(25, BACKWARD);
    moveDistance(40, BACKWARD, 1);
    turnLeft(25, 45);
    moveDistance(40, FORWARD, 3); //drive forward until over black line
    followLineForDistance(5);
}

void jukeboxStuff() {
    int jLight = waitUntilLight();

    //set arm to colour 
    if (jLight == RED_COLOR) {
        //blue (to the left) -- THIS COMMENT IS WRONG
        //move to starting position
        topServo.SetDegree(150);
        Sleep(1.0);
        middleServo.SetDegree(180);
        Sleep(1.0);
        bottomServo.SetDegree(120);
        Sleep(1.0);
    }
    else {
        //red (to the right)  -- THIS COMMENT IS WRONG
        //move to starting position
        topServo.SetDegree(150);
        Sleep(0.5);
        middleServo.SetDegree(180);
        Sleep(0.5);
        bottomServo.SetDegree(170);
        Sleep(0.5);
    }

    //move forward slowly
    moveDistance(25,FORWARD,.5);
    //move back slowly
    moveDistance(25,BACKWARD,4);

    //change position of arm for driving across the course
    setArmStart();
}

void iceCreamToJukebox() {
    //back up, turn to face wall, and drive until wall
    moveDistance(40, BACKWARD, 10);
    turnRight(25, 135);
    driveUntilWall(40, FORWARD);

    //back up
    moveDistance(40, BACKWARD, 12);

    //turn right and drive down ramp
    turnRight(25, 90);
    moveDistance(25, FORWARD, 20);

    //turn a bit to the left and drive until wall
    turnLeft(25, 10);
    driveUntilWall(40, FORWARD);

    //drive backwards and turn right
    moveDistance(40, BACKWARD, 19);
    turnRight(25, 90);
    driveUntilLine(25, FORWARD);

    //turn left and drive until over light
    moveDistance(25, FORWARD, 1);
    turnLeft(25, 90);
    moveDistance(25, FORWARD, 4); //to get above light
}

/*
    Jukebox light -> Ticket slide -> End of Run
*/
void ticketSlideAndEndOfRun() {
    //Start with arm in upright starting position
    // setArmStart();
    Sleep(2.0);

    //turn left 90 degrees and drive from jukebox light to wall
    turnLeft(25, 90);
    Sleep(1.0);
    driveUntilWall(40, FORWARD);
    Sleep(1.0);

    //back up and turn left 90 degrees
    moveDistance(25, BACKWARD, 3);
    Sleep(1.0);
    turnLeft(25, 90);
    Sleep(1.0);

    //drive into ticket area and back up a bit
    driveUntilWall(40, FORWARD);
    Sleep(1.0);
    moveDistance(25, BACKWARD, 8);
    Sleep(1.0);

    //set arm position for ticket slide and slide ticket
    setArmPositionTicketSlide();
    //slideTicket();
}

//Code for going from jukebox light to ticket and sliding ticket
int main(void) {
    // setArmStart();
    // Sleep(2.0);
    RPS.InitializeTouchMenu();
    startAndDropTray();
    burgerFlip();
    burgerFlipToIceCream();
    flipIceCreamLever();
    iceCreamToJukebox();
    jukeboxStuff();
    ticketSlideAndEndOfRun();
}
