#ifndef AT_GENERIC_DRIVER_H
#define AT_GENERIC_DRIVER_H

#define AT_SERIAL_BAUDRATE 9600

#include <Arduino.h>
#include <string.h>
#include <Arduino_FreeRTOS.h>

namespace hermes
{

template <typename T>
class AtGenericDriver
{
protected:
    T &_serial;

public:
    AtGenericDriver(T &_ss);

    String sendATCommand(char *command);

    bool AtDebug(char *command);

    bool areYouOkay(String str);

    bool forcedRefresh();
};

} // namespace hermes
#endif