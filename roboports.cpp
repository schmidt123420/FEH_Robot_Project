/**
    Definitions for all ports on the robot used
**/

#include "roboports.h"

#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>

//Igwan motor definitions
FEHMotor leftMotor(FEHMotor::Motor2, 9.0);
FEHMotor rightMotor(FEHMotor::Motor3, 9.0);

//Igwan motor shaft encoders definitions
DigitalEncoder rightEncoder(FEHIO::P0_1); 
DigitalEncoder leftEncoder(FEHIO::P3_5); 

//Servo motor definitions
FEHServo bottomServo(FEHServo::Servo0);
FEHServo middleServo(FEHServo::Servo5);
FEHServo topServo(FEHServo::Servo7);

//CdS cell definitions
AnalogInputPin cdsCell(FEHIO::P0_0);

//Optosensor definitions
AnalogInputPin rightOpt(FEHIO::P1_2);
AnalogInputPin middleOpt(FEHIO::P1_1);
AnalogInputPin leftOpt(FEHIO::P1_0);

//Bump switch definitions
DigitalInputPin microSwitchFrontRight(FEHIO::P2_0);
DigitalInputPin microSwitchFrontLeft(FEHIO::P2_2);
DigitalInputPin microSwitchBackRight(FEHIO::P2_1);
DigitalInputPin microSwitchBackLeft(FEHIO::P2_3);
