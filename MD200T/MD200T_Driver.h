#include <Arduino.h>
#include "MD200T_Commo.h"
#include "PID_Header.h"

#ifndef MD200T_DRIVER_H
#define MD200T_DRIVER_H

#define No_return 0
#define PNT_monitor 1
#define PNT_main_data 2
#define RMID 183
#define TMID 184
#define SpeedDataLength 13
#define Vel_packet_size 13

class MD200T_Driver
{
    public:
        MD200T_Driver();
        ~MD200T_Driver();
        Vel_Ctl_Dual_Motor(bool Mot1_enable, bool Mot2_enable, int Vel_mot1, int Vel_mot2, short Return_data);
        Stop_Dual_Motor(short Return_data);
        BLDC_default_para();

        typedef struct 
        {
            bool Mot1_enable;
            bool Mot2_enable;
            int Vel_mot1;
            int Vel_mot2;
            short Return_data;
        } BLDC_set_para;
    private:
        typedef struct 
        {
            bool Mot1_enable;
            bool Mot2_enable;
            int Vel_mot1;
            int Vel_mot2;
            short Return_data;
        } BLDC_para;
};
#endif