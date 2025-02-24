## Meter Command Types

1.Authentication Commands
2.Calibration Commands
3.Memory Format Commands
4.Siglog Commands
5.Ocmem Access Commands
6.Platform Access Commands
7.Unlock_access Commands

# Meter Communication

## Hardware Test Command Format

## Memory format command Format

## Invoke Action Commands Format

`Command Format`

| Header            |   Pkt length      |   Meter CMD code  |  CMD MEM Type      |     Area   |      CRC   | Frame End     |
| -------------------- |--- ----------------- | --------------------------|----------------------------| -------------|--------------|------------------- |
| 02 01 (2bytes) |      1-Byte        |  1-Byte                   |     1-Byte                |     1Byte  |   2Byte   | '03' (1Byte)    |

 example-
 clear 1 CMD = 02 01 06 03 02 FF 5C 05 03
