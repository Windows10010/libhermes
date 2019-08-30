#include <Timing.h>

#ifndef INC_ARDUINO_FREERTOS_H
unsigned long getSysTime() {
  return (unsigned long)millis();
}
#else
unsigned long getSysTime() {
  TickType_t tickCount = xTaskGetTickCount();
  return ((unsigned long)tickCount) * portTICK_PERIOD_MS;
}
#endif
