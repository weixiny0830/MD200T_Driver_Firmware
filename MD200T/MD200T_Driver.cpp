#include <Arduino.h>
#include "MD200T_Driver.h"
SoftwareSerial MDSerial(2, 3); // RX, TX
MD200T_RS485 Inwheel_motor;
MD200T_Driver::MD200T_Driver(){
    MDSerial.begin(19200);
    MDSerial.write('\n');
    MD200T_Driver::BLDC_default_para();
}
MD200T_Driver::~MD200T_Driver(){}

MD200T_Driver::BLDC_default_para()
{
    MD200T_Driver::BLDC_para BLDC_default_para;
    BLDC_default_para.Mot1_enable = true;
    BLDC_default_para.Mot2_enable = true;
    BLDC_default_para.Vel_mot1 = 0;
    BLDC_default_para.Vel_mot2 = 0;
    BLDC_default_para.Return_data = 0;
}

MD200T_Driver::Vel_Ctl_Dual_Motor(bool Mot1_enable, bool Mot2_enable, int Vel_mot1, int Vel_mot2, short Return_data)
{
    char byArray[Vel_packet_size], CheckSum;
    MD200T_RS485::IByte iData_1, iData_2;
    byArray[0] = RMID;
    byArray[1] = TMID;
    byArray[2] = RETURN_TYPE_MONITOR;
    byArray[3] = PID_PNT_VEL_CMD;
    byArray[4] = PID_ACK;
    byArray[5] = Mot1_enable;
    iData_1 = Inwheel_motor.Int2Byte(Vel_mot1);
    byArray[6] = iData_1.byLow;
    byArray[7] = iData_1.byHigh;
    byArray[8] = Mot2_enable;
    iData_2 = Inwheel_motor.Int2Byte(Vel_mot2);
    byArray[9] = iData_2.byLow;
    byArray[10] = iData_2.byHigh;
    byArray[11] = Return_data;
    CheckSum = Inwheel_motor.GetCheckSum(Vel_packet_size-1, byArray);
    byArray[12] = CheckSum;
    for(int i = 0; i < Vel_packet_size; i++)
        MDSerial.write(byArray[i]);
}

MD200T_Driver::Stop_Dual_Motor(short Return_data)
{
    unsigned char byArray[Vel_packet_size];
    byArray[0] = RMID;
    byArray[1] = TMID;
    byArray[2] = RETURN_TYPE_MONITOR;
    byArray[3] = PID_PNT_VEL_CMD;
    byArray[4] = PID_ACK;
    byArray[5] = 1;
    byArray[6] = 0;
    byArray[7] = 0;
    byArray[8] = 1;
    byArray[9] = 0;
    byArray[10] = 0;
    byArray[11] = Return_data;
    byArray[12] = Inwheel_motor.GetCheckSum(Vel_packet_size-1, byArray);
    for(int i = 0; i < Vel_packet_size; i++)
        MDSerial.write(byArray[i]);
}