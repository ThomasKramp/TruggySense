/**
 * @file base_station.h
 * @author Robbe Elsermans
 * @brief The base station class file which is used for sending telemetry data
 * 
 * Will cut the data into chunks of chunk_size. Is created to work for nrf24 boards that have a maximul payload size of 32 byte per transfer.
 * It uses the logging_data_t data structure originating from sd_lohher.h.
 * @version 1
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */

#pragma once

#include <stdint.h>
#include "sd_logger.h"

#define CE_PIN 10 /**< @brief Pin definition of Chip Enable to control data transmission (when to Rx and when to Tx) */
#define CSN_PIN 28 /**< @brief Pin definition of Chip Select Pin with negative logic of the SPI device*/

/**
 * @brief The selected path name. Must be in sync (the same) with the receivers end.
 *
 */
const byte path[6] = "00001";

/**
 * @brief The maximum allowable tryouts if a failure occures
 *
 */
#define MAXIMUM_TRYOUT 10

/**
 * @brief The length of the used payload defined in sd_logger.h logging_data_t
 *
 */
constexpr int payload_length = 228;

/**
 * @brief The amount of chunks that will be sended over.
 *
 */
constexpr int chunks_count = 10;

/**
 * @brief the byte size of one chunk.
 *
 * @note The +1 addition is due to the sequence number.
 *
 */
constexpr int chunk_size = 30 + 1;

class BaseStation
{
private:
    /**
     * @brief A tryout counter to enable some packets losses but not infinitely long. Uses MAXIMUM_TRYOUT as top value.
     *
     */
    uint8_t tryouts;

    /**
     * @brief A flag to state the first start.
     * 
     */
    uint8_t is_initialized;

    /**
     * @brief A local copy of the payload that needs to be send.
     *
     */
    logging_data_t payload;

    /**
     * @brief Method that converts a payload defined in a byte array, to a 2D array
     *
     * The row represents a chunk where the columns represent the content inside that chunk.
     *
     * @param payload payload defined in a byte array
     * @param chunked_payload a 2D array which is used to store the content of payload in a chunked manner.
     */
    void toChunk(uint8_t *payload, uint8_t chunked_payload[chunks_count][chunk_size]);

public:
    /**
     * @brief Initialize the radio
     * 
     * Will initialize the radio and will set the TX power to the maximum. 
     *
     * @return uint8_t 1 if initialization was a success and 0 if not.
     */
    uint8_t init();

    /**
     * @brief Send data to the basis station
     * 
     * The driver will try to restart the nrf24 by performing init() as long as tryouts is not 0.
     * If the tryouts reaches 0, nothing will be sended anymore and no init() is fired.
     *
     * @param data containing the logging data structure logging_data_t
     * @return uint8_t 1 if success otherwise 0.
     * 
     * @note the return of a 0 means that the tryouts has reached their end.
     */
    uint8_t sendData(logging_data_t data);
};