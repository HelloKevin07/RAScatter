/**
 * @file       main.c
 * @brief      WISP application-specific code set
 * @details    The WISP application developer's implementation goes here.
 *
 * @author     Aaron Parks, UW Sensor Systems Lab
 * @author     Ivar in 't Veen, TU Delft Embedded Software Group
 *
 */

#include "wisp-base.h"
#define UseTemperatureSensor
WISP_dataStructInterface_t wispData;

/** 
 * This function is called by WISP FW after a successful ACK reply
 *
 */
void my_ackCallback(void) {
    asm(" NOP");
    #ifndef UseTemperatureSensor
      BITSET(PMEAS_ENOUT, PIN_MEAS_EN);
      uint16_t adc_value = ADC_read();
      //int16_t adc_voltage = ADC_rawToVoltage(adc_value);
      BITCLR(PMEAS_ENOUT, PIN_MEAS_EN);

      wispData.epcBuf[9] = (adc_value >> 8) & 0xFF;
      wispData.epcBuf[10] = (adc_value >> 0) & 0xFF;
    #else
      uint16_t adc_value = ADC_read();
      //int16_t adc_temperature = ADC_rawToTemperature(adc_value);

      wispData.epcBuf[9] = (adc_value >> 8) & 0xFF;
      wispData.epcBuf[10] = (adc_value >> 0) & 0xFF;
    #endif
}

/**
 * This function is called by WISP FW after a successful read command
 *  reception
 *
 */
void my_readCallback(void) {
    asm(" NOP");
}

/**
 * This function is called by WISP FW after a successful write command
 *  reception
 *
 */
void my_writeCallback(void) {
    asm(" NOP");
}

/** 
 * This function is called by WISP FW after a successful BlockWrite
 *  command decode

 */
void my_blockWriteCallback(void) {
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
    BITSET(PMEAS_ENDIR, PIN_MEAS_EN); // SET direction of MEAS_EN pin to output!

    // Get access to EPC, READ, and WRITE data buffers
    WISP_getDataBuffers(&wispData);

    // Set up operating parameters for WISP comm routines
    WISP_setMode(0 /* MODE_READ | MODE_WRITE | MODE_USES_SEL*/);
    WISP_setAbortConditions(CMD_ID_READ | CMD_ID_WRITE /*| CMD_ID_ACK */);

    // Set up EPC
    wispData.epcBuf[0] = 0x05; // WISP version
    wispData.epcBuf[1] = *((uint8_t*)INFO_WISP_TAGID+1); // WISP ID MSB: Pull from INFO seg
    wispData.epcBuf[2] = *((uint8_t*)INFO_WISP_TAGID); // WISP ID LSB: Pull from INFO seg
    wispData.epcBuf[3] = 0x03;
    wispData.epcBuf[4] = 0x04;
    wispData.epcBuf[5] = 0x05;
    wispData.epcBuf[6] = 0x06;
    wispData.epcBuf[7] = 0x07;
    wispData.epcBuf[8] = 0x08;
    wispData.epcBuf[9] = 0;           // HIGH ADC value field
    wispData.epcBuf[10] = 0;           // LOW ADC value field
    wispData.epcBuf[11] = 0;

//#define UseTemperatureSensor

#ifndef UseTemperatureSensor
    ADC_init();
    BITSET(PMEAS_ENDIR, PIN_MEAS_EN); // SET direction of MEAS_EN pin to output
#else
    ADC_initCustom(ADC_reference_2_0V, ADC_precision_10bit,
            ADC_input_temperature);
#endif

    // Talk to the RFID reader.
    while (FOREVER) {

#ifndef UseTemperatureSensor
        BITSET(PMEAS_ENOUT, PIN_MEAS_EN);
        uint16_t adc_value = ADC_read();
        int16_t adc_voltage = ADC_rawToVoltage(adc_value);
        BITCLR(PMEAS_ENOUT, PIN_MEAS_EN);

        wispData.epcBuf[9] = (adc_voltage >> 8) & 0xFF;
        wispData.epcBuf[10] = (adc_voltage >> 0) & 0xFF;
#else

        uint16_t adc_value = ADC_read();
        //int16_t adc_temperature = ADC_rawToTemperature(adc_value);

        wispData.epcBuf[9] = (adc_value >> 8) & 0xFF;
        wispData.epcBuf[10] = (adc_value >> 0) & 0xFF;

        //adc_value = ADC_read();
        //adc_temperature = ADC_rawToTemperature(adc_value);

        //wispData.epcBuf[6] = (adc_temperature >> 8) & 0xFF;
        //wispData.epcBuf[7] = (adc_temperature >> 0) & 0xFF;

        //adc_value = ADC_read();
        //adc_temperature = ADC_rawToTemperature(adc_value);

        //wispData.epcBuf[4] = (adc_temperature >> 8) & 0xFF;
        //wispData.epcBuf[5] = (adc_temperature >> 0) & 0xFF;

        //RWData.RESBankPtr[0] += 1;
        //wispData.epcBuf[11] = RWData.RESBankPtr[0];

#endif

        WISP_doRFID();
    }
}
