#include <QDebug>
#include <lgpio.h>

#include "gpio.h"
#include "config.h"

gpio::gpio(QObject *parent) : QObject(parent)
{
    m_handle = lgGpiochipOpen(CHIP); // get a handle to the GPIO
    int init_level = 0;
    for (auto pin : LEDS) // Outputs
        lgGpioClaimOutput(m_handle, LFLAGS, pin, init_level);
    for (auto pin : BUTTONS) // Inputs
        lgGpioClaimInput(m_handle, LFLAGS, pin);
}

gpio::~gpio()
{
    // Cleanup GPIO
    int init_level = 0;
    for (auto pin : LEDS)
        lgGpioWrite(m_handle, pin, init_level);
    lgGpiochipClose(m_handle);
    qDebug() << "bye";
}

// Write to pins
void gpio::set(int pin, bool value)
{
    lgGpioWrite(m_handle, pin, value);
}

void gpio::set(unsigned int pattern)
{
    int n = 0;
    int value = 0;
    for (auto pin: LEDS) {
       // n-te Stelle vom pattern ausmaskieren, value =
       lgGpioWrite(m_handle, pin, value);
       // Maske um 1 Stelle nach vorne verschieben
    }
}


// Read pin state
bool gpio::get(int pin)
{
    return lgGpioRead(m_handle, pin);
}
