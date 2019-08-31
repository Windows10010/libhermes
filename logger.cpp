#include <logger.h>

#ifdef INC_ARDUINO_FREERTOS_H
float getTimeIndication() {
  float rawSystemTimeInMillisecond =
      (xTaskGetTickCount() * portTICK_PERIOD_MS) / 1000.0;
  return rawSystemTimeInMillisecond;
}
#else
float getTimeIndication() {
  float rawSystemTimeInMillisecond = (unsigned long)millis() / 1000.0;
  return rawSystemTimeInMillisecond;
}
#endif