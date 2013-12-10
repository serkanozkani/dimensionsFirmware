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
    Ramps();
    void setup();
    void loop(int ms);

    PoluluStepper getXMotor();
    PoluluStepper getYMotor();
    PoluluStepper getZMotor();

    LedIndicator getLedIndicator();

    ExtruderController getExtruderA();

    HeatbedController getHeatbed();

    
  private:
    PoluluStepper _xMotor;
    PoluluStepper _yMotor;
    PoluluStepper _zMotor;

    EndstopSwitch _xEndstop;
    EndstopSwitch _yEndstop;
    EndstopSwitch _zEndstop;

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