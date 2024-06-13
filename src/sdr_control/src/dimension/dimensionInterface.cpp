#include <sdr_control/dimensionInterface.h>

// Public

using namespace serial_interface;

dimensionInterface::dimensionInterface(std::string portName, int32_t baudrate)
    :SerialInterface(portName, baudrate, 5)
{ }

dimensionInterface::~dimensionInterface() { }

bool dimensionInterface::initialize(bool wait)
{
    std::vector<uint8_t> autobaud(0xAA);

    if(!this->open_port()) { return false; }

    if(wait) { usleep(2000); }
    write(autobaud);
    if(wait) { usleep(1000); }
    write(autobaud);
    if(wait) { usleep(1000); }
    write(autobaud);

    return true;
}

void dimensionInterface::cmdSyrenChannel(int8_t address, int16_t ch, bool invert)
{
    int16_t cmd = invert ? -1*ch : ch;
    int8_t type = cmd >= 0 ? cmdTypes.MOTOR1_DRIVE_FORWARD : cmdTypes.MOTOR1_DRIVE_BACKWARD;

    sendCommand(type, cmd, address);
}

void dimensionInterface::cmdSabertoothChannels(int8_t address, int16_t ch1, int16_t ch2, bool invertCh1, bool invertCh2)
{
    int16_t cmdCh1 = invertCh1 ? -1*ch1 : ch1;
    int16_t cmdCh2 = invertCh2 ? -1*ch2 : ch2;
    int8_t typeCh1 = cmdCh1 >= 0 ? cmdTypes.MOTOR1_DRIVE_FORWARD : cmdTypes.MOTOR1_DRIVE_BACKWARD;
    int8_t typeCh2 = cmdCh2 >= 0 ? cmdTypes.MOTOR1_DRIVE_FORWARD : cmdTypes.MOTOR1_DRIVE_BACKWARD;

    sendCommand(typeCh1, cmdCh1, address);
    sendCommand(typeCh2, cmdCh2, address);
}

void dimensionInterface::driveTurnSabertooth(int8_t address, int16_t drive, int16_t turn, bool invertDrive, bool invertTurn)
{
    int16_t cmdDrive = invertDrive ? -1*drive : drive;
    int16_t cmdTurn = invertTurn ? -1*turn : turn;
    int8_t typeDrive = cmdDrive >= 0 ? cmdTypes.MIXED_MODE_DRIVE_FORWARD : cmdTypes.MIXED_MODE_DRIVE_BACKWARD;
    int8_t typeTurn = cmdTurn >= 0 ? cmdTypes.MIXED_MODE_TURN_LEFT : cmdTypes.MIXED_MODE_TURN_RIGHT;

    sendCommand(typeDrive, cmdDrive, address);
    sendCommand(typeTurn, cmdTurn, address);
}

void dimensionInterface::mecSabertooth(int8_t addressFront, int8_t addressRear, int16_t fr, int16_t fl, int16_t rr, 
        int16_t rl, bool frInvert, bool flInvert, bool rrInvert, bool rlInvert)
{
    cmdSabertoothChannels(addressFront, fr, fl, frInvert, flInvert);
    cmdSabertoothChannels(addressRear, rr, rl, rrInvert, rlInvert);
}

// Private

void dimensionInterface::sendCommand(int8_t command, int16_t value, int8_t address)
{
    std::vector<uint8_t> cmdBytes;
    cmdBytes.resize(4);
    int8_t power = value > 126 ? 126 : (value < -126 ? -126 : value);
    power = abs(power);
    int16_t checksum = address + command + power;
    cmdBytes[0] = address;
    cmdBytes[1] = command;
    cmdBytes[2] = power;
    cmdBytes[3] = checksum & 0b01111111;

    write(cmdBytes);
}