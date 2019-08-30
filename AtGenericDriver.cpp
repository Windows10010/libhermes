#include "AtGenericDriver.h"

using namespace hermes;

template <typename T>
AtGenericDriver<T>::AtGenericDriver(T &ss) : _serial(ss)
{
    _serial.begin(AT_SERIAL_BAUDRATE);
}

template <typename T>
String AtGenericDriver<T>::sendATCommand(char *command)
{
    _serial.println(command);
    // TickType_t staort = xTaskGetTickCount();
    TickType_t start;
    char c;
    String str("");
    bool flag = true;
    while (true)
    {   
        if(!this->_serial.available()){
            if(flag){
                // start = millis();
                start = xTaskGetTickCount();
                flag = !flag;
            }else{
                if(((unsigned long)(xTaskGetTickCount() - start))*portTICK_PERIOD_MS > 5000){
                    if(str.equals(String(""))){
                        Serial.println("TIMEOUT");
                    }else{
                        Serial.println("DATA RECEIVED");
                    }
                    break;
                }
            }
        }else{
            flag = true;
            c = _serial.read();
            str += c;
        }
    }
    return str;
}

template <typename T>
bool AtGenericDriver<T>::AtDebug(char *command)
{
    bool isSuccessful = false;
    // Serial.println("SIM7600CE Test Started...");
    Serial.print("Sending AT command : \"");
    Serial.print(command);
    Serial.println("\"");
    String str = sendATCommand(command);
    Serial.println("The response is: ");
    Serial.println(str);
    Serial.println("response ended.");
    isSuccessful = areYouOkay(str);
    Serial.println((isSuccessful) ? "OK detected" : "OK indetected");
    return isSuccessful;
}

template <typename T>
bool AtGenericDriver<T>::areYouOkay(String str)
{
    bool O = false;
    char ca[str.length()];
    str.toCharArray(ca, str.length());
    for (char c : ca)
    {
        if (c == 'O')
        {
            O = true;
            continue;
        }
        if (O)
        {
            if (c == 'K')
            {
                return true;
            }
            else
            {
                O = false;
            }
        }
    }
    return false;
}

template <typename T>
bool AtGenericDriver<T>::forcedRefresh()
{
    String raw = this->sendATCommand("AT");
    if (raw == String(""))
    {
        return false;
    }
    if (!areYouOkay(raw))
    {
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
