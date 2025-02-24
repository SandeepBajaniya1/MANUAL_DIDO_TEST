#ifndef __PC_COMM_H__
#define __PC_COMM_H__

#include "r_cg_macrodriver.h"
#include "SerialData.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "rtc_prod.h"
#include "jig_operations.h"
#include "error_codes.h"
#include "r_main.h"


/*--------MACROS-----------*/
//* Define the type of Meter to be tested *//
//#define WCM_3P
//#define LTCT_3P
//#define HTCT_3P

#ifdef WCM_3P
    #define JIG_FIRM_VERSION "WCM_2.0.0"
#elif defined(LTCT_3P)
    #define JIG_FIRM_VERSION "LTCT_2.0.0"
#elif defined(HTCT_3P)
    #define JIG_FIRM_VERSION "HTCT_2.0.0"
#endif

#ifdef WCM_3P
    #define PC_COMM_HEADER "$3PHWCM"
#elif defined(LTCT_3P)
    #define PC_COMM_HEADER "$3PLTCT"
#elif defined(HTCT_3P)
    #define PC_COMM_HEADER "$3PHTCT"  
#endif

#define PC_SEND_TIMEOUT 20      // for 2 sec timeout
#define PC_HEADER_LEN 7
#define PC_FRAME_LEN_LEN 1
#define PC_COMMAND_LOC 8
#define PC_COMMAND_LEN  1
#define PC_DATA_LOC PC_HEADER_LEN+PC_FRAME_LEN_LEN+PC_COMMAND_LEN
#define PC_DATA_MAX_LEN 100
#define PC_HEADER_END '#'
#define OVERHEAD PC_DATA_LOC+1 // including frame Header_end length

// ENUMS


// Data structures
typedef struct 
{
    /**
     * This contains the different parameters information received in the received command
     */
    BYTE rxCommand;
    uint8_t rxDataLen;
    char rxData[PC_DATA_MAX_LEN];
} t_rxFrameInfo;

typedef struct 
{
    /**
     * This contains the different parameters information received in the received command
     */
    BYTE rxCommand;
    BYTE errorCode;
    char txData[PC_DATA_MAX_LEN];
    uint8_t txDataLen;
} t_txFrameInfo;

// Global variables
extern int g_enableDebug;

// User Defined functions
void pc_comm_startPcOperation(void);
e_jigOperationCommands pc_comm_waitForPcCommand(t_rxFrameInfo*);
e_jigOperationCommands pc_comm_getPcCommand(e_commPorts, t_rxFrameInfo*);
e_jigErrorCodes pc_comm_sendPcResponse(e_commPorts pcPort, t_txFrameInfo *txPcFrame);
void pc_comm_sendDataToPC(char *str);
void pc_comm_createResponseFrame(char *data, uint8_t dataLen, e_jigErrorCodes, t_txFrameInfo *txPcFrame);

e_jigErrorCodes jig_operations_executeCommand(t_rxFrameInfo *rxPcFrame, t_txFrameInfo *txPcFrame);

void sendPCData(uint8_t bySPCDTag);

#endif