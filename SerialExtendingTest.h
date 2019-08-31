#ifndef SERIAL_EXTENDING_TEST_H
#define SERIAL_EXTENDING_TEST_H

#include <Arduino.h>
#include <string.h>

namespace test {
template <class Serial> class MySerial : public Serial {
public:
  MySerial(Serial &hs);
  void sendATCommand(char *command);
};

template <class Serial> MySerial<Serial>::MySerial(Serial &hs) : Serial(hs) {}

template <class Serial> void MySerial<Serial>::sendATCommand(char *command) {
  this->println(String(command));
}

} // namespace test

#endif