/**
  * Class definition for the Magnetic Pulse Counter.
  * Count the width of the magnetic field strength rise and fall. 
  */
#include "MagneticPulseCounter.h"

bool MagneticPulseCounter::IsUpdating = false;

void MagneticPulseCounter::StartMagneticPulseCount()
{
    if (IsUpdating)
        return;

    IsUpdating = true;
    while (IsUpdating)
    {
        MicroBitEvent ev(MAGNETIC_PULSE_ID, MAGNETIC_PULSE_EVT_UPDATE);
        fiber_sleep(10);
    }
}

MagneticPulseCounter::MagneticPulseCounter(MicroBit &_uBit)
    : uBit(_uBit)
{
    uBit.compass.setPeriod(10); // milli-seconds (MAG3110 12.5ms/80Hz .. 12.8s/0.08Hz)
    // Setup callbacks for event. 
    // uBit.messageBus.listen(MICROBIT_ID_COMPASS, MICROBIT_COMPASS_EVT_DATA_UPDATE, this, &MagneticPulseCounter::compassUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE); // This is case to clash. 
    uBit.messageBus.listen(MAGNETIC_PULSE_ID, MAGNETIC_PULSE_EVT_UPDATE, this, &MagneticPulseCounter::compassUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

MagneticPulseCounter::~MagneticPulseCounter()
{
}

void MagneticPulseCounter::setStrengthThreshold(int threshold) {
    strengthThreshold = threshold;
}

int MagneticPulseCounter::getStrengthThreshold() {
    return strengthThreshold;
}

int MagneticPulseCounter::getPulseWidth() {
    return (int)pulseWidth;
}

void MagneticPulseCounter::compassUpdate(MicroBitEvent evt)
{
    countPulse();
}

void MagneticPulseCounter::startCount()
{
    // create_fiber(this->countLoop);
}

void MagneticPulseCounter::countLoop()
{
    while (1)
        {
            countPulse();
            fiber_sleep(20);
        }
}

void MagneticPulseCounter::countPulse()
{
    if (strength < strengthThreshold){
        strength = uBit.compass.getFieldStrength() / 1000;
        if (strength > strengthThreshold) {
            onRise();
        }
    } else {
        strength = uBit.compass.getFieldStrength() / 1000;
        if (strength < strengthThreshold) {
            onFall();
        }
    }
}

/**
  * This member function manages the calculation of the timestamp of a pulse detected
  * on magnetic force.
  *
  * @param eventValue the event value to distribute onto the message bus.
  */
void MagneticPulseCounter::pulseWidthEvent(int eventValue)
{
    MicroBitEvent evt(MAGNETIC_PULSE_ID, eventValue, CREATE_ONLY);
    uint64_t now = evt.timestamp; // micro seconds. 
    uint64_t previous = lastTimestamp;

    if (previous != 0)
    {
        evt.timestamp -= previous;
        pulseWidth = evt.timestamp;
        evt.fire();
    }

    lastTimestamp = now;
}

/**
  * Interrupt handler for when an rise interrupt is triggered.
  */
void MagneticPulseCounter::onRise()
{
    pulseWidthEvent(MAGNETIC_PULSE_EVT_PULSE_LO);
    MicroBitEvent(MAGNETIC_PULSE_ID, MAGNETIC_PULSE_EVT_RISE);
}

/**
  * Interrupt handler for when an fall interrupt is triggered.
  */
void MagneticPulseCounter::onFall()
{
    pulseWidthEvent(MAGNETIC_PULSE_EVT_PULSE_HI);
    MicroBitEvent(MAGNETIC_PULSE_ID, MAGNETIC_PULSE_EVT_FALL);
}
