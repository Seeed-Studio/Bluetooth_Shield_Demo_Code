/*
BluetoothShield Demo Code Slave.pde. This sketch could be used with
Master.pde to establish connection between two Arduino. It can also
be used for one slave bluetooth connected by the device(PC/Smart Phone)
with bluetooth function.
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


#include <SoftwareSerial.h>   //Software Serial Port

#define RxD         7
#define TxD         6

#define PINLED      9

#define LEDON()     digitalWrite(PINLED, HIGH)
#define LEDOFF()    digitalWrite(PINLED, LOW)

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(PINLED, OUTPUT);
    LEDOFF();
    
    setupBlueToothConnection();
}

void loop()
{
    char recvChar;
    
    while(1)
    {
        if(blueToothSerial.available())
        {//check if there's any data sent from the remote bluetooth shield
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
            
            if(recvChar == '1')
            {
                LEDON();
            }
            else if(recvChar == '0')
            {
                LEDOFF();
            }
        }
    }
}

void setupBlueToothConnection()
{
	blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400); 
	
	blueToothSerial.print("AT+NAMESeeedBTSlave");    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	
	blueToothSerial.print("AT+ROLES");             // set the bluetooth work in slave mode
	delay(400); 
	
    blueToothSerial.print("AT+PIN0000");             // set the pair code to connect 
	delay(400);
	
	blueToothSerial.print("AT+AUTH1");             // Auto-connection should be forbidden here
    delay(400);    

    blueToothSerial.flush();
}
