/**
 * @file pin_config.h
 * @author Thomas Kramp (creator)
 * @brief Pin configurations for the TruggySense project.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

#pragma once

#define PIN_LED             32  /** @brief Pin definition of the indicator led. */

#define PIN_SERVO            2  /** @brief Pin definition of the steering servo. */

#define PIN_FR_MOTOR         3  /** @brief Pin definition of EXC of the front right Wheel motor. */
#define PIN_FL_MOTOR         4  /** @brief Pin definition of EXC of the front left Wheel motor. */
#define PIN_RR_MOTOR         5  /** @brief Pin definition of EXC of the rear right Wheel motor. */
#define PIN_RL_MOTOR         6  /** @brief Pin definition of EXC of the rear left Wheel motor. */

#define PIN_FAN             33  /** @brief Pin definition of the cooling fan. */

#define PIN_FR_ENCODER      24  /** @brief Pin definition of the front right wheel encoder. */
#define PIN_FL_ENCODER      25  /** @brief Pin definition of the front left wheel encoder. */
#define PIN_RR_ENCODER      26  /** @brief Pin definition of the rear right wheel encoder. */
#define PIN_RL_ENCODER      27  /** @brief Pin definition of the rear left wheel encoder. */

#define PIN_BATTERY_CELL_1  14  /** @brief Pin definition of the battery cell ranging from:  3.2V -  4.2V*/
#define PIN_BATTERY_CELL_2  15  /** @brief Pin definition of the battery cell ranging from:  6.4V -  8.4V*/
#define PIN_BATTERY_CELL_3  16  /** @brief Pin definition of the battery cell ranging from:  9.6V - 12.6V*/
#define PIN_BATTERY_CELL_4  17  /** @brief Pin definition of the battery cell ranging from: 12.8V - 16.8V*/
#define PIN_3V3_RAIL        40  /** @brief Pin definition of the voltage rail ranging from:  3.3V -  3.3V*/
#define PIN_5V0_RAIL        41  /** @brief Pin definition of the voltage rail ranging from:  4.8V -  5.1V*/

#define PIN_FR_HIGH_CURRENT 22  /** @brief Pin definition of the front right wheel's high current sensor. */
#define PIN_FL_HIGH_CURRENT 23  /** @brief Pin definition of the front left wheel's high current sensor. */
#define PIN_RR_HIGH_CURRENT 38  /** @brief Pin definition of the rear right wheel's high current sensor. */
#define PIN_RL_HIGH_CURRENT 39  /** @brief Pin definition of the rear left wheel's high current sensor. */

#define PIN_LONG_RANGE_CE    9  /** @brief Pin definition of the chip enable of the wireless data logger. */
#define PIN_LONG_RANGE_CSN  10  /** @brief Pin definition of the chip select of the wireless data logger. */

#endif