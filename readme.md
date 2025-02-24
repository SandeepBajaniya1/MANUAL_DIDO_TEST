# JIG 3-Ph LTCT Meters Tester Boards (v1.0.0)
This code needs to be modified according to 3-Ph only.


## Switch Functionalities
Switches are provide to do certain tasks in some cases and some tasks in other cases.

More about switches can be found [here](./switches_actions/switches.md)


## Different Commands Format to communicate with JIG

Data Frame Format Sent to the JIG Controller
				
header	|Frame length	|meter no	|Command ID	|Data|	Frame End
---|---|---|---|---|---
6-Bytes|	1-Byte|	4-Byte	|1-Byte|	optinal|	1-Bytes
$JIG02	| |	0 0 0 0	| | |		#

**Example**

24	4a	49	47	30	32|	13	|0	0	0	0|	32	|73	61	63	68	69	6E|	23
---|---|---|---|---|---
Header|						Frame Length|	meter no	|			Command|	Data| (temporary)						|Frame End



**Get Internal Firmware Version**

```24 4a 49 47 30 32 13 00 00 00 00 50 73 61 63 68 69 6e 23```


## RTC Calibration
Related document can be found [here](./rtc_calibration_docs/README_RTC_CALIBRATION.md)



## Total Tests

1. RTC Battery
2. Main Battery
3. Sleep Current
4. Board Volage:
    1. DVCC
    2. 6.5 V
5. Super Cap Voltage
6. Optical Communication Test
7. I2C test
8. RTC Calibration
9. Date and Time Set
10. Meter Internal Firmware
11. Board ID 
12. Clear 1
13. Reset
14. Main Voltages and Currents
15. KWh
16. RF Test
17. Switch Tests
    1. Cover Open
    2. Up Switch
    3. Down Switch

