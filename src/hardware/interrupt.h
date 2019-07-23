/*
 * interrupt.h
 *
 *  Created on: Jun 17, 2019
 *      Author: Robert Phillips III
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <cstdint>
#include "memmap.h"
#include "memorycontroller.h"

enum class InterruptVector : uint16_t {
    VBLANK = 0x40,
    LCD    = 0x48,
    TIMER  = 0x50,
    SERIAL = 0x58,
    JOYPAD = 0x60,

    INVALID = 0x00,
};

enum class InterruptMask : uint8_t {
    VBLANK = 0x01,
    LCD    = 0x02,
    TIMER  = 0x04,
    SERIAL = 0x08,
    JOYPAD = 0x10,
};

struct Interrupts {
    Interrupts(uint8_t & m, uint8_t & s)
        : enable(true), mask(m), status(s) { }
    ~Interrupts() = default;

    bool enable;

    uint8_t & mask;
    uint8_t & status;

    inline void reset() { enable = true; mask = status = 0x00; }

    static void set(MemoryController & memory, InterruptMask interrupt)
    {
        uint8_t & current = memory.read(INTERRUPT_FLAGS_ADDRESS);

        uint8_t enabled = memory.read(INTERRUPT_MASK_ADDRESS);
        if (enabled & uint8_t(interrupt)) {
            current |= uint8_t(interrupt);
        }
    }

    static void clear(MemoryController & memory, InterruptMask interrupt)
    {
        uint8_t & current = memory.read(INTERRUPT_FLAGS_ADDRESS);

        current &= uint8_t(interrupt);
    }
};

#endif /* INTERRUPT_H_ */
