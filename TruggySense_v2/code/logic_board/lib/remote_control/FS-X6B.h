/**
 * @file FS-X6B.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Driver for the FS-X6B receiver which utilized the IBUS protocol. 
 * Can be used for any IBUS enabled receiver.
 * @version 2.0
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */

#ifndef FS_X6B_H_
#define FS_X6B_H_

#pragma once

#include "module.h"

#define PACKET_LENGTH    32                  /** @brief The total size of an IBUS packet including header, payload and CRC-16. */
#define DATA_LENGTH      PACKET_LENGTH - 2   /** @brief Size of the data including payload and CRC-16. */
#define CHANNEL_COUNT    10                  /** @brief The amount of channels of 2 byte. */
#define START_FRAME_0    0x20                /** @brief First byte of frame  (frame length 2^5=32). */
#define START_FRAME_1    0x40                /** @brief Second byte of frame (length). */
#define DEAD_TIME        100                 /** @brief timer to state that the connection is dead -> activates failsafe. */
#define INTER_FRAME_STOP 4                   /** @brief The minimal time a frame can be checked. IBUS sends each 7ms a frame. */

/** @brief States of the FS-X6B driver data retrieval. */
enum class DECODER_STATES {
    DETECT,   /** @brief detect the start of a frame defined by START_FRAME_0 and START_FRAME_1. */
    DATA,     /** @brief save the sequentially bytes that present data and CRC-16. */
    VALIDATE, /** @brief check saved data based on received CDC-16 and calculated one. */
    DECODE,   /** @brief decode the received bytes to the internal channel data.  */
    DISCARD   /** @brief a defaut state where we go to if something was off. */
};

/**
 * @brief The different channel sources that are available in FS-X6B.
 * @note Depending on the used transmitter, not all sources can be used.
 */
enum class CHANNEL_SOURCE : uint8_t {
    CH_1,   /** @brief channel for right joystick front-to-back movement. */
    CH_2,   /** @brief channel for right joystick left-to-right movement. */
    CH_3,   /** @brief channel for left joystick front-to-back movement. */
    CH_4,   /** @brief channel for left joystick left-to-right movement. */
    CH_5,   /** @brief channel for outer left switch (SWA). */
    CH_6,   /** @brief channel for inner left switch (SWB). */
    CH_7,   /** @brief channel for inner right switch (SWC). */
    CH_8,   /** @brief channel for outer right switch (SWD). */
    CH_9,   /** @brief channel for left knob (VAA). */
    CH_10,  /** @brief channel for right knob (VAB). */
};

/** @brief Default values for each channel. */
enum class CHANNEL_DEFAULT_VALUES : uint16_t {
    CH1_DEFAULT_VAL =  1500, /** @brief default value in case of failsafe to send out for channel 1. */
    CH2_DEFAULT_VAL =  1500, /** @brief default value in case of failsafe to send out for channel 2. */
    CH3_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 3. */
    CH4_DEFAULT_VAL =  1500, /** @brief default value in case of failsafe to send out for channel 4. */
    CH5_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 5. */
    CH6_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 6. */
    CH7_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 7. */
    CH8_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 8. */
    CH9_DEFAULT_VAL =  1000, /** @brief default value in case of failsafe to send out for channel 9. */
    CH10_DEFAULT_VAL = 1000, /** @brief default value in case of failsafe to send out for channel 10. */
};

class FS_X6B: public Module {
private:
        HardwareSerial *_serial; /** @brief Serial interface used to receive IBUS frames. */

        DECODER_STATES _state; /** @brief Current decoder state machine state. */
        /** @brief Default values for each channel. */
        const CHANNEL_DEFAULT_VALUES _channel_defaults[CHANNEL_COUNT] = {
            CHANNEL_DEFAULT_VALUES::CH1_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH2_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH3_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH4_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH5_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH6_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH7_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH8_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH9_DEFAULT_VAL,
            CHANNEL_DEFAULT_VALUES::CH10_DEFAULT_VAL,
        };

        uint16_t _crc; /** @brief CRC extracted from the received IBUS frame. */
        uint16_t _crc_calculated; /** @brief CRC calculated over the received payload bytes. */

        uint8_t _data_counter; /** @brief Number of payload bytes already collected for the current packet. */

        uint8_t _raw_data[DATA_LENGTH]; /** @brief Raw packet contents including payload and CRC. */
        uint16_t _data[CHANNEL_COUNT]; /** @brief Decoded channel output values. */

        bool _failsafe; /** @brief Flag indicating the receiver is currently in failsafe mode. */

        long _dead_timer; /** @brief Timestamp of the last valid frame reception. */
        long _read_timer; /** @brief Timestamp used to enforce minimum inter-frame delay. */

        void _setDefaultChannelValue(); /** @brief Reset all channel outputs to their configured failsafe values. */

    public:
        /**
         * @brief Constructor for the FS_X6B class.
         * @param serial The serial interface.
        */
        FS_X6B(HardwareSerial &serial)
        : Module("FS-X6B - Remote control communication module"), 
          _serial(&serial),
          _state(DECODER_STATES::DETECT),   // Start to detect data
          _crc(0),
          _crc_calculated(0xFFFF),
          _data_counter(0),
          _failsafe(true),
          _dead_timer(0) {}

        /**
         * @brief Initialize the FS-X6B module.
         * @return true if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Search for receiving data based on the START_FRAME_SEQUENCE.
         * @return A status code.
         * - 0: No valid frame received, connection not established.
         * - 1: Connection established and data successfully received.
         * - 2: Read request occurred too quickly after the previous call; no new frame processed.
         * @note If this method is called faster than INTER_FRAME_STOP, it returns 2 and does not process data.
         */
        uint8_t read();

        /**
         * @brief Get the channel value based on provided channel source.
         * @param channel_number The source channel.
         * @return The channel value ranging between 1000 and 2000.
         */
        uint16_t getChannel(const CHANNEL_SOURCE channel_number);

        /**
         * @brief Determine whether the receiver is in failsafe mode.
         * @return true when the receiver is in failsafe mode, false when valid channel data is available.
         */
        bool isFailsafe();
};

#endif