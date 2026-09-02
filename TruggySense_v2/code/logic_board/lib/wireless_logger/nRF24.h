/**
 * @file nRF24.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief nRF24L01 wireless communication module for long-range data transmission.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef NRF24_H_
#define NRF24_H_

#pragma once

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "module.h"
#include "structs.h"

#define SPI_CLOCK_SPEED 4000000                 /** @brief The default SPI clock speed of the Arduino UNO. */
#define MAXIMUM_TRYOUT  10                      /** @brief The maximum allowable tryouts if a failure occurs. */
#define PAYLOAD_LENGTH  sizeof(logging_data_t)  /** @brief The length of the used payload */
#define CHUNK_COUNT     8                       /** @brief The amount of chunks that will be sended over. */
/**
 * @brief the byte size of one chunk.
 * @note The +1 addition is due to the sequence number. 
 */
#define CHUNK_SIZE 30 + 1

class nRF24 : public Module {
    private:
        RF24 _radio;              /** @brief The RF24 radio object for handling nRF24L01 communication. */
        byte _address[6];         /** @brief The selected address name. Must be in sync (the same) with the receivers end. */
        uint8_t tryouts;          /** @brief A tryout counter to enable some packets losses but not infinitely long. Uses MAXIMUM_TRYOUT as top value. */
        uint8_t is_initialized;   /** @brief A flag to state the first start. */
        logging_data_t _payload;  /** @brief A local copy of the payload that needs to be send. */

        /**
         * @brief Method that converts a payload defined in a byte array, to a 2D array.
         *
         * The row represents a chunk where the columns represent the content inside that chunk.
         *
         * @param payload payload defined in a byte array.
         * @param chunked_payload a 2D array which is used to store the content of payload in a chunked manner.
         */
        void toChunk(uint8_t *payload, uint8_t chunked_payload[CHUNK_COUNT][CHUNK_SIZE]);

    public:
        /** 
         * @brief Constructor for the nRF24 class.
         * @param ce_pin The CE pin for the nRF24 module.
         * @param csn_pin The CSN pin for the nRF24 module.
         * @param address The address for the nRF24 communication. Must be in sync (the same) with the receiver's end.
         */
        nRF24(const uint8_t ce_pin, const uint8_t csn_pin, const byte* address = (const byte*)"00001")
        : Module("nRF24L01 - Long range communication/wireless logger"),
          _radio(RF24(ce_pin, csn_pin, SPI_CLOCK_SPEED)) {
            memcpy(_address, address, 6);
          }

        /** 
         * @brief Initializes the nRF24 module.
         * @return true if initialization is successful, false otherwise.
         */
        bool init() override;

        /** 
         * @brief Sends the logging data over the wireless connection.
         * @param log_data The logging data to send.
         * @return The status of the send operation (e.g., number of attempts or error code).
         */
        uint8_t sendData(const logging_data_t &log_data);
};

#endif