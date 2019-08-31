#include "AtGenericDriver.h"

using namespace hermes;

template <typename T> AtGenericDriver<T>::AtGenericDriver(T &ss) : _serial(ss) {
  _serial.begin(AT_SERIAL_BAUDRATE);
}

template <typename T> String AtGenericDriver<T>::sendATCommand(char *command) {
  _serial.println(command);
  // TickType_t staort = xTaskGetTickCount();
  unsigned long start;
  char c;
  String str("");
  bool flag = true;
  while (true) {
    if (!this->_serial.available()) {
      if (flag) {
        // start = millis();
        start = getSysTime();
        flag = !flag;
      } else {
        Serial.println((unsigned long)xTaskGetTickCount());
        if (getSysTime() - start > 5000l) {
          if (str.equals(String(""))) {
            DEBUG("TIMEOUT");
          } else {
            DEBUG("DATA RECEIVED");
          }
          break;
        }
      }
    } else {
      Serial.println("available");
      flag = true;
      c = _serial.read();
      str += c;
    }
  }
  return str;
}

template <typename T> bool AtGenericDriver<T>::AtDebug(char *command) {
  bool isSuccessful = false;
  // DEBUG("SIM7600CE Test Started...");
  Serial.print("Sending AT command : \"");
  Serial.print(command);
  DEBUG("\"");
  String str = sendATCommand(command);
  DEBUG("The response is: ");
  DEBUG(str);
  DEBUG("response ended.");
  isSuccessful = areYouOkay(str);
  if (isSuccessful) {
    DEBUG("OK detected");
  } else {
    DEBUG("OK not detected");
  }
  // DEBUG((isSuccessful) ? "OK detected" : "OK indetected");
  return isSuccessful;
}

template <typename T> bool AtGenericDriver<T>::areYouOkay(String str) {
  bool O = false;
  char ca[str.length()];
  str.toCharArray(ca, str.length());
  for (char c : ca) {
    if (c == 'O') {
      O = true;
      continue;
    }
    if (O) {
      if (c == 'K') {
        return true;
      } else {
        O = false;
      }
    }
  }
  return false;
}

template <typename T> bool AtGenericDriver<T>::forcedRefresh() {
  String raw = this->sendATCommand("AT");
  if (raw == String("")) {
    return false;
  }
  if (!areYouOkay(raw)) {
    return false;
  }
  return true;
}

#ifdef __GSM_SOFTWARE_SERIAL__
#include <SoftwareSerial.h>
template class AtGenericDriver<SoftwareSerial>;
#else
template class AtGenericDriver<HardwareSerial>;
#endif
