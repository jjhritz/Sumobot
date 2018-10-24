/*
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <ZumoShield.h>
#include <SerialExp.h>

#define AUTOSTOP 1
#define DEBUG 0

int fspeed = 400;                       // Full motor speed
int hspeed = 250;                       // Reduced motor speed.  Should be high enough to allow movement.
const uint8_t messageSize = 8;          // Maxiumum number of bytes expected for command message.
char message[messageSize];              // Message received from the serial port
int commandTime = 250;                  // How long, in ms, a command is allowed to run before the motors stop.
long finishCommandTimestamp = 0;        // timestamp of the last finished command.
long currentTimestamp = 0;              // current execution time.
ZumoMotors motors;                      // Object that controls the motors on the Zumo Shield.

/// Sets the timestamp for when the motors will be stopped.
void reset_ending_timestamp()
{
    finishCommandTimestamp = millis() + commandTime;
}

/// Sets motor speeds based on the received command
void drive_motors(char cmd)
{
    switch (cmd)
    {
        // Move forward
        case 'f':
            motors.setSpeeds(hspeed, hspeed);
            if(DEBUG){Serial.println("drive_motors moving forward.");}
            break;
        // Turbo forward
        case 't':
            motors.setSpeeds(fspeed, fspeed);
            if(DEBUG){Serial.println("drive_motors turbo forward.");}
            break;
        // Move backwards
        case 'b':
            motors.setSpeeds(-hspeed,-hspeed);
            if(DEBUG){Serial.println("drive_motors moving backward.");}
            break;
        // Turn left
        case 'l':
            motors.setSpeeds(0,hspeed);
            if(DEBUG){Serial.println("drive_motors turning left.");}
            break;
        // Turn right
        case 'r':
            motors.setSpeeds(hspeed, 0);
            if(DEBUG){Serial.println("drive_motors turning right.");}
            break;
        // Clockwise spin
        case 'c':
            motors.setSpeeds(hspeed, -hspeed);
            if(DEBUG){Serial.println("drive_motors spinning clockwise.");}
            break;
        // Anti-clockwise spin
        case 'a':
            motors.setSpeeds(-hspeed, hspeed);
            if(DEBUG){Serial.println("drive_motors turning left.");}
            break;
        case 's':
            motors.setSpeeds(0,0);
            if(DEBUG){Serial.println("drive_motors stopping.");}
        // Ignore unrecognized command
        default:
            if(DEBUG){Serial.println("drive_motors bogus command.");}
            break;
    }
} // end drive_motors

/// Parses most recent serial message and passes the command to drive_motors"(char)"
void parse_serial_command()
{
    char cmd = message[0];
    if(DEBUG){Serial.print("parse_serial_cmd="); Serial.println(cmd);}
    drive_motors(cmd);
}// end parse_serial_command


/// Initializes the hardware and software components needed to operate the robot.
/// Only runs once, at board startup
void setup()
{
    // Initialize Uno serial
    Serial.begin(9600);
    // flip motors, if needed
    // motors.flipLeftMotor(true);
    motors.flipRightMotor(true);
    // set timestamps
    finishCommandTimestamp = millis();
    currentTimestamp = millis();
} // end setup

/// Primary execution loop.  Runs endlessly while board is powered.
void loop()
{
    // set current timestamp
    currentTimestamp = millis();
    // if command timeout, stop the motors
    if ((currentTimestamp > finishCommandTimestamp)
        && AUTOSTOP)
    {
        motors.setSpeeds(0, 0);
    }

    // receive message from Nub060
    recvWithStartEndMarkers(messageSize, '[', ']', message);

    // if we've received a complete message,
    if (newData == true)
    {
        // parse the message to extract the commands
        parse_serial_command();
        // set command end timestamp
        reset_ending_timestamp();
        // tell recv we're ready to accept a new message
        newData = false;
    }// end if
} // end loop

