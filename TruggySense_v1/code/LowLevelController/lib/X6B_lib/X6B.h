/**
 * @file X6B.h
 * @author Robbe Elsermans
 * @brief containing a reversed engineerd IBUS protocol library
 * @version 1.0
 * @date 2025-04-10

 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */

#ifndef _X6B_H_
#define _X6B_H_

#include "Arduino.h"

#define IBUS_SERIAL Serial8

#define START_FRAME_SEQUENCE 0x2040
#define FRAME_LENGTH 16
#define INTER_FRAME_STOP 4 // 7ms of time gap between messages. Checking each 4ms is enough. Faster has no use.
#define DEAD_TIME 20
#define NO_CRC_FRAME_LENGTH ((FRAME_LENGTH - 1) * 2)

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

#define CH_LOW_MIN 950
#define CH_HIGH_MAX 2050


class IBUS
{
private:
    uint16_t channel[14] = {0}; // Minus the checksum and header

    HardwareSerial *s;

    bool pre_start_frame = false;
    bool start_frame_detected = true;
    bool is_decoding = false;
    bool alive = false;
    // bool deadlock = false;
    long trigger_time;

    uint8_t buffer[NO_CRC_FRAME_LENGTH];
    uint8_t buffer_index = 0;

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

    long timer;
    long deadTimer; // When no connection is present
    uint16_t crc_calculated;

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
    IBUS();

    /**
     * @brief Construct a new IBUS object with another HardwareSerial object
     * 
     * @param serial the HardwareSerial object
     */
    IBUS(HardwareSerial &serial);

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
    uint8_t readData();

    /**
     * @brief Get the Channel value based on provided channel source
     * 
     * @param ch the source
     * @return uint32_t the channel value ranging from 1000 to 2000
     */
    uint32_t getChannel(channel_source ch);

    /**
     * @brief Defines if a connection is alive meaning if a transmitter is connected to the receiver
     * 
     * @return uint8_t 1 if connected and 0 if not connected
     */
    uint8_t isAlive();

    /**
     * @brief Set the Channel Default Value object
     * 
     * @param ch 
     * @param milliseconds 
     * @return uint8_t 
     */
    uint8_t setChannelDefaultValue(channel_source ch, uint16_t milliseconds);
};

#endif