/**
 * @file ibus.h
 * @author Robbe Elsermans
 * @brief Driver for the FS-X6B receiver which utilized IBUS protocol. Can be used for any IBUS enabled receiver.
 * @version 2
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */

#pragma once

#include "Arduino.h"

#define CH1_DEFAULT_VAL 1500  /** < @brief default value in case of failsafe to send out for channel 1 */
#define CH2_DEFAULT_VAL 1500  /** < @brief default value in case of failsafe to send out for channel 2 */
#define CH3_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 3 */
#define CH4_DEFAULT_VAL 1500  /** < @brief default value in case of failsafe to send out for channel 4 */
#define CH5_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 5 */
#define CH6_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 6 */
#define CH7_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 7 */
#define CH8_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 8 */
#define CH9_DEFAULT_VAL 1000  /** < @brief default value in case of failsafe to send out for channel 9 */
#define CH10_DEFAULT_VAL 1000 /** < @brief default value in case of failsafe to send out for channel 10 */
#define CH11_DEFAULT_VAL 1000 /** < @brief default value in case of failsafe to send out for channel 11 */
#define CH12_DEFAULT_VAL 1000 /** < @brief default value in case of failsafe to send out for channel 12 */
#define CH13_DEFAULT_VAL 1000 /** < @brief default value in case of failsafe to send out for channel 13 */
#define CH14_DEFAULT_VAL 1000 /** < @brief default value in case of failsafe to send out for channel 14 */

class IBUS
{
private:
    /**
     * @brief States of the IBUS driver data retrieval
     *
     */
    enum decoder_states
    {
        DETECT,   //**< @brief detect the start of a frame defined by START_FRAME_0 and START_FRAME_1 */
        DATA,     //**< @brief save the sequentially bytes that present data and CRC-16 */
        VALIDATE, //**< @brief check saved data based on received CDC-16 and calculated one */
        DECODE,   //**< @brief decode the received bytes to the internal channel data  */
        DISCARD   //**< @brief a defaut state where we go to if something was off. */
    };
    static const uint8_t PACKET_LENGTH = 32;                  /**< the total size of an IBUS packet including header, payload and CRC-16 */
    static const uint8_t DATA_LENGTH = PACKET_LENGTH - 2;     /**< size of the data including payload and CRC-16 */
    static const uint8_t CHANNEL_COUNT = DATA_LENGTH / 2 - 1; /**< The amount of channels of 2 byte */
    static const uint8_t START_FRAME_0 = 0x20;                /**< @brief First byte of frame  (frame length 2^5=32)*/
    static const uint8_t START_FRAME_1 = 0x40;                /**< @brief Second byte of frame (length) */
    static const uint8_t DEAD_TIME = 100;                     /**< @brief timer to state that the connection is dead -> activates failsafe*/
    static const uint8_t INTER_FRAME_STOP = 4;                /**< @brief The minimal time a frame can be checked. IBUS sends each 7ms a frame.*/
    decoder_states state;

    uint16_t crc;
    uint16_t crc_calculated;

    uint8_t data_counter;
    uint8_t raw_data[DATA_LENGTH];

    uint16_t data[CHANNEL_COUNT];

    uint8_t failsafe;

    long dead_timer;
    long read_timer;

    uint16_t channel_defaults[14] = {CH1_DEFAULT_VAL,
                                     CH2_DEFAULT_VAL,
                                     CH3_DEFAULT_VAL,
                                     CH4_DEFAULT_VAL,
                                     CH5_DEFAULT_VAL,
                                     CH6_DEFAULT_VAL,
                                     CH7_DEFAULT_VAL,
                                     CH8_DEFAULT_VAL,
                                     CH9_DEFAULT_VAL,
                                     CH10_DEFAULT_VAL,
                                     CH11_DEFAULT_VAL,
                                     CH12_DEFAULT_VAL,
                                     CH13_DEFAULT_VAL,
                                     CH14_DEFAULT_VAL};

    void setDefaultChannelValue();

public:
    /**
     * @brief Defining the different channel sources that are available in IBUS
     *
     * @note depending on the used transmitter, not all sources can be used
     *
     */
    enum channel_source
    {
        ch_1,
        ch_2,
        ch_3,
        ch_4,
        ch_5,
        ch_6,
        ch_7,
        ch_8,
        ch_9,
        ch_10,
        ch_11,
        ch_12,
        ch_13,
        ch_14,
    };

    /**
     * @brief Construct a new IBUS object with HardwareSerial8 object
     *
     */
    uint8_t init();

    /**
     * @brief search for receiving data based on the START_FRAME_SEQUENCE
     *
     * @note if this method is addressed faster than INTER_FRAME_STOP, it will do nothing but returning 2.
     *
     * @return uint8_t
     * <ul>
     * <li>0, no connection.</li>
     * <li>1, connection and communication.</li>
     * <li> 2, requested to fast after one another. See: INTER_FRAME_STOP</li>
     * </ul>
     */
    uint8_t read();

    /**
     * @brief Get the Channel value based on provided channel source
     *
     * @param ch the source
     * @return uint32_t the channel value ranging from 1000 to 2000
     */
    uint16_t getChannel(channel_source channel_number);

    /**
     * @brief Defines if a connection is alive meaning if a transmitter is connected to the receiver
     *
     * @return uint8_t 0 if connected and 1 if not connected
     */
    uint8_t isFailsafe();
};
