/**
 * @file encoder.h
 * @author Robbe Elsermans
 * @brief Enables encoding of the wheels and motors utilized by the RCV
 * @version 0.1
 * @date 2025-04-10
 * 
 * @note only wheel part is implemented
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */

#pragma once

#ifndef MOTOR_POSITION_H_
#define MOTOR_POSITION_H_

/** @brief All possible positions of the encoders */
enum MOTOR_POSITION {
    FL, /** @brief Front Left position */
    RL, /** @brief Rear Left position */
    RR, /** @brief Rear Right position */
    FR  /** @brief Front Right position */
};

#endif