/**
 * @file module.h
 * @author Thomas Kramp (creator)
 * @brief Base class for the module classes.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef MODULE_H_
#define MODULE_H_

#pragma once

#include <Arduino.h>

#include "interrupt_utils.h"

/** @brief Serial output for debugging. */
extern Print &debugSerial;

/**
 * @enum ModuleError
 * @brief Enumeration of possible module error states.
 */
enum class ModuleError {
    OK = 0,              /** @brief No error, operation successful. */
    INIT_FAILED,         /** @brief Module initialization failed. */
    LOG_FAILED,          /** @brief Logging operation failed. */
    DISABLED,            /** @brief Module is disabled. */
    REPORT_FAILED,       /** @brief Error report generation failed. */
    EMPTY_BATTERY,       /** @brief Battery is empty. */
};

/**
 * @brief Converts a ModuleError enum value to its string representation.
 * @param e The ModuleError to convert.
 * @return The string representation of the error.
 */
inline const char* moduleErrorToString(ModuleError e) {
    switch (e) {
        case ModuleError::OK:               return "OK";
        case ModuleError::INIT_FAILED:      return "Init failed";
        case ModuleError::LOG_FAILED:       return "Log failed";
        case ModuleError::DISABLED:         return "Disabled";
        case ModuleError::REPORT_FAILED:    return "Report failed";
        case ModuleError::EMPTY_BATTERY:    return "Empty battery";
        default:                            return "Unknown error";
    }
}

class Module {
    private:
        const String _name;     /** @brief Name of the module. */
        ModuleError _lastError; /** @brief Last error that occurred. */

    protected:
        /** 
         * @brief Sets the module's error state.
         * @param e The new ModuleError.
        */
        void setError(ModuleError e) { _lastError = e; }

    public:
        /**
         * @brief Constructor for the Module parent class.
         * @param name The name of the module.
         */
        explicit Module(const String& name) 
        : _name(name), 
          _lastError(ModuleError::OK) {}

        /** @brief Virtual destructor for proper cleanup. */
        virtual ~Module() = default;

        /** 
         * @brief Initializes the module.
         * @return true if initialization is successful, false otherwise.
         */
        virtual bool init()   = 0;

        /**
         * @brief Gets the last error that occurred.
         * @return The last ModuleError.
         */
        ModuleError  getLastError()  const { return _lastError; }
        /**
         * @brief Gets the string representation of the last error.
         * @return The error string.
         */
        const char*  getErrorString()const { return moduleErrorToString(_lastError); }
        /**
         * @brief Gets the name of the module.
         * @return The module name.
         */
        const String& getName() const { return _name; }

        /**
         * @brief Generates a formatted error report.
         * @return The error report string.
         */
        String getErrorReport() const {
            return "[" + _name + "] " + moduleErrorToString(_lastError);
        }

        /** @brief Clears the last error, resetting it to OK. */
        void clearError() { _lastError = ModuleError::OK; }


};

#endif