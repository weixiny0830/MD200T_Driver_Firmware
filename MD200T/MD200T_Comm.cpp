#include <Arduino.h>
#include "MD200T_Commo.h"

MD200T_Modbus::MD200T_Modbus(){}
MD200T_Modbus::~MD200T_Modbus(){}
MD200T_RS485::MD200T_RS485(){}
MD200T_RS485::~MD200T_RS485(){}

////////////////////////////////////////////////////////
//////////////Communication through Modbus//////////////
////////////////////////////////////////////////////////

////////////////// ModBus CRC16///////////////
int MD200T_Modbus::CRC16(int crc, char *buf, int len)
{
    int i, tmp;
    for(i=0; i<len; i++) 
    {
        tmp = crc ^ (0x00ff & *(char *)buf++);
        crc = (crc>>8) ^ Crc16Table[tmp & 0xff];
    }
    return crc;
}

// MODBUS protocol.
int MD200T_Modbus::IsCRC16ChkOK(char byIn[], int nPacketSize)
{
    int wCRC, wInCRC;
    wInCRC = CRC16(0xffff, byIn, nPacketSize-2);
    wCRC = byIn[nPacketSize-2] | (int)byIn[nPacketSize-1]<<8;
    if(wCRC==wInCRC) 
        return 1;
    else 
        return 0;
}

// Make interger from two bytes
short MD200T_Modbus::Byte2Int(char byLow, char byHigh)
{
    return (byLow | (short)byHigh<<8);
}

// Make long type data from four bytes
int MD200T_Modbus::Byte2LInt(char byData1, char byData2, char byData3, char byData4)
{
    return((int)byData1 | (int)byData2<<8 | (int)byData3<<16 | (int)byData4<<24);
}

int MD200T_Modbus::MDPutData(char byFuncCode, int nData, char byPort)
{
    char byArray[MAX_PACKET_SIZE];
    MD200T_Modbus::IByte iData;
    int wCRC;
    byArray[0] = DEF_ID;
    byArray[1] = byFuncCode;
    byArray[2] = 2;
    iData = MD200T_Modbus::Int2Byte(nData);
    byArray[3] = iData.byHigh;
    byArray[4] = iData.byLow;
    wCRC = MD200T_Modbus::CRC16(0xffff, byArray, 5);
    iData = MD200T_Modbus::Int2Byte(wCRC);
    byArray[5] = iData.byLow;
    byArray[6] = iData.byHigh;
    // PutArray2Buf(byArray, 7, byPort);
    return 1;
}

int MD200T_Modbus::MDPutLongData(char byFuncCode, long Data, char byPort)
{
    char byArray[MAX_PACKET_SIZE];
    MD200T_Modbus::IByte iData;
    MD200T_Modbus::LByte lData;
    int wCRC;
    byArray[0] = DEF_ID;
    byArray[1] = byFuncCode;
    byArray[2] = 4;
    lData = MD200T_Modbus::Long2Byte(Data);
    byArray[3] = lData.byData2;
    byArray[4] = lData.byData1;
    byArray[5] = lData.byData4;
    byArray[6] = lData.byData3;
    wCRC = MD200T_Modbus::CRC16(0xffff, byArray, 7);
    iData = MD200T_Modbus::Int2Byte(wCRC);
    byArray[7] = iData.byLow;
    byArray[8] = iData.byHigh;
    // PutArray2Buf(byArray, 9, byPort);
    return 1;
}

int MD200T_Modbus::Response2Write(char byIn[], int nSize, char byPort)
{
    char byArray[MAX_PACKET_SIZE], i;
    MD200T_Modbus::IByte iData;
    int wCRC;
    for(i=0; i<nSize; i++) 
        byArray[i] = byIn[i];
    wCRC = CRC16(0xffff, byArray, nSize);
    iData = MD200T_Modbus::Int2Byte(wCRC);
    byArray[nSize] = iData.byLow;
    byArray[nSize+1] = iData.byHigh;
    // PutArray2Buf(byArray, nSize+2, byPort);
    return 1;
}

////////////////////////////////////////////////////////
//////////////Communication through RS485///////////////
////////////////////////////////////////////////////////

// Test the packet using check-sum in InBuf
short MD200T_RS485::IsChkSumOK(char *byArray, short nPacketSize)
{
    short i;
    char cbySum = 0;
    for(i=0; i<nPacketSize; i++) 
    {
        cbySum += *(byArray + i);
    }
    if(cbySum==0) 
        return 1;
    else 
        return 0;
}

////////////////// RS485 CheckSUM///////////////
char MD200T_RS485::GetCheckSum(short nPacketSize, char *byArray)
{
    char byTmp=0;
    short i;
    for(i=0; i<nPacketSize; i++) 
        byTmp += *(byArray+i);
    return (~byTmp + 1);
}

// Make interger from two bytes
short MD200T_RS485::Byte2Int(char byLow, char byHigh)
{
    return (byLow | (short)byHigh << 8);
}
// Make long type Data from four bytes
int MD200T_RS485::Byte2LInt(char byData1, char byData2, char byData3, char byData4)
{
    return((int)byData1 | (int)byData2 << 8 | (int)byData3 << 16 | (int)byData4 << 24);
}

/////////////////Get Main Data///////////////////
short MD200T_RS485::GetMainData(char byData[])
{
    MD200T_RS485::Motor_data Motor_data;
    Motor_data.nRPM = Byte2Int(byData[0], byData[1]);
    Motor_data.wTq = Byte2Int(byData[2], byData[3]);
    Motor_data.byType = byData[4]; //Control type
    Motor_data.nRefVel = Byte2Int(byData[5], byData[6]);
    Motor_data.nOut = Byte2Int(byData[7], byData[8]);
    Motor_data.byStatus = byData[9];
    Motor_data.nPosi = Byte2LInt(byData[10], byData[11], byData[12], byData[13]);
    Motor_data.byBrakeDuty = byData[14];
    Motor_data.byTemp = byData[15];
    return 1;
}