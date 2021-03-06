dimensionsFirmware
==================

Dimensions Firmware is a Ramps 1.4-compatible firmware designed for the Rep Rap Prusia Mendel and its derivatives.

### [Api 0.2.5 Documentation](https://github.com/stokes-forit/dimensionsFirmware/wiki/API-0.2.5)

## Project Status

    - Currently this project is in development and is incomplete.

## Premise
The current stack for the Rep Rap 3D printer has a set of software packages built upon
the need to develop while testing new hardware platforms and various modifications. This
firmware is a from-scratch rebuild starting from the readily available RAMPS 1.4 board
with an overall design goal of making the source code and slicing programs more tightly
connected with fewer firmware setbacks, and a well-defined open-source license. Currently
this project is not functional, and new RepRap users should check out the [RepRap Wiki](http://reprap.org/wiki/Main_Page).

## External projects for RepRap:
[Sprinter Firmware](https://github.com/kliment/Sprinter)

[Slic3r Software](http://slic3r.org/)

[Pronterface](https://github.com/kliment/Printrun)

[RepRap Wiki](http://reprap.org/wiki/Main_Page)

## License

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Dimensions Firmware</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/stokes-forit/dimensionsFirmware" property="cc:attributionName" rel="cc:attributionURL">Alexander Stokes LLC</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

## Project Timeline
    - [Reverse chronological order]
    - 2013-12-23 Api version bump. 0.2.5
    - 2013-12-23 Deleted [testMotorX]. [testMotors] replaced it functionally
    - 2013-12-23 Built new class LinearMotionController to remove redundant code from CartesianController
    - 2013-12-21 Built new class HeaterController to remove redundant code from ExtruderController
    - 2013-12-19 Api version bump. 0.2.0
    - 2013-12-19 Started [testHotend, testHeatbed, pollExtruder]
    - 2013-12-19 Tested [navigate, status, lockout, pollHeatbed]
    - 2013-12-16 All tests pass
    - 2013-12-16 New class CartesianController will handle all 2d motion.
    - 2013-12-16 Massive changes to Ramps.cpp (making it a shared singleton instance)
    - 2013-12-15 Started Node.js test code
    - 2013-12-12 Tested [calibrateX, calibrateY] successfully
    - 2013-12-12 Tested [reset, emergencyStop, testEndstopX, testMotorX] successfully
    - 2013-12-11 Wrote simple command parser library.
    - 2013-12-10 Published progress to github, licensed cc by-sa 4.0
    - 2013-12-09 Broke out to current library format etc.
    - 2013-12-08 Testing basic led functionality etc.
	- 2013-12-07 Started lower-level design and sketches.
	- [Project Started]
