#ifndef DIMENSION_INTERFACE_H
#define DIMENSION_INTERFACE_H

#include <stdlib.h>
#include <sdr_control/serial_interface.hpp>

using namespace serial_interface;

class dimensionInterface : public SerialInterface
{
public:
    dimensionInterface(std::string portName, int32_t baudRate);
    ~dimensionInterface();
    // Sends autobaud and initializes a motor controller
    // Returns true if the serial port is opened. False otherwise.
    bool initialize(bool wait=true);
    // Sends command to a SyRen motor controller
    // Expects a range from -126 to 126
    void cmdSyrenChannel(int8_t address, int16_t ch, bool invert=false);
    // Sends commands to a Sabertooth motor controller
    // Expects a range from -126 to 126
    void cmdSabertoothChannels(int8_t address, int16_t ch1, int16_t ch2, bool invertCh1=false, 
        bool invertCh2=false);
    // Sends drive and turn values to a Sabertooth motor controller
    // Expects a range from -126 to 126
    void driveTurnSabertooth(int8_t address, int16_t drive, int16_t turn, bool invertDrive=false, 
        bool invertTurn=false);
    // Function to control a mecanum platform with two Sabertooth controllers
    void mecSabertooth(int8_t addressFront, int8_t addressRear, int16_t fr, int16_t fl, int16_t rr, 
        int16_t rl, bool frInvert=false, bool flInvert=false, bool rrInvert=false, bool rlInvert=false);

private:
    // Structure that defines command types
    struct COMMAND_TYPES
    {
        const int8_t MOTOR1_DRIVE_FORWARD = 0;
        const int8_t MOTOR1_DRIVE_BACKWARD = 1;
        const int8_t MOTOR1_DRIVE_7BIT = 6;
        const int8_t MOTOR2_DRIVE_FORWARD = 4;
        const int8_t MOTOR2_DRIVE_BACKWARD = 5;
        const int8_t MOTOR2_DRIVE_7BIT = 7;
        const int8_t MIXED_MODE_DRIVE_FORWARD = 8;
        const int8_t MIXED_MODE_DRIVE_BACKWARD = 9;
        const int8_t MIXED_MODE_TURN_RIGHT = 10;
        const int8_t MIXED_MODE_TURN_LEFT = 11;
        const int8_t MIXED_MODE_DRIVE_7BIT = 12;
        const int8_t MIXED_MODE_TURN_7BIT = 13;
        const int8_t SET_MIN_VOLTAGE = 2;
        const int8_t SET_MAX_VOLTAGE = 3;
        const int8_t SET_SERIAL_TIMEOUT = 14;
        const int8_t SET_BAUD_RATE = 15;
        const int8_t SET_RAMPING = 16;
        const int8_t SET_DEADBAND = 17;
    }cmdTypes;
    // Sends a byte array to a device
    void sendCommand(int8_t command, int16_t value, int8_t address);
};

#endif
