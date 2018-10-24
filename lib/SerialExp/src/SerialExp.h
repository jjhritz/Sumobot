//
// Created by User on 10/11/2018.
//

#include <Arduino.h>

#ifndef SERIALEXP_H
#define SERIALEXP_H

extern boolean newData;

/// Copies data from the serial buffer to the arrays allocated by <allocateStrings>"("int txLength")".
/// Only copies data between specified start and end markers
/// \param txLength The maximum number of bytes accepted
/// \param startMarker The character that signals the start of a message
/// \param endMarker The character that signals the end of a message
/// \param receivedChars The array that the serial message will be copied into
void recvWithStartEndMarkers(uint8_t txLength, char startMarker, char endMarker, char* receivedChars);

/// Reads and discards all data in the serial buffer
void emptyReceiveBuffer();



#endif //SERIALEXP_H


