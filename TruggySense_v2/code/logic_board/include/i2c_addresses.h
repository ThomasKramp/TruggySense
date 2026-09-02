/**
 * @file i2c_addresses.h
 * @author Thomas Kramp (creator)
 * @brief I2C addresses for the TruggySense project.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef I2C_ADDRESSES_H_
#define I2C_ADDRESSES_H_

#pragma once

#define INA219_ADDRESS_1    0x40 /** @brief The I2C first (default) address of the INA219 sensor. */
#define INA219_ADDRESS_2    0x41 /** @brief The I2C second address of the INA219 sensor. */
#define INA219_ADDRESS_3    0x44 /** @brief The I2C third address of the INA219 sensor. */
#define INA219_ADDRESS_4    0x45 /** @brief The I2C fourth address of the INA219 sensor. */

#define LM75_ADDRESS_1      0x48 /** @brief The I2C first (default) address of the LM75 sensor. */
#define LM75_ADDRESS_2      0x49 /** @brief The I2C second address of the LM75 sensor. */
#define LM75_ADDRESS_3      0x4A /** @brief The I2C third address of the LM75 sensor. */
#define LM75_ADDRESS_4      0x4B /** @brief The I2C fourth address of the LM75 sensor. */
#define LM75_ADDRESS_5      0x4C /** @brief The I2C fifth address of the LM75 sensor. */
#define LM75_ADDRESS_6      0x4D /** @brief The I2C sixth address of the LM75 sensor. */
#define LM75_ADDRESS_7      0x4E /** @brief The I2C seventh address of the LM75 sensor. */
#define LM75_ADDRESS_8      0x4F /** @brief The I2C eighth address of the LM75 sensor. */

#define BNO085_ADDRESS_1    0x4A /** @brief The I2C first (default) address of the BNO085 sensor. */
#define BNO085_ADDRESS_2    0x4B /** @brief The I2C second address of the BNO085 sensor. */

#define PIM448_ADDRESS_1    0x68 /** @brief The I2C first (default) address of the PIM448 sensor. */
#define PIM448_ADDRESS_2    0x69 /** @brief The I2C second address of the PIM448 sensor. */

#endif