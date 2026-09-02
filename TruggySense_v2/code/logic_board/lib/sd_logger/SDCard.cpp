/**
 * @file SDCard.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of SDCard.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "SDCard.h"

#define PRINTDEBUG

bool SDCard::init() {
    
    if (!SD.begin(CHIP_SELECT)) {
        setError(ModuleError::INIT_FAILED);
        #ifdef PRINTDEBUG
            debugSerial.println("\t" + String(getErrorString()));
        #endif
        return false;
    }

    openFile();

    if (!_file) {
        setError(ModuleError::INIT_FAILED);
        return false;
    }

    return true;
}

void SDCard::sendData(const logging_data_t &log_data) {
    if (!_file) {
        openFile();
        if (!_file) {
            setError(ModuleError::LOG_FAILED);
            return;
        }
    }

    setError(ModuleError::OK);

    // _file.println("Ewa broeder, Nu ook fysiek");
    // _file.flush();
    // #ifdef PRINTDEBUG
    //     debugSerial.println("\tWrite:\t" + _filename);
    // #endif
    
    switch (_fileType) {
        case FILE_TYPE::BINARY:
            writeBin(log_data);
            break;
        case FILE_TYPE::CSV:
            writeCsv(log_data);
            break;
        case FILE_TYPE::JSON:
            writeJson(log_data);
            break;
        default:
            setError(ModuleError::LOG_FAILED);
            break;
    }
}
void SDCard::writeBin(const logging_data_t &log_data) {
    if (!_file) return;

    uint8_t buffer[sizeof(log_data)];
    memcpy(buffer, &log_data, sizeof(log_data));
    _file.write(buffer, sizeof(buffer));
    _file.flush();
}
void SDCard::writeCsv(const logging_data_t &log_data) {
    if (!_file) return;

    // Timestamp & GPS
    _file.print(log_data.timestamp);   _file.print(',');
    _file.print(log_data.date);        _file.print(',');
    _file.print(log_data.time);        _file.print(',');
    _file.print(log_data.latitude, 6); _file.print(',');
    _file.print(log_data.longitude, 6);_file.print(',');
    _file.print(log_data.velocity, 3); _file.print(',');
    _file.print(log_data.altitude, 3); _file.print(',');
    _file.print(log_data.c_s);         _file.print(',');
    // RC channels
    _file.print(log_data.ch_1);        _file.print(',');
    _file.print(log_data.ch_2);        _file.print(',');
    _file.print(log_data.ch_3);        _file.print(',');
    _file.print(log_data.ch_4);        _file.print(',');
    _file.print(log_data.ch_5);        _file.print(',');
    _file.print(log_data.ch_6);        _file.print(',');
    _file.print(log_data.ch_7);        _file.print(',');
    _file.print(log_data.ch_8);        _file.print(',');
    _file.print(log_data.ch_9);        _file.print(',');
    _file.print(log_data.ch_10);       _file.print(',');
    _file.print(log_data.failsafe);    _file.print(',');
    // RPM
    _file.print(log_data.rpm_wheel_1, 2); _file.print(',');
    _file.print(log_data.rpm_wheel_2, 2); _file.print(',');
    _file.print(log_data.rpm_wheel_3, 2); _file.print(',');
    _file.print(log_data.rpm_wheel_4, 2); _file.print(',');
    // Voltage
    _file.print(log_data.v_cell_1, 3); _file.print(',');
    _file.print(log_data.v_cell_2, 3); _file.print(',');
    _file.print(log_data.v_cell_3, 3); _file.print(',');
    _file.print(log_data.v_cell_4, 3); _file.print(',');
    _file.print(log_data.v_rail_3V3, 3);_file.print(',');
    _file.print(log_data.v_rail_5v0, 3);_file.print(',');
    // Current
    _file.print(log_data.i_esc_1, 3);  _file.print(',');
    _file.print(log_data.i_esc_2, 3);  _file.print(',');
    _file.print(log_data.i_esc_3, 3);  _file.print(',');
    _file.print(log_data.i_esc_4, 3);  _file.print(',');
    _file.print(log_data.i_etc, 3);    _file.print(',');
    _file.print(log_data.i_llc, 3);    _file.print(',');
    _file.print(log_data.i_hlc, 3);    _file.print(',');
    // Temperature
    _file.print(log_data.t_llc, 2);    _file.print(',');
    _file.print(log_data.t_hlc, 2);    _file.print(',');
    _file.print(log_data.t_bp, 2);     _file.print(',');
    _file.print(log_data.t_ss, 2);     _file.print(',');
    _file.print(log_data.t_bm_1, 2);   _file.print(',');
    _file.print(log_data.t_bm_2, 2);   _file.print(',');
    _file.print(log_data.t_bm_3, 2);   _file.print(',');
    _file.print(log_data.t_bm_4, 2);   _file.print(',');
    _file.print(log_data.t_esc_1, 2);  _file.print(',');
    _file.print(log_data.t_esc_2, 2);  _file.print(',');
    _file.print(log_data.t_esc_3, 2);  _file.print(',');
    _file.print(log_data.t_esc_4, 2);  _file.print(',');
    // IMU
    _file.print(log_data.roll, 4);     _file.print(',');
    _file.print(log_data.pitch, 4);    _file.print(',');
    _file.print(log_data.yaw, 4);      _file.print(',');
    _file.print(log_data.w, 6);        _file.print(',');
    _file.print(log_data.i, 6);        _file.print(',');
    _file.print(log_data.j, 6);        _file.print(',');
    _file.print(log_data.k, 6);        _file.print('\n');

    _file.flush();
}
static void jInt(File &f, const char *key, long val, bool comma = true) {
    if (comma) f.print(',');
    f.print('"'); f.print(key); f.print("\":"); f.print(val);
}
static void jFloat(File &f, const char *key, double val, uint8_t decimals, bool comma = true) {
    if (comma) f.print(',');
    f.print('"'); f.print(key); f.print("\":"); f.print(val, decimals);
}
void SDCard::writeJson(const logging_data_t &log_data) {
    if (!_file) return;

    // Seek back 1 byte to overwrite the closing ']'
    uint32_t pos = _file.size();
    _file.seek(pos - 1);

    // Separator between records
    if (!_jsonFirstRecord) _file.print(',');
    _jsonFirstRecord = false;

    _file.print('{');

    // Timestamp & GPS  (first field: no leading comma)
    jInt  (_file, "timestamp", log_data.timestamp,    false);
    jInt  (_file, "date",      log_data.date);
    jInt  (_file, "time",      log_data.time);
    jFloat(_file, "latitude",  log_data.latitude,  6);
    jFloat(_file, "longitude", log_data.longitude, 6);
    jFloat(_file, "velocity",  log_data.velocity,  3);
    jFloat(_file, "altitude",  log_data.altitude,  3);
    jInt  (_file, "c_s",       log_data.c_s);

    // RC channels
    jInt(_file, "ch_1",     log_data.ch_1);
    jInt(_file, "ch_2",     log_data.ch_2);
    jInt(_file, "ch_3",     log_data.ch_3);
    jInt(_file, "ch_4",     log_data.ch_4);
    jInt(_file, "ch_5",     log_data.ch_5);
    jInt(_file, "ch_6",     log_data.ch_6);
    jInt(_file, "ch_7",     log_data.ch_7);
    jInt(_file, "ch_8",     log_data.ch_8);
    jInt(_file, "ch_9",     log_data.ch_9);
    jInt(_file, "ch_10",    log_data.ch_10);
    jInt(_file, "failsafe", log_data.failsafe);

    // RPM
    jFloat(_file, "rpm_wheel_1", log_data.rpm_wheel_1, 2);
    jFloat(_file, "rpm_wheel_2", log_data.rpm_wheel_2, 2);
    jFloat(_file, "rpm_wheel_3", log_data.rpm_wheel_3, 2);
    jFloat(_file, "rpm_wheel_4", log_data.rpm_wheel_4, 2);

    // Voltage
    jFloat(_file, "v_cell_1",   log_data.v_cell_1,   3);
    jFloat(_file, "v_cell_2",   log_data.v_cell_2,   3);
    jFloat(_file, "v_cell_3",   log_data.v_cell_3,   3);
    jFloat(_file, "v_cell_4",   log_data.v_cell_4,   3);
    jFloat(_file, "v_rail_3V3", log_data.v_rail_3V3, 3);
    jFloat(_file, "v_rail_5v0", log_data.v_rail_5v0, 3);

    // Current
    jFloat(_file, "i_esc_1", log_data.i_esc_1, 3);
    jFloat(_file, "i_esc_2", log_data.i_esc_2, 3);
    jFloat(_file, "i_esc_3", log_data.i_esc_3, 3);
    jFloat(_file, "i_esc_4", log_data.i_esc_4, 3);
    jFloat(_file, "i_etc",   log_data.i_etc,   3);
    jFloat(_file, "i_llc",   log_data.i_llc,   3);
    jFloat(_file, "i_hlc",   log_data.i_hlc,   3);

    // Temperature
    jFloat(_file, "t_llc",   log_data.t_llc,   2);
    jFloat(_file, "t_hlc",   log_data.t_hlc,   2);
    jFloat(_file, "t_bp",    log_data.t_bp,    2);
    jFloat(_file, "t_ss",    log_data.t_ss,    2);
    jFloat(_file, "t_bm_1",  log_data.t_bm_1,  2);
    jFloat(_file, "t_bm_2",  log_data.t_bm_2,  2);
    jFloat(_file, "t_bm_3",  log_data.t_bm_3,  2);
    jFloat(_file, "t_bm_4",  log_data.t_bm_4,  2);
    jFloat(_file, "t_esc_1", log_data.t_esc_1, 2);
    jFloat(_file, "t_esc_2", log_data.t_esc_2, 2);
    jFloat(_file, "t_esc_3", log_data.t_esc_3, 2);
    jFloat(_file, "t_esc_4", log_data.t_esc_4, 2);

    // IMU
    jFloat(_file, "roll",  log_data.roll,  4);
    jFloat(_file, "pitch", log_data.pitch, 4);
    jFloat(_file, "yaw",   log_data.yaw,   4);
    jFloat(_file, "w",     log_data.w,     6);
    jFloat(_file, "i",     log_data.i,     6);
    jFloat(_file, "j",     log_data.j,     6);
    jFloat(_file, "k",     log_data.k,     6);

    _file.print("}]");
    _file.flush();
}

bool SDCard::fileHasData(const String& filename) {
    if (!SD.exists(filename.c_str())) return false;
    File f = SD.open(filename.c_str(), FILE_READ);
    if (!f) return false;
    uint32_t size = f.size();
    f.close();

    switch (_fileType) {
        case FILE_TYPE::BINARY: return size > 0;
        case FILE_TYPE::CSV:    return size > CSV_HEADER_SIZE;
        case FILE_TYPE::JSON:   return size > 2; // "[]" = 2 bytes
        default:                return false;
    }
}
String SDCard::generateUniqueFilename() {
    while (fileHasData(getFileName())) {
        _nameCounter++;
    }
    #ifdef PRINTDEBUG
        debugSerial.println("\tFilename:\t" + getFileName());
    #endif
    return getFileName();
}
void SDCard::openFile() {
    String filename = generateUniqueFilename();

    SD.remove(filename.c_str()); // Ensure we're starting with a clean file
    _file = SD.open(filename.c_str(), FILE_WRITE);
    #ifdef PRINTDEBUG
        debugSerial.println("\tFile opened. Size: " + String(_file.size()) + " bytes");
    #endif

    switch (_fileType) {
        case FILE_TYPE::BINARY: break;
        case FILE_TYPE::CSV:
            // CSV header row
            _file.println(CSV_HEADER);
            _file.flush();
            break;
        case FILE_TYPE::JSON:
            // JSON: start the array
            _file.print("[]");
            _file.flush();
            _jsonFirstRecord = true;
            break;
        default:
            setError(ModuleError::INIT_FAILED);
            break;
    }
}
void SDCard::closeFile() {
    if (_file) {
        _file.flush();
        #ifdef PRINTDEBUG
            debugSerial.println("\tFile closed. Size: " + String(_file.size()) + " bytes");
        #endif
        _file.close();
    }
}

String SDCard::getFileName() { 
    return _fileNamePrefix + String(_nameCounter) + fileTypeToString(_fileType);
}
uint64_t SDCard::getFileSize() { 
    return _file.size(); 
}