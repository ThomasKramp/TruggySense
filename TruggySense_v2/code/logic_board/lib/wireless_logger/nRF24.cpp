/**
 * @file nRF24.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of nRF24.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "nRF24.h"

// #define PRINTDEBUG

bool nRF24::init() {
    if (!_radio.begin()) {
        setError(ModuleError::INIT_FAILED);
        return false;
    }
    delay(100);

    //debugSerial.println((char*)_address);
    _radio.openWritingPipe(_address);
    _radio.setPALevel(RF24_PA_MIN);
    _radio.setChannel(76);

    //_radio.setCRCLength(RF24_CRC_16);
    //_radio.setPayloadSize(64);
    //_radio.setAutoAck(false);

    if (!is_initialized) {
        tryouts = MAXIMUM_TRYOUT;
        is_initialized = 1;
    }

    #ifdef PRINTDEBUG
        // NR24.printDetails() is hardcoded to only use Serial.
        if (&debugSerial == &Serial) {
            debugSerial.println("=== Radio Config ===");
            _radio.printDetails();   // dumps all registers
            debugSerial.println("=== Sending ===");
        }
    #endif
    
    return true;
}

uint8_t nRF24::sendData(const logging_data_t &log_data) {
    if (getLastError() == ModuleError::INIT_FAILED) {
        #ifdef PRINTDEBUG
            debugSerial.println("\tNo module attached");
        #endif
        return 0;
    }
    
    _radio.flush_tx();
    delayMicroseconds(200);

    // Test connection:
    // char text[64] = "Ewa Broeder 2: Electric Boogaloo";
    // bool result = _radio.write(&text, sizeof(text));
    // if (result) setError(ModuleError::LOG_FAILED);
    // #ifdef PRINTDEBUG
    //     debugSerial.println("\tWrite:\t" + String(text));
    // #endif

    // only try to send something if tryouts is not depleted yet.
    if (tryouts > 0) {
        // make a copy
        memcpy(&_payload, &log_data, sizeof(log_data));

        uint8_t *str_buffer = reinterpret_cast<uint8_t *>(&_payload);
        uint8_t buffer[CHUNK_COUNT][CHUNK_SIZE] = {0};

        toChunk(str_buffer, buffer);
        int8_t error = 0;

        for (int i = 0; i < CHUNK_COUNT; i++) {            
            error += _radio.write(&buffer[i], sizeof(buffer[i]));

            #ifdef PRINTDEBUG
                if (i == 0) debugSerial.println();
                debugSerial.print("\tWrite chunk " + String(i) + ":\t");
                for (int j = 0; j < CHUNK_SIZE; j++) {
                    if (buffer[i][j] < 0x10) debugSerial.print("0");
                    debugSerial.print(buffer[i][j], HEX);
                    debugSerial.print(" ");
                }
                debugSerial.println();
            #endif
        }
        #ifdef PRINTDEBUG
            debugSerial.println("\tErrors:\t\t\t" + String(error));
        #endif

        // 20% dropout maximum allowed.
        if (error >= (uint8_t)CHUNK_COUNT * (0.8)) {
            // Increase tryouts if we had 1 failure between all good transmits.
            if (tryouts < MAXIMUM_TRYOUT)
                tryouts++;

            #ifdef PRINTDEBUG
                debugSerial.println("\tIncrease try-count:\t" + String(tryouts));
            #endif
        } else {
            // Decrease tryouts if failure happened that is 20% dropout of packets.
            if (tryouts > 0)
                tryouts--;

            #ifdef PRINTDEBUG
                debugSerial.println("\tDecrease try-count:\t" + String(tryouts));
            #endif
        }

        // The NRF is not responding at all. Try to restart it.
        if (error == 0) {
            init();
            #ifdef PRINTDEBUG
                debugSerial.println("\tRe-init");
            #endif
        }
        
    }
    return tryouts != 0;
}

void nRF24::toChunk(uint8_t *payload, uint8_t chunked_payload[CHUNK_COUNT][CHUNK_SIZE]) {
    uint16_t counter = 0;
    for (uint8_t i = 0; i < CHUNK_COUNT; i++) {
        for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
        if (j == 0) {
            chunked_payload[i][j] = (uint8_t)i;
        } else {
            if (counter < PAYLOAD_LENGTH) {
                chunked_payload[i][j] = *payload; // add real value
                payload++;
            } else
                chunked_payload[i][j] = 114; // padding
            counter++;
        }
        }
    }
}