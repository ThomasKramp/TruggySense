/**
 * @file FS-X6B.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of FS-X6B.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "FS-X6B.h"

// #define PRINTDEBUG

bool FS_X6B::init() {
    _serial->begin(115200, SERIAL_8N1);
    _setDefaultChannelValue();
    return true;
}

uint8_t FS_X6B::read() {
    if ((millis() - _read_timer) < INTER_FRAME_STOP) {
        #ifdef PRINTDEBUG
            debugSerial.println("To fast");
        #endif
        return 2;
    }
    _read_timer = millis();

    // #ifdef PRINTDEBUG
    //     debugSerial.println("Ready 2 read");
    // #endif
    while (_serial->available()) {
        uint8_t received_data = _serial->read();

        // #ifdef PRINTDEBUG
        //     debugSerial.println("Reading");
        // #endif
        switch (_state) {
        case DECODER_STATES::DETECT:
            if (received_data == START_FRAME_0) {
                _crc_calculated -= received_data;
                _data_counter++;
            } else if (_data_counter == 1 && received_data == START_FRAME_1) {
                _crc_calculated -= received_data;
                _data_counter++;
                _state = DECODER_STATES::DATA;
            } else {
                _data_counter = 0;
                _crc_calculated = 0xFFFF;
                _state = DECODER_STATES::DETECT;
            }
            break;

        case DECODER_STATES::DATA:
            _raw_data[_data_counter] = received_data;
            _crc_calculated -= _raw_data[_data_counter];
            _data_counter++;

            if (_data_counter == DATA_LENGTH) {
                _state = DECODER_STATES::VALIDATE;
            }
            break;

        case DECODER_STATES::VALIDATE:
            if (_data_counter == DATA_LENGTH) {
                _crc = received_data;
                _data_counter++;
            } else if (_data_counter == DATA_LENGTH + 1) {
                _crc |= (received_data << 8);

                if (_crc == _crc_calculated)
                    _state = DECODER_STATES::DECODE;
                else
                    _state = DECODER_STATES::DISCARD;
            }
            break;

        case DECODER_STATES::DECODE:
            for (size_t i = 2; i < DATA_LENGTH - 1; i += 2) {
                _data[(i / 2) - 1] = _raw_data[i] | _raw_data[i + 1] << 8;
                // #ifdef PRINTDEBUG
                //     debugSerial.printf("%d: %d ", i / 2, _raw_data[i] | _raw_data[i + 1] << 8);
                // #endif
            }
            // #ifdef PRINTDEBUG
            //     debugSerial.println();
            // #endif
            _dead_timer = millis();
            _state = DECODER_STATES::DISCARD;
            break;

        case DECODER_STATES::DISCARD:
            _data_counter = 0;
            _crc_calculated = 0xFFFF;
            _state = DECODER_STATES::DETECT;
            // flush buffer
            break;
        }
    }

    if (millis() - _dead_timer > DEAD_TIME) {
        _failsafe = true;
        _setDefaultChannelValue();
        #ifdef PRINTDEBUG
            debugSerial.println("\tInactive for too long, reset values");
        #endif
    } else {
        _failsafe = false;
    }

    return 1;
}
uint16_t FS_X6B::getChannel(const CHANNEL_SOURCE channel_number) {
    if ((uint8_t)channel_number >= 0 && (uint8_t)channel_number < CHANNEL_COUNT) {
        #ifdef PRINTDEBUG
            debugSerial.print("\t" + String((uint8_t)channel_number+1) + ":   " + String(_data[(uint8_t)channel_number]));
        #endif

        return _data[(uint8_t)channel_number];
    }
    return 0;
}

bool FS_X6B::isFailsafe() {
    #ifdef PRINTDEBUG
        debugSerial.println("\tFailsafe activated: " + String(_failsafe));
    #endif
    return _failsafe;
}

void FS_X6B::_setDefaultChannelValue() {
  for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
    _data[i] = (uint16_t)_channel_defaults[i];
  }
}