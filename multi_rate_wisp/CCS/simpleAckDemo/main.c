/**
 * @file       usr.c
 * @brief      WISP application-specific code set
 * @details    The WISP application developer's implementation goes here.
 *
 * @author     Aaron Parks, UW Sensor Systems Lab
 *
 */

#include "wisp-base.h"

WISP_dataStructInterface_t wispData;

/** 
 * This function is called by WISP FW after a successful ACK reply
 *
 */
void my_ackCallback (void) {
  asm(" NOP");
}

/**
 * This function is called by WISP FW after a successful read command
 *  reception
 *
 */
void my_readCallback (void) {
  asm(" NOP");
}

/**
 * This function is called by WISP FW after a successful write command
 *  reception
 *
 */
void my_writeCallback (void) {
  asm(" NOP");
}

/** 
 * This function is called by WISP FW after a successful BlockWrite
 *  command decode

 */
void my_blockWriteCallback  (void) {
  asm(" NOP");
}


/**
 * This implements the user application and should never return
 *
 * Must call WISP_init() in the first line of main()
 * Must call WISP_doRFID() at some point to start interacting with a reader
 */
void main(void) {

  WISP_init();
  
  // Register callback functions with WISP comm routines
  WISP_registerCallback_ACK(&my_ackCallback);
  WISP_registerCallback_READ(&my_readCallback);
  WISP_registerCallback_WRITE(&my_writeCallback);
  WISP_registerCallback_BLOCKWRITE(&my_blockWriteCallback);
  
  // Get access to EPC, READ, and WRITE data buffers
  WISP_getDataBuffers(&wispData);
  
  // Set up operating parameters for WISP comm routines
  WISP_setMode( MODE_READ | MODE_WRITE | MODE_USES_SEL); 
  WISP_setAbortConditions(CMD_ID_READ | CMD_ID_WRITE /*| CMD_ID_ACK*/);
  
  // Set up EPC
  wispData.epcBuf[0] = 0x00; // WISP version
  wispData.epcBuf[1] = *((uint8_t*)INFO_WISP_TAGID+1); // WISP ID MSB
  wispData.epcBuf[2] = *((uint8_t*)INFO_WISP_TAGID); // WISP ID LSB
  wispData.epcBuf[3] = 0x00;
  wispData.epcBuf[4] = 0x00;
  wispData.epcBuf[5] = 0x00;
  wispData.epcBuf[6] = 0x00;
  wispData.epcBuf[7] = 0x00;
  wispData.epcBuf[8] = 0x88;
  wispData.epcBuf[9] = 0x99;
  wispData.epcBuf[10]= 0xAA;
  wispData.epcBuf[11]= 0xBB;
  
  // Talk to the RFID reader.
  while (FOREVER) {
    WISP_doRFID();
  }
}
