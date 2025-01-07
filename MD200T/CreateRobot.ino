#include <Arduino.h>
#include "MD200T_Driver.h"
void setup()  
{
    Serial.begin(19200);
}

String inString = "";
int Mot_col = 0;
MD200T_Driver MD100_Inwheel_Motor;

void loop() // run over and over
{
    while (Serial.available() > 0) 
    {
        int inChar = Serial.read();
        if (isDigit(inChar)) 
        {
            // convert the incoming byte to a char and add it to the string:
            inString += (char)inChar;
        }
        // if you get a newline, print the string, then the string's value:
        if (inChar == '\n') 
        {
            Mot_col = inString.toInt();
            // Serial.println(Mot_col);
            // clear the string for new input:
            inString = "";
        }
    }
    
    if(Mot_col != 0)
    {
        // Serial.print("Motor Control Command: = ");
        // Serial.println(Mot_col);
        switch (Mot_col)
        {
        case 1:
            MD100_Inwheel_Motor.Stop_Dual_Motor(0);
            break;
        case 2:
            MD200T_Driver::BLDC_set_para BLDC_set_para;
            BLDC_set_para.Vel_mot1 = 40;
            BLDC_set_para.Vel_mot2 = 40;
            BLDC_set_para.Return_data = 0;
            MD100_Inwheel_Motor.Vel_Ctl_Dual_Motor(1,1, BLDC_set_para.Vel_mot1, BLDC_set_para.Vel_mot2, BLDC_set_para.Return_data);
            break;
        default:
            break;
        }
        Mot_col = 0;
    }

}


