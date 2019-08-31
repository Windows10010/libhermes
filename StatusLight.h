#ifndef STATUS_LIGHT_H
#define STATUS_LIGHT_H

#define NUMBER_PIXELS 10
#define DEFAULT_BLINKING_INTERVAL 300

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <Arduino_FreeRTOS.h>
#include <Timing.h>
#include <semphr.h>
#include <string.h>

using namespace std;

namespace hermes {

enum class ledStatus { initializing, operating, error, undefined };

class StatusLight : private Adafruit_NeoPixel {

public:
  StatusLight(uint8_t index);

  void setLed(int index, ledStatus status);

  ledStatus getLed(int index);

  void controller();

  void setBlinkingInterval(const int sec);

  int getBlinkingInterval();

private:
  void lightOff(int index);

  unsigned long tgetSysTime();

  ledStatus buffers[10];
  SemaphoreHandle_t _lock;
  bool _blinkingState;
  int _blinkingInterval;
};

void tStatusLightController(void *pvParameters);
} // namespace hermes

extern hermes::StatusLight statusLight;

#endif