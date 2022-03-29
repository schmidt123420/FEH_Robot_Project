/**
    Header file for all ports on the robot used
**/


#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>

#ifndef ROBOPORTS_H
#define ROBOPORTS_H



//Igwan motor declarations
extern FEHMotor leftMotor;
extern FEHMotor rightMotor;

//Igwan motor shaft encoders declarations
extern DigitalEncoder rightEncoder;
extern DigitalEncoder leftEncoder;

//Servo motor declarations
extern FEHServo bottomServo;
extern FEHServo middleServo;
extern FEHServo topServo;

//CdS cell declaration
extern AnalogInputPin cdsCell;

//Optosensor declarations
extern AnalogInputPin rightOpt;
extern AnalogInputPin middleOpt;
extern AnalogInputPin leftOpt;

//Bump switch declarations
extern DigitalInputPin microSwitchFrontRight;
extern DigitalInputPin microSwitchFrontLeft;
extern DigitalInputPin microSwitchBackRight;
extern DigitalInputPin microSwitchBackLeft;

#endif