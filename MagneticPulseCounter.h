#ifndef MAGNETIC_PULSE_COUNTER_H
#define MAGNETIC_PULSE_COUNTER_H

#include "MicroBit.h"

#define MAGNETIC_PULSE_ID 2001

#define MAGNETIC_PULSE_EVT_RISE               2
#define MAGNETIC_PULSE_EVT_FALL               3
#define MAGNETIC_PULSE_EVT_PULSE_HI           4
#define MAGNETIC_PULSE_EVT_PULSE_LO           5

#define MAGNETIC_PULSE_EVT_UPDATE 1

class MagneticPulseCounter
{

private:
    /* data */

    // microbit runtime instance
    MicroBit &uBit;

    /**
     * Timestamp when the previous fall/rise event. 
     */
    uint64_t lastTimestamp;

    /**
     * Strength of magnetic force. 
     */
    int strength;

    /**
     * Threshold value of magnetic force strenght. 
     */
    int strengthThreshold = 1000;

    uint64_t pulseWidth;

    /**
      * Interrupt handler for when an rise interrupt is triggered.
      */
    void onRise();

    /**
      * Interrupt handler for when an fall interrupt is triggered.
      */
    void onFall();

    /**
     * This member function manages the calculation of the timestamp of a pulse detected
     * on magnetic force.
     *
     * @param eventValue the event value to distribute onto the message bus.
     */
    void pulseWidthEvent(int eventValue);

    void countPulse();

public:

    static bool IsUpdating;

    static void StartMagneticPulseCount();

  /**
    * Constructor.
    * Create a representation of the ScratchMoreService
    * @param _uBit The instance of a MicroBit runtime.
    */
    MagneticPulseCounter(MicroBit &_uBit);

    ~MagneticPulseCounter();

    void startCount();

    void countLoop();

    void compassUpdate(MicroBitEvent);

    void setStrengthThreshold(int threshold);

    int getStrengthThreshold();

    int getPulseWidth();
};


#endif