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


/* Upload this sketch into Seeeduino and press reset*/

#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 9
#define TxD 8

#define DEBUG_ENABLED  1

#define PIN_TEMP    A5


SoftwareSerial blueToothSerial(RxD,TxD);

int getTemp()
{
    int a = analogRead(PIN_TEMP);
    int B=3975;
    float resistance = (float)(1023-a)*10000/a;
    float temperature = temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;
    
    return (int)temperature;
    
}

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
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
            
            if(recvChar == 't' || recvChar == 'T')
            {
                blueToothSerial.print("temperature: ");
                blueToothSerial.println(getTemp());
            }
        }
        if(Serial.available())
        {//check if there's any data sent from the local serial terminal, you can add the other applications here
            recvChar  = Serial.read();
            blueToothSerial.print(recvChar);
        }
    }


}



void setupBlueToothConnection()
{
    blueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode
    blueToothSerial.print("\r\n+STNA=SeeedBTSlave\r\n");    // set the bluetooth name as "SeeedBTSlave"
    blueToothSerial.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me
    blueToothSerial.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
    delay(2000);                                            // This delay is required.
    blueToothSerial.print("\r\n+INQ=1\r\n");                // make the slave bluetooth inquirable
    Serial.println("The slave bluetooth is inquirable!");
    delay(2000);                                            // This delay is required.
    blueToothSerial.flush();
}