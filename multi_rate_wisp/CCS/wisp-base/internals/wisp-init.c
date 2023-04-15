/**
 * @file wisp-init.c
 *
 * Provides the initialization routines for the WISP.
 *
 * @author Aaron Parks, Justin Reina
 */

#include "../globals.h"

// Gen2 state variables
RFIDstruct  rfid;   // inventory state
RWstruct    RWData; // tag-access state

// Buffers for Gen2 protocol data
uint8_t cmd[CMDBUFF_SIZE];      // command from reader
uint8_t dataBuf[DATABUFF_SIZE]; // tag's response to reader
uint8_t rfidBuf[RFIDBUFF_SIZE]; // internal buffer used by RFID handles

/*
 * Globals
 */

volatile uint8_t isDoingLowPwrSleep;


/** @fcn        void WISP_init (void)
 *  @brief      Called by client code as a first step after PUC
 *
 *  @section    Purpose
 *
 *  @post       the RAM, clock and peripherals are initialized for use
 *
 *  @section    Timing
 *      -# The routine is entered X cycles into the CPU's operation.
 *      -# The routine tries to sleep Y cycles into the CPU's operation.
 *      -# From a cold-boot, the CPU is configured for fCPU = Z MHz. (i.e. ...?)
 *
 *  @section    Considerations
 *      -# If preforming a 'sleep_till_full_power' operation,
 */
void WISP_init(void) {

    //----------------------------------------Setup Watchdog, IO--------------------------------------------------------------------//
    WDTCTL = WDTPW + WDTHOLD;                                               /* disable the watchdog timer                           */

    setupDflt_IO();

    // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings.
    PM5CTL0 &= ~LOCKLPM5;

//  PHOLDOUT |= PIN_HOLD;                                                   /* Hold the LDO ON                                      */
    PRXEOUT |= PIN_RX_EN; /** TODO: enable PIN_RX_EN only when needed in the future */

    // TODO Try running the system as fast as possible here for power savings
    CSCTL0_H = 0xA5;
    CSCTL1 = DCOFSEL0 + DCOFSEL1; //4MHz
    CSCTL2 = SELA_0 + SELS_3 + SELM_3;
    CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;

    // Initialize Gen2 standard memory banks
    RWData.EPCBankPtr = &dataBuf[0];                    // volatile
    RWData.RESBankPtr = (uint8_t*) MEM_MAP_INFOC_START; // nonvolatile
    RWData.TIDBankPtr = (uint8_t*) MEM_MAP_INFOB_START; // nonvolatile
    RWData.USRBankPtr = (uint8_t*) &usrBank[0];         // volatile

    // Initialize rfid transaction mode
    rfid.isSelected = TRUE;
    rfid.abortOn    = 0x00;

    isDoingLowPwrSleep = FALSE;

    // Initialize callbacks to null in case user doesn't configure them
    RWData.akHook =0;
    RWData.rdHook= 0;
    RWData.wrHook =0;
    RWData.bwrHook=0;

    return;
}

