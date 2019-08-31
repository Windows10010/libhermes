#ifndef AT_GENERIC_DRIVER_H
#define AT_GENERIC_DRIVER_H

#define DEBUG(x)                                                               \
  { std::cout << x << std::endl; }

#define AT_SERIAL_BAUDRATE 9600

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <Arduino_FreeRTOS.h>
#include <Timing.h>
#include <string.h>

namespace hermes {

template <typename T> class AtGenericDriver {
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