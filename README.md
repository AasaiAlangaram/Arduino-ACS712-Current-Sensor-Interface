# Arduino-ACS712 Current-Sensor Interface

ACS712 - Fully Integrated, Hall Effect-Based Linear Current Sensor. It provide AC or DC Current Sensing. The ouput of the device has a positive slope(>VIOUT(Q)).When Current starts flowing through conductio path from pin 1 to 2 to pin 3 and 4, which is path used for current sensing.

## Sensitivity Reference

| Part Number | Sensitivity(mV/A) |
| ----------- | ----------- |
| ACS712ELCTR-05B-T | 185 |
| ACS712ELCTR-20A-T | 100 |
| ACS712ELCTR-30A-T | 66 |

![Datasheet Diagram](https://github.com/AasaiAlangaram/Arduino-ACS712-Current-Sensor-Interface/blob/master/Images/ACS712.PNG)

## Schematic

![Schematic](https://github.com/AasaiAlangaram/Arduino-Python-PYQT5-Serial-Communication/blob/master/Schematic.PNG)

## Analog Inputs
| `Arduino Uno` | `Arduino Mega` |
| ----------- | ----------- |
| 6 | 16 |

## Code 
⋅⋅* First define input analog pin 
```
const int analogIn = 0; //Connect current sensor with A0 of Arduino
```
⋅⋅* Read Analog Value
```
analog_value = analogRead(Pin number);  
```

⋅⋅* Current Calculation
-------------------
((Analog_value * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts 2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes out to be 2.5 which is out offset. If your arduino is working on different voltage than you must change the offset according to the input voltage) 0.185v(185mV) is rise in output voltage when 1A current flows at input.
```
float mVperAmp = 0.185; //Use 185 for 5A Sensor
float ACoffset = 2.5;
float involtage = 5.0;
float ADC10bit_resolrange = 1024.0;

f_AcsValue = ((analog_value * (involtage / ADC10bit_resolrange)) - ACoffset )/mVperAmp;
```
