/**
 * @file SDCard.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief SDCard interface for on board data logging.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef SDCARD_H_
#define SDCARD_H_

#pragma once

#include <SD.h>
#include <SPI.h>

#include "module.h"
#include "structs.h"

/** @brief Header row of the csv file. */
static const char CSV_HEADER[] = 
    "timestamp,date,time,latitude,longitude,velocity,altitude,c_s,"
    "ch_1,ch_2,ch_3,ch_4,ch_5,ch_6,ch_7,ch_8,ch_9,ch_10,failsafe,"
    "rpm_wheel_1,rpm_wheel_2,rpm_wheel_3,rpm_wheel_4,"
    "v_cell_1,v_cell_2,v_cell_3,v_cell_4,v_rail_3V3,v_rail_5v0,"
    "i_esc_1,i_esc_2,i_esc_3,i_esc_4,i_etc,i_llc,i_hlc,"
    "t_llc,t_hlc,t_bp,t_ss,t_bm_1,t_bm_2,t_bm_3,t_bm_4,"
    "t_esc_1,t_esc_2,t_esc_3,t_esc_4,"
    "roll,pitch,yaw,w,i,j,k";

/**
 * @brief The size of the header row of to check if the file is empty.
 * @note -1 for the null terminator.
 * @note +2 for the println's \r\n
 */
#define CSV_HEADER_SIZE (sizeof(CSV_HEADER) - 1 + 2)

/** @brief Internal SPI enable pin for the built-in SD-card of the LLC. */
#define CHIP_SELECT     BUILTIN_SDCARD

/** @brief Supported file types to log onto the SD-card. */
enum class FILE_TYPE {
    BINARY,     /** @brief .bin file, for low data storage. */
    CSV,        /** @brief .csv file. */
    JSON        /** @brief .json file, for matlab compatibility. */
};

/**
 * @brief Converts a FILE_TYPE enum value to its string representation.
 * @param e The FILE_TYPE to convert.
 * @return The string representation of the file type.
 */
inline const char* fileTypeToString(FILE_TYPE e) {
    switch (e) {
        case FILE_TYPE::BINARY: return ".bin";
        case FILE_TYPE::CSV:    return ".csv";
        case FILE_TYPE::JSON:   return ".json";
        default:                return "";
    }
}

class SDCard : public Module {
    private:
        const char _fileNamePrefix[10] = "llc_data_"; /** @brief Prefix for log filenames. */
        uint16_t   _nameCounter = 0;                  /** @brief Counter to ensure unique filenames. */
        const FILE_TYPE _fileType;                    /** @brief The type of file to log to (BIN, CSV, JSON). */
        File _file;                                   /** @brief File object for the current log file. */
        bool _jsonFirstRecord;                        /** @brief Tracks whether to prepend a comma for JSON logging.  */

        /**
         * @brief Checks if a file contains valid data.
         * @param filename The filename to check.
         * @return true if the file contains valid data, false otherwise.
         */
        bool fileHasData(const String& filename);

        /**
         * @brief Generates a unique filename for the logging files.
         * @return A unique filename string.
         */
        String generateUniqueFilename();

        /**
         * @brief Writes logging data to the binary file.
         * @param log_data The data to log.
         */
        void writeBin(const logging_data_t &log_data);

        /**
         * @brief Writes logging data to the CSV file.
         * @param log_data The data to log.
         */
        void writeCsv(const logging_data_t &log_data);

        /**
         * @brief Writes logging data to the JSON file.
         * @param log_data The data to log.
         */
        void writeJson(const logging_data_t &log_data);

    public:
        /** @brief Constructor for SDCard. */
        SDCard(const FILE_TYPE type = FILE_TYPE::BINARY)
        : Module("SD-Card - Onboard logger"),
          _fileType(type),
          _jsonFirstRecord(true) {}

        /**
         * @brief Initializes the SD card module.
         * @return true if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Sends data to be logged in all formats.
         * @param log_data The data to log.
         */
        void sendData(const logging_data_t &log_data);

        /** @brief Opens a new log file. */
        void openFile();
        /** @brief Closes the open log file. */
        void closeFile();

        /**
         * @brief Gets the filename of the log file.
         * @return The filename string.
         */
        String getFileName();

        /**
         * @brief Gets the size of the log file.
         * @return The file size in bytes.
         */
        uint64_t getFileSize();
};

#endif