# Generic Car Controller
It is a device designed to improve the capabilities of vehicles in an automotive environment.
This custom controller connects to an existing CAN Bus on the vehicle and acts like another component of the system.

### Motivation
It is very common that certain car features are locked by the manufacturers in its mid/low-series products or that its embedded systems are restricted to work with a limited array of devices.
There even are internal parameters hidden to customer's usage. These parameters may be helpful in order to predict potential malfunctions and enhances the security and the reliability of the vehicle.

### Compatibility with specific car models
Even though the project is developed and tested using an Opel Astra H (2004) it can be adapted and used with any vehicle which uses a CAN BUS network to communicate across its devices.

Other car models must be defined inside the "cars/" folder following the defined structure in the "DefaultCar" model.

### Hardware setup
The project is not closed to a specific hardware device but it is developed focusing on the ATMega brand microprocessors with multiple peripheral connected via the SPI interface.

The latest prototype version (4.3) was built using the following components:
* 1x - Arduino Nano v3 (ATMega 328p)
* 1x - MCP 2515 CAN Controller board
* 1x - MCP4131 Potentiometer
* 1x - PC817 Optocoupler
* 1x - HC-05 Bluetooth interface (Optional, used for wireless debugging and configuration)
* 1x - SIM800L GSM Module
* 1x - GY-NEO6MV2 GPS Module

You can build your own with the schematic and the gerber file located inside the "Board/" folder.
<img src='img/v43_board.png?raw=true' width='350px' height='247px' alt='Prototype V4.3' />

### Previous prototypes
For learning purposes you can see inside the "img/" folder the progression of already built prototypes and how they were improved. 
<br/>
<img src='img/prototypes_front.png?raw=true' width='400px' height='255px' alt='Prototypes (Front view)' />
<img src='img/prototypes_back.png?raw=true' width='400px' height='255px' alt='Prototype (Back view)' />

### License
Author:	  Álvaro Graciá Gil<br/>
License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License<br/>
	 		    (http://creativecommons.org/licenses/by-nc-sa/4.0/)<br/>
