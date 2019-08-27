#include <GsmDriver.h>
using namespace hermes;

template <typename T>
GsmDriver<T>::GsmDriver(T &ss) : AtGenericDriver<T>(ss) , _phoneNumber(String(DEFAULT_SMS_RECIPIENT))
{
}  

template <typename T>
bool GsmDriver<T>::init(){
    this->_serial.print("\n");
    bool _isOkay = false;
    String response = this->sendATCommand("AT");
    _isOkay = this->areYouOkay(response);

    DEBUG("Initializing SMS Service");
    AtDebug("AT+CMGF=1");

    DEBUG("Initializing GPS:");
    AtDebug("AT+CGPS=0");
    delay(2000);
    AtDebug("AT+CGPS=1,1");
    delay(2000);
    
    this->_isInitialized = _isOkay;
    return _isOkay;
}

template <typename T>
String GsmDriver<T>::sendSms(String &messege)
{

    Serial.println("Sending SMS");
    String str = sendATCommand("AT+CMGS=\"13152068453\"");
    this->_serial.println(messege);
    this->_serial.write(0x1A);
    this->_serial.write(0x0D);
    this->_serial.write(0x0A);
    this->_serial.print("\n");
    long start = millis();
    char c;
    while (true)
    {
        if (this->_serial.available())
        {
            Serial.println("SERIAL AVAILAIBLE");
            break;
        }
        if (millis() - start > 3000)
        {
            Serial.println("SERIAL TIMEOUT");
            return String("");
        }
    }
    str = String("");
    while (this->_serial.available())
    {
        delay(70);
        c = this->_serial.read();
        str += c;
    }
    return str;
}

String extractGpsInfo(String raw)
{
    String frontPart = raw.substring(12, raw.length() - 3);
    return frontPart;
}

String delete_returns(String str)
{
    String newString = str;
    newString.replace(String("\n"), String(""));
    return newString;
}

template <typename T>
String GsmDriver<T>::getGpsInfo()
{
    DEBUG("Sending AT command: \n\"AT+CGPSINFO\"");
    String rawGpsInfo = sendATCommand("AT+CGPSINFO");
    bool isOkay = areYouOkay(rawGpsInfo);
    DEBUG((isOkay) ? "The response is OK" : "The command failed");
    DEBUG("The response starts:");
    DEBUG("----------------------------------------------");
    DEBUG(rawGpsInfo);
    DEBUG("\n----------------------------------------------");
    DEBUG("The response ends here.");

    if (!isOkay)
    {
        return String("");
    }

    DEBUG("Extracting info from the response...");
    String gpsInfo = extractGpsInfo(delete_returns(rawGpsInfo));
    DEBUG("The extracted info:");
    DEBUG(gpsInfo);
    return gpsInfo;
}

#ifdef __GSM_SOFTWARE_SERIAL__
#include <SoftwareSerial.h>
template class GsmDriver<SoftwareSerial>;
#else
template class GsmDriver<HardwareSerial>;
#endif