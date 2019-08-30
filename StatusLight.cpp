#include <StatusLight.h>

namespace hermes
{

StatusLight::StatusLight(uint8_t index) : Adafruit_NeoPixel(NUMBER_PIXELS, index, NEO_GRB + NEO_KHZ800), _blinkingState(false), _blinkingInterval(DEFAULT_BLINKING_INTERVAL)
{
    this->_lock = xSemaphoreCreateMutex();
    if (_lock != NULL)
    {
        xSemaphoreGive(this->_lock);
    }
    for (int i = 0; i < (sizeof(buffers) / sizeof(buffers[0])); i++)
    {
        buffers[i] = ledStatus::undefined;
    }
    this->setBrightness(255);
    this->begin();
}

void StatusLight::setLed(int index, ledStatus status)
{
    if (index > 10 || index < 0)
        return;
    if (xSemaphoreTake(this->_lock, (TickType_t)(200 / portTICK_PERIOD_MS)) == pdTRUE)
    {
        this->buffers[index] = status;
        xSemaphoreGive(this->_lock);
    }
}

ledStatus StatusLight::getLed(int index)
{
    if (index >= 10 || index < 0)
        return;
    ledStatus temp;
    if (xSemaphoreTake(this->_lock, (TickType_t)(200 / portTICK_PERIOD_MS)) == pdTRUE)
    {
        temp = this->buffers[index];
        xSemaphoreGive(this->_lock);
    }
    return temp;
}

void StatusLight::lightOff(int index)
{
    if (index < 0 || index >= 10)
    {
        return;
    }
    this->setPixelColor(index, this->Color(0, 0, 0));
}

void StatusLight::controller()
{

    if ((int)((int)getSysTime() / this->_blinkingInterval) % 2 == 0)
    {
        this->_blinkingState = false;
    }
    else
    {
        this->_blinkingState = true;
    }
    this->show();
    for (int i = 0; i < 10; i++)
    {
        switch (buffers[i])
        {
        case ledStatus::initializing: //green light blinking...
            if (_blinkingState)
            {
                this->setPixelColor(i, this->Color(0, 255, 0));
            }
            else
            {
                this->lightOff(i);
            }
            break;
        case ledStatus::operating: //constant green light
            this->setPixelColor(i, this->Color(0, 255, 0));
            break;
        case ledStatus::error: //red light blinking
            if (this->_blinkingState)
            {
                this->setPixelColor(i, this->Color(255, 0, 0));
            }
            else
            {
                this->lightOff(i);
            }
            break;
        case ledStatus::undefined: //orenge light blinking
            if (this->_blinkingState)
            {
                this->setPixelColor(i, this->Color(255, 128, 0));
            }
            else
            {
                this->lightOff(i);
            }
            break;
        }
    }
}

int StatusLight::getBlinkingInterval(){
    return this->getBlinkingInterval;
}

void StatusLight::setBlinkingInterval(const int sec){
    this->_blinkingInterval = sec;
}

void tStatusLightController(void *pvParameters)
{
    (void)pvParameters;
    while (true)
    {
        statusLight.controller();
    }
}

} // namespace hermes
