
# README: manual_jig_funcTest_Digital_IO

## Function Overview

The `manual_jig_funcTest_Digital_IO` function runs once when the device is powered on and retains the result until power off.

## Purpose

This function tests the Digital IO of a meter by activating different relays and verifying the meter's response.

## Process

1. Turns on relay power and introduces an initial delay.
2. Activates relays in sequence and checks meter responses.
3. Stores test results in `<span>byLedDisplayResult</span>`.
4. Resets all relay states after testing.
5. Returns `JIG_COMMNAD_OK` upon completion.

## Key Variables

* `testByte[4]`: Stores test results.
* `byLedDisplayResult`: Stores pass/fail status.
* `retry_count`: Allows up to three retries.

## Notes

* The function controls external relays (`RELAY_SHUNT`, `RELAY_CT`, etc.).
* Uses optical communication (`OPTICAL_PORT`).
* Includes delays to stabilize hardware before reading responses.
