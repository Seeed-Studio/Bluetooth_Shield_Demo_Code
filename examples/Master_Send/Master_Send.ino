/*
BluetoothShield Demo Code Master.pde.This sketch could be used with
Slave.pde to establish connection between two Arduino.
2011 Copyright (c) Seeed Technology Inc.  All right reserved.

Author: Steve Chang

This demo code is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

For more details about the product please check http://www.seeedstudio.com/depot/

*/


/* Upload this sketch into Seeeduino and press reset*/

#include <SoftwareSerial.h>                     // Software Serial Port

#define RxD 6
#define TxD 7


#define DEBUG_ENABLED  1

String retSymb = "+RTINQ=";                     // start symble when there's any return
String slaveName = ";SeeedBTSlave";             // caution that ';'must be included, and make sure the slave name is right.
int nameIndex = 0;
int addrIndex = 0;

String recvBuf;
String slaveAddr;

String connectCmd = "\r\n+CONN=";

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection();
    //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
}

void loop()
{
    char recvChar;
    while(1)
    {
        if(blueToothSerial.available())         //check if there's any data sent from the remote bluetooth shield
        {
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
        }
        if(Serial.available())                  //check if there's any data sent from the local serial terminal, you can add the other applications here
        {
            recvChar  = Serial.read();
            blueToothSerial.print(recvChar);
        }
    }
}

void setupBlueToothConnection()
{
    blueToothSerial.begin(38400);                               // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=1\r\n");                 // set the bluetooth work in master mode
    blueToothSerial.print("\r\n+STNA=SeeedBTMaster\r\n");       // set the bluetooth name as "SeeedBTMaster"
    blueToothSerial.print("\r\n+STAUTO=0\r\n");                 // Auto-connection is forbidden here
    delay(2000);                                                // This delay is required.
    blueToothSerial.flush();
    blueToothSerial.print("\r\n+INQ=1\r\n");                    //make the master inquire
    Serial.println("Master is inquiring!");
    delay(2000); // This delay is required.

    //find the target slave
    char recvChar;
    while(1)
    {
        if(blueToothSerial.available())
        {
            recvChar = blueToothSerial.read();
            recvBuf += recvChar;
            nameIndex = recvBuf.indexOf(slaveName);             //get the position of slave name
            
                                                                //nameIndex -= 1;
                                                                //decrease the ';' in front of the slave name, to get the position of the end of the slave address
            if ( nameIndex != -1 )
            {
                //Serial.print(recvBuf);
                addrIndex = (recvBuf.indexOf(retSymb,(nameIndex - retSymb.length()- 18) ) + retSymb.length());//get the start position of slave address
                slaveAddr = recvBuf.substring(addrIndex, nameIndex);//get the string of slave address
                break;
            }
        }
    }
    
    //form the full connection command
    connectCmd += slaveAddr;
    connectCmd += "\r\n";
    int connectOK = 0;
    Serial.print("Connecting to slave:");
    Serial.print(slaveAddr);
    Serial.println(slaveName);
    //connecting the slave till they are connected
    do
    {
        blueToothSerial.print(connectCmd);//send connection command
        recvBuf = "";
        while(1)
        {
            if(blueToothSerial.available()){
                recvChar = blueToothSerial.read();
                recvBuf += recvChar;
                if(recvBuf.indexOf("CONNECT:OK") != -1)
                {
                    connectOK = 1;
                    Serial.println("Connected!");
                    blueToothSerial.print("Connected!");
                    break;
                }
                else if(recvBuf.indexOf("CONNECT:FAIL") != -1)
                {
                    Serial.println("Connect again!");
                    break;
                }
            }
        }
    }while(0 == connectOK);
}