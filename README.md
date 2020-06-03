# samd21_QFP_reference_PCB - based on the Adafruit ItsyBitsy M0

This is a reference design for a SAMD21 for easy SWD programming and testing.  This is a copy of the [Adafruit itsyBitsy m0](https://www.adafruit.com/product/3727)... Using the SAMD21G QFP package among other small part changes. It does not include the memory module for circuit python use. This is similar to the other QFP packages with less memory that I was working with [here](https://github.com/hydronics2/samd21_QFP_reference_PCB).

The QFP package is pretty easy to solder and gives you about the same amount of pins as the original Aruduino 328P.

![](https://github.com/hydronics2/2020-samd21_QFP_Reference/blob/master/pics/PCB_top.png)


You can order boards from Oshpark using this link: [project](https://oshpark.com/shared_projects/W1OeQO5j)

Here's a list of parts:
- ATSAMD21G from [digikey](https://www.digikey.com/product-detail/en/microchip-technology/ATSAMD21G18A-AUT/ATSAMD21G18A-AUTCT-ND/4878879).
  -	ARM® Cortex®-M0+ SAM D21G Microcontroller IC 32-Bit 48MHz 256KB (256K x 8) FLASH 48-TQFP (7x7)
- 10 pin SWD Header
  - I included an SMD version [mouser](https://www.mouser.com/ProductDetail/485-752)
  - an even cooler SMD version [mouser](https://www.mouser.com/ProductDetail/485-4048)
  - and a through hole version [mouser](https://www.mouser.com/ProductDetail/200-SHF10501LDTH)
- 3.3V Regulator - generic (18V max input) [mouser](https://www.mouser.com/ProductDetail/511-LDL1117S50R)
- USB micro connector [digikey](https://www.digikey.com/product-detail/en/amphenol-icc-fci/10118194-0001LF/609-4618-1-ND/2785382)
- generic 6mmx6mm tactile button [sparkfun](https://www.sparkfun.com/products/97)
- screw terminal 5mm pitch [sparkfun](https://www.sparkfun.com/products/8432) or [aliexpress](https://www.aliexpress.com/wholesale?catId=0&initiative_id=SB_20190221221755&SearchText=pcb+screw+terminal)



SWD Programmer/Debugger: [Segger](https://www.digikey.com/product-detail/en/segger-microcontroller-systems/8.08.91-J-LINK-EDU-MINI/899-1061-ND/7387472)

![schematic](https://github.com/hydronics2/2020-samd21_QFP_Reference/blob/master/pics/schematic.JPG)

## Programming

Adafruit has provided a great [tutorial](https://learn.adafruit.com/how-to-program-samd-bootloaders) for flashing the bootloader onto the board using the segger SWD programmer. It's easier than it looks.


You probably already have the booloader loader on your computer! Arduino installs them when you install the board files.
![files](https://github.com/hydronics2/2020-samd21_QFP_Reference/blob/master/pics/bootloaders.JPG)
