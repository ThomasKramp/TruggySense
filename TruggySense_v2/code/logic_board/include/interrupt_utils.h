/**
 * @file interrupt_utils.h
 * @author Thomas Kramp (creator)
 * @brief Interrupt methods.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef INTERRUPT_UTILS_H_
#define INTERRUPT_UTILS_H_

#pragma once

// "inline" makes it so these functions are inserted as lines at the place of the call,
// instead of them being functions with a "call", losing the overhead.

/** @brief Enable interrupts  (replaces sei). */
inline void interrupts_enable()  { __asm__ volatile("CPSIE i" ::: "memory"); }
/** @brief Disable interrupts (replaces cli). */
inline void interrupts_disable() { __asm__ volatile("CPSID i" ::: "memory"); }

#endif