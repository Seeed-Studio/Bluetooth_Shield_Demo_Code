/*
BluetoothShield Demo Code Master.pde.This sketch could be used with
Slave.pde to establish connection between two Arduino.
2011 Copyright (c) Seeed Technology Inc.  All right reserved.

Author: Steve Chang

Modify: Loovee
2013-10-29

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

*/


#include <SoftwareSerial.h>                         // Software Serial Port

#define RxD         7
#define TxD         6

#define PINBUTTON   9                               // pin of button

#define DEBUG_ENABLED  1



SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(PINBUTTON, INPUT);
    
    setupBlueToothConnection();
    //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
}

void loop()
{
    
    static unsigned char state = 0;             // led off
    
    if(digitalRead(PINBUTTON))
    {
        state = 1-state;
        
        Serial.println("button on");
        
        blueToothSerial.print(state);
        
        delay(10);
        while(digitalRead(PINBUTTON))       // until button release
        {
            delay(10);
        }
        
        Serial.println("button off");
    }
}


void setupBlueToothConnection()
{

	
    blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400); 
	
	blueToothSerial.print("AT+NAMESeeedMaster");    // set the bluetooth name as "SeeedMaster" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	
	blueToothSerial.print("AT+ROLEM");             // set the bluetooth work in slave mode
	delay(400); 
	
    blueToothSerial.print("AT+PIN0000");             // set the pair code to connect 
	delay(400);
	
	blueToothSerial.print("AT+AUTH1");             // Auto-connection should be forbidden here
    delay(400);    
	
	blueToothSerial.print("AT+SEARCH");             // Auto-search
    delay(400);  

    blueToothSerial.flush();
	
	
}