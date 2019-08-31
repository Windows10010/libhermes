#ifndef GSM_DRIVER_H
#define GSM_DRIVER_H

// #define DEBUG(x) Serial.println(x)
#define DEBUG(x)                                                               \
  { std::cout << x << std::endl; }
#define OK "OK"

#define DEFAULT_SMS_RECIPIENT "13152068453"

#ifdef INC_ARDUINO_FREERTOS_H
#define __CLOCK__() (xTaskGetTickCount() * portTICK_PERIOD_MS)
// #pragma messege "(freeRTOS library detected) Using getTickCount as the system
// clock function." OK
#else
#define __CLOCK__() millis()
// #define messege "(freeRTOS library not detected) Using millis as the system
// clock function." OK
#endif

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <AtGenericDriver.h>
#include <string.h>

namespace hermes {

typedef struct {

  double longitude;
  double latitude;
  double altitude;
  int year;
  int month;
  int day;
  int minute;
  int second;

} GpsData;

template <typename T> class GsmDriver : public AtGenericDriver<T> {
public:
  GsmDriver(T &ss);

  bool init();

  String sendSms(String messege);

  String getGpsInfo();

  void setPhoneNumber(String tel);

  void setPhoneNumber(char *tel);

  String getPhoneNumber();

  bool isInitialized();

private:
  bool _isInitialized;

  String _phoneNumber;
};

} // namespace hermes

#endif