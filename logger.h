#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <string.h>
#include <SoftwareSerial.h>

#define DEFAULT_OUTPUT_BAUD_RATE 9600
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO
#define DEFAULT_WITH_NEW_LINE true
#define DEFAULT_LOG_SERIAL Serial

#define prefixINFO "[INFO]   : "
#define prefixDEBUG "[DEBUG]  : "
#define prefixWARNING "[WARNING]: "
#define prefixERROR "[ERROR]  : "

#define logINFO(x)                                        \
    {                                                     \
        DEFAULT_LOG_SERIAL.print(getTimeIndication(), 3); \
        DEFAULT_LOG_SERIAL.print("->");                  \
        DEFAULT_LOG_SERIAL.print(prefixINFO);             \
        DEFAULT_LOG_SERIAL.println(x);                    \
    }
#define logDEBUG(x)                                       \
    {                                                     \
        DEFAULT_LOG_SERIAL.print(getTimeIndication(), 3); \
        DEFAULT_LOG_SERIAL.print("->");                  \
        DEFAULT_LOG_SERIAL.print(prefixDEBUG);            \
        DEFAULT_LOG_SERIAL.println(x);                    \
    }
#define logWARNING(x)                                     \
    {                                                     \
        DEFAULT_LOG_SERIAL.print(getTimeIndication(), 3); \
        DEFAULT_LOG_SERIAL.print("->");                  \
        DEFAULT_LOG_SERIAL.print(prefixWARNING);          \
        DEFAULT_LOG_SERIAL.println(x);                    \
    }
#define logERROR(x)                                       \
    {                                                     \
        DEFAULT_LOG_SERIAL.print(getTimeIndication(), 3); \
        DEFAULT_LOG_SERIAL.print("->");                  \
        DEFAULT_LOG_SERIAL.print(prefixWARNING);          \
        DEFAULT_LOG_SERIAL.println(x);                    \
    }

float getTimeIndication();

#endif