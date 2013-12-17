/*  * Ramps.h
    *
    * This file is subject to the terms and conditions defined in
    * file 'LICENSE', which is part of this source code package.
    *
    * Authored by: Alexander Stokes
    * License: CC by-sa 4.0
    * Owner: Alexander Stokes LLC
    * Written: 2013-12-10
    */


#ifndef Ramps_h
#define Ramps_h

#include "Arduino.h"

// The components
#include "PoluluStepper.h"
#include "LedIndicator.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"

// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"

class Ramps
{
  public:
    static Ramps& instance()
    {
        static Ramps instance;
        return instance;
    }

    void setup();
    void loop(unsigned long ms);

    PoluluStepper * getMotorX();
    PoluluStepper * getMotorY();
    PoluluStepper * getMotorZ();

    EndstopSwitch * getEndstopX();
    EndstopSwitch * getEndstopY();
    EndstopSwitch * getEndstopZ();

    LedIndicator * getLedIndicator();

    PoluluStepper * getExtruderMotorA();

    ExtruderController * getExtruderA();

    HeatbedController * getHeatbed();

  private:
    // 3 intentionally unimplemented constructors
    // or operators to prevent extra instances.
    Ramps() {};
    Ramps(Ramps const&);
    void operator=(Ramps const&);

    // The rest is 
    PoluluStepper _motorX;
    PoluluStepper _motorY;
    PoluluStepper _motorZ;

    EndstopSwitch _endstopX;
    EndstopSwitch _endstopY;
    EndstopSwitch _endstopZ;

    LedIndicator _ledIndicator;

    Thermistor _bedThermistor;
    Thermistor _extruderThermistorA;
    //Thermistor _extruderThermistorB;

    PoluluStepper _extruderA;
    //PoluluStepper _extruderB;

    ExtruderController _extruderControllerA;

    HeatbedController _heatbedController;
};

#endif