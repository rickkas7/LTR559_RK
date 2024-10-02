# LTR559_RK

*LTR559 light sensor library for Particle*

This library is not complete but does have enough functions implemented to read and
proximity sensor (PS) and ambient light sensor (ALS) data.

This library is intended for use with the Pimoroni enviro + air quality (PIM458)
hat. Since the hat does not implement the interrupt line to the MCU, those functions
are not completely implemented in this library as they wouldn't be particularly
useful.
