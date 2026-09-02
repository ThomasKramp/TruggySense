/**
Written by Robbe Elsermans.
Reversed engineerd IBUS protocol with a logic analyzer and a scope.

Used in my master project to control an RC vehicle with a Teensy as low-level-controller.
**/

#ifndef _IBUS_H_
#define _IBUS_H_

#include "Arduino.h"

#define START_FRAME_SEQUENCE 0x2040
#define FRAME_LENGTH 16
#define INTER_FRAME_STOP 4 //7ms of time gap between messages. Checking each 4ms is enough. Faster has no use.
#define DEAD_TIME 20

#define CH1_DEFAULT_VAL 1500
#define CH2_DEFAULT_VAL 1500
#define CH3_DEFAULT_VAL 1000
#define CH4_DEFAULT_VAL 1500
#define CH5_DEFAULT_VAL 1000
#define CH6_DEFAULT_VAL 1000
#define CH7_DEFAULT_VAL 1000
#define CH8_DEFAULT_VAL 1000
#define CH9_DEFAULT_VAL 1000
#define CH10_DEFAULT_VAL 1000
#define CH11_DEFAULT_VAL 1000
#define CH12_DEFAULT_VAL 1000
#define CH13_DEFAULT_VAL 1000
#define CH14_DEFAULT_VAL 1000

//extern uint32_t channel[FRAME_LENGTH-2]; //Minus the checksum and header

void initIBUS(HardwareSerial &serial);
uint8_t readData();
uint32_t getChannel(uint8_t index);
uint8_t isAlive();

#endif