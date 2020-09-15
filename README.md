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


## Software

### Library
LCD2004 driver: [duinoWitchery/hd44780](https://github.com/duinoWitchery/hd44780) library

NFC: [Seeed-Studio/PN532](https://github.com/Seeed-Studio/PN532)

Menu Selector: 



## Usage

## Troubleshooting

### 1. Buzzer Problem
I found out that PWM output  / tone() on `pin 11` will affect the i2c performance. The i2c connection will be unstable and the MCU maybe not responsive. 

This is because `pin 11` is controlled by Timer2. The tone() function is also using Timer2 to generate sound. Two of them will share the same hardware resource and create uncertain behavior.

I changed to use `pin 9` for the buzzer connection avoiding this problem after that.


### 2. Timer Problem

The electronic Lock `YF591S` limited the power-up time into 100ms. This value should not excess 200ms or the lock circuit will blow up. We need to design a control flow that cuts off the power supply to the lock before reaching the time limit.

I design a one-shot function that activates the lock at the beginning by pulling the pin signal to HIGH and pull down the signal to Low after 100ms.

In the first implementation, I use Arduino Timer interrupt to fulfil the timing requirement. The function will pull down the power signal of the lock and the timer interrupt will call `Timer.stop()` to stop the timer itself. But some uncertain behaviours happen. For example, the timer interrupt is not executed some of the time.

In the second implementation, I use [Ticker](https://github.com/sstaub/Ticker) rather than the hardware timer to doing the same thing. Ticker can activates the callback function after a certain period and limit the number of execution times of the function. But the disadvantage is that you cannot use delay() on the main loop() anymore. 

However, using delay() on the main loop() is not a good practice since it will slow down the whole execution flow of the program. Using millis() function with some conditional statement(if-else) to limit the execution frequency of the code fragment can provide a better result.


## Reference
