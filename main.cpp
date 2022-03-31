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
    dropTray();
    Sleep(1.0);
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

    // //Flip hot plate back
    // Sleep(4.0);
    // moveDistance(25,FORWARD, 1);
    // turnLeft(25, 45);
    // moveDistance(40,BACKWARD, 1);
}

/*
    Jukebox light -> Ticket slide -> End of Run
*/
void ticketSlideAndEndOfRun() {
    //Start with arm in upright starting position
    setArmStart();
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
    RPS.InitializeTouchMenu();
}
