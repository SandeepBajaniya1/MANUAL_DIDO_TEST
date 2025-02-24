# PC Communication

## **Command Structure**

### **From PC to JIG Controller**

| Header      | Frame Length | Packet ID/Command ID | Data     | Frame End     |
| ----------- | ------------ | -------------------- | -------- | ------------- |
| `$3PHWCM` | 1 Byte       | 1 Byte               | Optional | `#`(1 Byte) |

 **Example** :
`24 33 50 48 57 43 4D 0A 01 23`

---

### **From JIG Controller to PC (Response)**

| Header      | Frame Length | Packet ID/Command ID | Error Code | Data     | Frame End     |
| ----------- | ------------ | -------------------- | ---------- | -------- | ------------- |
| `$3PHWCM` | 1 Byte       | 1 Byte               | 1 Byte     | Optional | `#`(1 Byte) |

 **Example** :
`24 33 50 48 57 43 4D 0B 01 64 23`

---

## **Commands and Responses**

#### 1. **IS_JIG_READY**

* **Command** :
  **`24 33 50 48 57 43 4D 0A 01 23`**
* **Response** :
  `24 33 50 48 57 43 4D 0B 01 64 23 `

---

#### 2. **GET_SWITCH_STATUS**

* **Command** :
  `24 33 50 48 57 43 4D 0A 02 23`
* **Response** :
  `24 33 50 48 57 43 4D 0D 02 64 84 FD 23`

---

#### 3. **GET_JIG_FIRM_VER**

* **Command** :
  `24 33 50 48 57 43 4D 0A 03 23`
* **Response** :
  `24 33 50 48 57 43 4D 15 03 64 4C 54 43 54 5F 31 2E 30 2E 30 23`

---

#### 4. **CHECK_METER_COMM**

* **Command** :
  `24 33 50 48 57 43 4D 0A 04 23`
* **Response** :
  `24 33 50 48 57 43 4D 0B 04 64 23`

---

#### 5. **GET_METER_PCB_BAT_VTG**

* **Command** :
  `24 33 50 48 57 43 4D 0A 20 23`
* **Response** :
  `24 33 50 48 57 43 4D 13 20 64 80 F6 35 40 80 32 68 40 23`
* **Data** :

  * 1st 4 bytes = RTC battery voltage (float).
  * 2nd 4 bytes = Main battery voltage (float).

  ---

#### 6. **GET_MAIN_BAT_SLEEP_CURRENT**

* **Command** :
  `24 33 50 48 57 43 4D 0A 21 23`
* **Response** :
  `24 33 50 48 57 43 4D 0F 21 64 00 00 48 3C 00 00 00 00 23`
* **Data** :

  * 1st 4 bytes = Main battery current with resistance (float).
  * 2nd 4 bytes = Main battery current (float).
  * **2nd-1st byte=   -0.1 to 0.1

  ---

#### 7. **GET_METER_PCB_MAINS_DC_VTG**

* **Command** :
  `24 33 50 48 57 43 4D 0A 22 23`
* **Response** :
  `24 33 50 48 57 43 4D 13 22 64 00 E0 4E 40 80 C1 56 40 23`
* **Data** :

  * 1st 4 bytes = DVCC voltage (float).
  * 2nd 4 bytes = 6.5V DC voltage (float).

  ---

#### 8. **GET_METER_INT_FIRM_VERSION**

* **Command** :
  `24 33 50 48 57 43 4D 0A 10 23`
* **Response** :
  `24 33 50 48 57 43 4D 16 10 64 49 50 4B 37 30 2E 30 31 2E 31 34 23`

#### 9. **GET_METER_BOARD_NUMBER**

* **Command** :
  `24 33 50 48 57 43 4D 0A 11 23`
* **Response** :
  `24 33 50 48 57 43 4D 13 11 64 15 CD 5B 07 00 00 00 00 23`

#### 10. **START_RTC_CALIB**

* **Command** :
  `24 33 50 48 57 43 4D 0A 52 23`
* **Response** :
  `24 33 50 48 57 43 4D 13 52 B0 3B DD 64 7D 64 7D 99 40 23`

#### **11. GET_STATUS_METER_I2C_CHECK**

* **Command** :
  `24 33 50 48 57 43 4D 0A 30 23`
* **Response** :
  `24 33 50 48 57 43 4D 0B 30 64 23`

#### **12. SET_METER_DATE_TIME**

* **Command** :
  `24 33 50 48 57 43 4D 0A 31 23`
* **Response** : *(Handled based on specific implementation, not provided)*

#### **13. GET_STATUS_METER_CLEAR1**

* **Command** :
  `24 33 50 48 57 43 4D 0A 32 23`
* **Response** :
  `24 33 50 48 57 43 4D 0B 32 64 23`---

#### **14. GET_STATUS_METER_RESET**

* **Command** :
  `24 33 50 48 57 43 4D 0A 33 23`
* **Response** :
  `24 33 50 48 57 43 4D 0F 33 64 55 15 0E 40 23`
* **Data** :

  * 4 bytes = Reset voltage(flaot).

  ---

#### **15. GET_STATUS_RELAY_TEST**

* **Command** :
  `24 33 50 48 57 43 4D 0A 34 23 `
* **Response** :
  `24 33 50 48 57 43 4D 0B 34 64 23`

#### 16. **GET_STATUS_VTG_AND_CURRENT**

* **Command** :
  `24 33 50 48 57 43 4D 0A 35 23`
* **Response** :
  Response: `24 33 50 48 57 43 4D 27 35 64 9A D7 68 43 E2 E8 68 43 02 3C 69 43 E1 0B 77 41 DA AC 76 41 47 72 76 41 3D 2C 77 41 23`
* **Data** :

  * 1st 4 bytes =  R Phase voltage (float).
  * 2nd 4 bytes =  Y Phase voltage (float).
  * 3rd 4 bytes =  B Phase voltage (float).
  * 4th 4 bytes =  R Phase current (float).
  * 5th 4 bytes =  Y Phase current (float).
  * 6th 4 bytes =  B Phase current (float).
  * 7th 4 bytes =  Neutral current (float).

  ---

#### **17. GET_STATUS_KWH**

* **Command** :
  `24 33 50 48 57 43 4D 0A 36 23`
* **Response** :
  `24 33 50 48 57 43 4D 0B 36 64 23`

#### **18. GET_STATUS_RF**

* **Command** :
  `24 33 50 48 57 43 4D 0A 38 23`
* **Response** :
  `24 33 50 48 57 43 4D 0B 38 64 23`

#### **19. GET_STATUS_METER_SWITCHES**

* **Command** :
  `24 33 50 48 57 43 4D 0A 39 23`
* **Response** :
  `24 33 50 48 57 43 4D 0C 39 64 00 23`

#### 20. **GET_METER_PCB_SUPERCAP_VTG**

* **Command** :
  `24 33 50 48 57 43 4D 0A 23 23`
* **Response** :
  `24 33 50 48 57 43 4D 0F 23 64 80 0D 4C 40 23`

  * **Data** :

    * 4 bytes = SuperCap voltage (float).

  ---

#### **21. GET_RTC_PARAMETERS**

* **Command** :
  `24 33 50 48 57 43 4D 0A 41 23`
* **Response** :
  `24 33 50 48 57 43 4D 23 41 64 3B DD 68 48 33 33 13 42 68 48 A7 40 40 F0 E4 41 86 7E 11 42 CD CC 4C BE 23`

  * **Data** :

    * 1st 4 bytes =  PPM offset(float).
    * 2nd 4 bytes = Operational PPM offset (float).
    * 3rd 4 bytes =  Temperature offset (float).
    * 4th 4bytes =   Current Temperature (float).
    * 5th 4bytes =   Current PPM (float).
    * 6th 4bytes =   Average PPM (float).

  ---

#### **22. SET_STATUS_ALL_PASS_LED (Pass) / (Fail)**

* **Command** :

  * Pass:  `24 33 50 48 57 43 4D 0B 43 01 23`
  * Fail:    `24 33 50 48 57 43 4D 0B 43 00 23`
* **Response** :

  * Pass: `24 33 50 48 57 43 4D 0C 43 64 01 23`
  * Fail: ` 24 33 50 48 57 43 4D 0C 43 64 00 23`

  ---
