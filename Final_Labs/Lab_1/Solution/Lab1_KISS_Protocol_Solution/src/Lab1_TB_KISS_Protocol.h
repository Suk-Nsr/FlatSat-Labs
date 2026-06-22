/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.3: Testbench Header
 */

#ifndef LAB1_TB_KISS_PROTOCOL_H
#define LAB1_TB_KISS_PROTOCOL_H

#include <Arduino.h>

typedef size_t (*KISS_Encoder_Ptr)(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer);

void runKISSTestbench(KISS_Encoder_Ptr studentFunc);

#endif // LAB1_TB_KISS_PROTOCOL_H