# NFC Locker

## Introduction
Unlock the locker with Octopus Card

## Implementation

## Hardware

### Connection

        MEGA                  NFC Module
    +------------+            +-------+
    +        GND +------------+  GND  +
    +       +5V  +------------+  VCC  +
    + (SDA)  D20 +------------+  SDA  +
    + (SCL)  D21 +------------+  SCL  +
	+------------+            +-------+

                               LCD2004
	    MEGA            (HD44780 w/ PCF8574)
    +------------+            +-------+
    +        GND +------------+  GND  +
    +       +5V  +------------+  VCC  +
    + (SDA)  D20 +------------+  SDA  +
    + (SCL)  D21 +------------+  SCL  +
	+------------+            +-------+

        MEGA            Notched Shaft Encoder
	+------------+            +-------+
    +       GND  +------------+  GND  +
    +        D2  +------------+  S1   +  
	+        D3  +------------+  S2   +  
	+        D5  +------------+  KEY  +
    +       +5V  +------------+  5V   +
    +------------+            +-------+

        MEGA                   Buzzer
    +------------+            +-------+
    +        D9  +------------+  Out  +
    +        5V  +------------+  VCC  +
    +        GND +------------+  GND  +
	+------------+            +-------+

        MEGA                 Relay Module
    +------------+            +-------+
    +        D10 +------------+   S   +
    +        +5V +------------+   +   +
    +        GND +------------+   -   +
	+------------+            +-------+

        Relay                      Lock
    +------------+            +-------------+
    +         NO +------------+ solenoid P1 +
    +        COM +--|    |----+ solenoid P2 +
    +         NC +  |    |    +             +
    +            +  |    |    +             +
    +       +12V +--|    |    +             +
    +        GND +------ |    +             +
    +------------+            +             +
                              +             +
         MEGA                 +             +
	+------------+            +             +
    +        D47 +-------|----+  Switch P1  +- 0.1 uF -|
    +        +5V +-4.7kΩ-|  |-+  Switch P2  +----------|
    +        GND +----------| +-------------+
	+------------+             



#### Buzzer Problem
I found out that pwm output  / tone() on `pin 11` will affect the i2c performance. The i2c connection will be unstable and the mcu may be not responsive. I use `pin 9` for buzzer connection avoiding this problem.

## Software


### Library
LCD2004 driver: [hd44780](https://github.com/duinoWitchery/hd44780) library

NFC: 

Menu Selector: 

## Usage

## Reference
