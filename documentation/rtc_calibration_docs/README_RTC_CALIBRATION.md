# JIG_1PH Controller

This code is written for JIG-1H controller board. This board is used to test the Meter board in JIGs and to do other calibrartion related tasks.

## Computer communication
The board takes the command from computer in specific frame format and then returns the data (if needed) or response in the same format as well.
Frame format for communication with computer application is as follows:

### Frame format when sending data to JIG controller

 header | length | meter no | packet-ID or command ID | Data |  Frame End |
| --- |---|---|---|---|---|
|$JIG02|1-Byte|4-Byte|1-Byte|optinal|'#' (2Byte)|

**example**

```24 4a 49 47 30 32 13 00 00 00 00 32 73 61 63 68 69 6e 23```

### Frame format of data sent by JIG controller to PC

header | length | meter no | packet-ID or command ID | error_code | Data |  Frame End |
| --- |---|---|---|--- |---|---|
|$JIG02|1-Byte|4-Byte|1-Byte | 1-Byte |optinal|'#' (2Byte)|

Note: 
1. this meter number is not being used as of now and will be empty. The term ```internal number``` is used for the barcode or PCB number. It will come from the PC in the Data Field.
2. In response packet, Error code part is implemented for the RTC calibration Only.

**example**

```24 4a 49 47 30 32 13 00 00 00 00 01 73 61 63 68 69 6e 23```

## RTC Calibration Flow
It is for time being as part of this RTC calibration code is integrated with the other code.

1. Send `START_RTC_CALIB`
2. After getting response, send other command related to required operation to be performed
3. Once computer gets response and there is not other task to be done send `STOP_PC_OPERATION` command.

## Get RTC parameters (command ID = 0x35)
It was required to get teh average PPM value in order to validate the compensations operation. Process to read the same was implemented in the command id `0x35`. Originally, it was taking around 50sec (as RTC board need some time to give correct average valuee of 10 pulses). In order to reduce the time, part of this command are implemented in `main` function itself. 

This should be noted that the correct avg. PPM value depends upon time so, if switches are removed and proper time delay is not provided, we may get wrong avg. PPM value.

## Data Format for different Commands

**Start RTC Calibration**

`24 4a 49 47 30 32 13 00 00 00 00 32 73 61 63 68 69 6e 23`

**Stop PC Operation**

`24 4a 49 47 30 32 13 00 00 00 00 01 73 61 63 68 69 6e 23`

**GET RTC Parmeters**

`24 4a 49 47 30 32 13 00 00 00 00 35 73 61 63 68 69 6e 23`