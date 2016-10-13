# openhoop
a place to store the development of an open source, sound-reactive, movement-aware hulahoop

## Intro

The aim of this project is to create an easy-to-assemble, [Arduino](https://www.arduino.cc/)-based [LED Hulahoop](https://en.wikipedia.org/wiki/Hooping#LED_hooping). I spent almost two year collecting information [here](https://www.tumblr.com/blog/openhoop), made a first protype sprint in 2015, and now I’d like to make PCB prototypes.

OpenHoop LED’s behaviours features:

* responsiveness to the sound
* responsiveness to the performer movement / hulahoop rotation
* light-painting

## Hardware Modules

The setup implies:

* a professional DIY hulahoop
* MCU (Arduino Micro)
* ~~6DOF sensor (MPU6050)~~ 9DOF IMU ([LSM9DS1TR](http://www.mouser.it/ProductDetail/STMicroelectronics/LSM9DS1TR/?qs=4b8myOmUP%252bsiwRDrJOMxKQ%3d%3d)) 
* MSGEQ7 graphic equalizer and electret mic
* 4 AAA batteries and a rechargeable circuit or a lithium iron phosphate battery management (based on [MCP73123](http://www.microchip.com/wwwproducts/en/MCP73123))
* APA102 RGB LEDs
* a little Joystick as UI
* (tentative) BLE coonection with basic upload capability

Detailed BOM to be relased soon

## Software Modules and libraries

OpenHoop is using [Fastled](http://fastled.io/) as LED Library, a simple and little code snippet for taking data from the [MPU6050](http://playground.arduino.cc/Main/MPU-6050) and the simplest way to receive data from the [MSGEQ7](https://www.sparkfun.com/products/10468).
