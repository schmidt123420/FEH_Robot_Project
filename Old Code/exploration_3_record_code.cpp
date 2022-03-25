#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHRPS.h>
#include <FEHSD.h>

int main(void)
{
    //Declare variables
    float touch_x, touch_y;
    int n;
    char points[4] = {'A','B','C','D'};

    //Call this function to initialize the RPS to a course
    RPS.InitializeTouchMenu();

    //Open SD file for writing
    FEHFile *fptr = SD.FOpen("RPS_Test.txt","w");

    //Wait for touchscreen to be pressed and released
    LCD.WriteLine("Press Screen to Start");
    while(!LCD.Touch(&touch_x, &touch_y));
    while(LCD.Touch(&touch_x, &touch_y));

    //Clear screen
    LCD.Clear();

    //Write initial screen info
    LCD.WriteRC("X Position:",2,0);
    LCD.WriteRC("Y Position:",3,0);
    LCD.WriteRC("   Heading:",4,0);

    //Step through each path point to record position and heading
    for (n=0; n<=3; n++)
    {
        //Write point letter
        LCD.WriteRC("Touch to set point ",0,0);
        LCD.WriteRC(points[n],0,20);

        //Wait for touchscreen to be pressed and display RPS data
        while(!LCD.Touch(&touch_x, &touch_y))
        {
            LCD.WriteRC(RPS.X(),2,12); //update the x coordinate
            LCD.WriteRC(RPS.Y(),3,12); //update the y coordinate
            LCD.WriteRC(RPS.Heading(),4,12); //update the heading

            Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
        }
        while(LCD.Touch(&touch_x, &touch_y));

        //Print RPS data for this path point to file
        SD.FPrintf(fptr, "%f %f\n", RPS.X(), RPS.Y());
    }

    //Close SD file
    SD.FClose(fptr);

    return 0;
}
