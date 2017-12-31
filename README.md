# 8x8x8-RGB-LED-Cube
This project is an 8x8x8 cube that animates a 3D matrix of LEDs. It is written using the Arduino IDE and programmed onto an ATmega328P with no bootloader via a USBASP.

The cube consists of 512 RGB LEDs, over 200 ft of wire, 192 transistors, 8 MOSFETs, 393 resistors, 25 shift registers, 1 ATmega328P, and a handful of other components.

In order to connect and control 1536 LEDs, the cube has each of the 8 layers multiplexed. This leaves 192 LEDs and 1 MOSFET per layer to control. I use 24 74HC595 8-bit shift registers which communicate via SPI to control the LEDS and 1 shift register to control the MOSFETS and which layer is active.

Each LED is capable of producing 16 levels of brightness by using 4-bit BAM (Bit Angle Modulation). Each RGB can therefore produce 4096 unique colors.

<h1>Schematic</h1>
Unfortunately my "schematics" were made before I knew how to use Eagle and therefore I made them in Photoshop.
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Schematic/LargeRGBCubeSchematic.png?raw=true" alt="8x8x8-RGB-LED-Cube Preview Schematic">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Schematic/LargeRGBCubeSchematic2.png?raw=true" alt="8x8x8-RGB-LED-Cube Hookup Schematic">

<h1>Pictures</h1>
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMAG0233.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 1">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_231849.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 2">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_231916.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 3">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232000.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 4">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232116.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 5">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232141.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 6">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232241.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 7">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232335.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 8">
<img src="https://github.com/JamesLenz/8x8x8-RGB-LED-Cube/blob/master/Images/IMG_20171230_232521.jpg?raw=true" alt="8x8x8-RGB-LED-Cube Image 9">
