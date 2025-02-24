# Switches Actions
Switches are provided on the JIG controller board to do different tasks based on different requirements.

## Switch State
`0` = Switch is ON (i.e. jumper is connected)
`1` = Switch is OFF (i.e. jumper is not connected)

## Different Switches Actions
1. `SW_IN_1`: To check EEPROM, set date and time after getting from PC, Format the EEPROM
    1. `EEPROM Format`: when we issue a fromat command, we change the header in meter code after which meter resets itself and upon starting it checks the header. if header is not there then meter formats the EEPROM. During this operation meter is not able to send the acknowledgement and hence in tester code we have provided the delay for 60 sec for 1-Ph and additional 15sec fro 3-ph. Now for 3-ph we can combine both.
2. `SW_IN_2`: Meter reset test
3. `SW_IN_3`: This is used to check the power latch (relay) of meter.
    1. currently it runs in all cases. 
    2. and within `checkRelayStatus` function, it checks the pulses only or complete AC output based on the presense of latach or not. as in normal PCB latches will not be there.
    3. `This test will not be done for LTCT meter`
4. `SW_IN_4`: To test current and voltage and send current packet to PC if switch is connected.
    1. cuurently we do both 1-ph and 3-ph in this and in 3-Ph we don't check of blinking which needs to be added.
    2. To check the supply voltages and current we apply the voltage and current through `phantom load` and measure the values.
    3. In 3-ph we store the results in different locations (5,6,7) these can be combined to single location and other location can be used for blinking result.
    4. In led blinking earlier there were on active LED was there. now, reactive needs to be added. 
5. `SW_IN_5`: RF + IO of RF 
    1. first we check the port by getting the response
    2. then we make RF pins Low and verify the same from RF
    3. then we make RF pins High and verify the same from RF
6. `SW_IN_6`: Switch Check
    In 3-Ph meter there are 3 switches and 1 magnet, hence total 4. 
    1. Make relay OFF. it turn OFF the relay which press the switches and magnet in real JIG.
    2. We get the release check packet from meter. It retries 5 times. State of switches is different for different switches in 3-Ph meter.
    3. Turn ON the relay to press the switches.
    4. Again the the switch press packet from meter.
7. `SW_IN_7`: Check battery volatge and current and board voltages at different points.
    1. This is done at two points for 1-Ph now in 3-Ph it can be done at one place. 
8. `SW_IN_8`: It is for Magnet check during the switch checking of `SW_IN_6`.
9. `SW_IN_9`: In case same board is used for another Jig that is used only for sending Clear 1 command... for formatting the whole Meter. So if at power up, this jumper is present, it will only perform this operation.
10. `SW_IN_10_PC`: This jumper is used if PC is communicating with the tester board.
11. 
12. `SW_IN_12_9600_BAUD`: To switch between baud rates.
    *  `0`: 19200
    *  `1`: 9600

