/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.4: Testbench Header
 */

#ifndef LAB1_TB_SPACE_LINK_H
#define LAB1_TB_SPACE_LINK_H

#include <Arduino.h>
#include <RadioLib.h>

typedef int (*InitRadio_Ptr)(SX1278 &);
typedef int (*TransmitKISS_Ptr)(SX1278 &, const uint8_t *, size_t);

void runSpaceLinkTestbench(SX1278 &radioObj, InitRadio_Ptr initFunc, TransmitKISS_Ptr txFunc);

#endif // LAB1_TB_SPACE_LINK_H