/*
 * Module: Serial Expansion
 * Function: Provides more functions for dealing with the serial communication interface on the Arduino
 *
 * Based on code by Robin2: https://forum.arduino.cc/index.php?topic=396450.0
 */

#include "SerialExp.h"

boolean newData = false;
void recvWithStartEndMarkers(uint8_t txLength, char startMarker, char endMarker, char* receivedChars)
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char rc;

    // While there's data in the buffer and we aren't looking for a new message
    while (Serial.available() > 0 && newData == false)
    {
        delay(2);
        rc = Serial.read();

        if (recvInProgress == true)
        {
            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= txLength)
                {
                    ndx = txLength - 1;
                }
            }
            else
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker)
        {
            recvInProgress = true;
        }
    }
}

void emptyReceiveBuffer()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}
