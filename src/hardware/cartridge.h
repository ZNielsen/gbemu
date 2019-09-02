#ifndef _CARTRIDGE_H
#define _CARTRIDGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "memmap.h"

class Cartridge {
public:
    explicit Cartridge(const std::string & path);
    ~Cartridge() = default;

    inline bool isValid() const { return m_valid; }

    inline uint8_t bank() const { return m_info.mbc.bank; }

    void write(uint16_t address, uint8_t value);
    uint8_t & read(uint16_t address);
    
private:
    static const uint16_t NINTENDO_LOGO_OFFSET;
    
    static const uint16_t ROM_HEADER_LENGTH;
    static const uint16_t ROM_NAME_OFFSET;
    static const uint16_t ROM_TYPE_OFFSET;
    static const uint16_t ROM_SIZE_OFFSET;
    static const uint16_t ROM_RAM_SIZE_OFFSET;
    static const uint8_t ROM_NAME_MAX_LENGTH;

    static const std::vector<uint8_t> NINTENDO_LOGO;
    
    enum BankType {
        MBC_NONE = 0x00,
        MBC_1    = 0x01,
        MBC_1R   = 0x02,
        MBC_1RB  = 0x03,
        MBC_2    = 0x05,

        MBC_3RB  = 0x13,
    };
    enum BankMode { MBC_ROM, MBC_RAM };
    
    std::string m_path;

    bool m_valid;

    struct {
        std::string name;
        uint32_t size;

        struct {
            BankType type;
            bool ramEn;
            uint8_t bank;
            BankMode mode;
        } mbc;
    } m_info;
    
    std::vector<uint8_t> m_memory;
    std::vector<uint8_t> m_ram;
    
    void writeROM(uint16_t address, uint8_t value);
    void writeRAM(uint16_t address, uint8_t value);

    void writeMBC1(uint16_t address, uint8_t value);
    void writeMBC2(uint16_t address, uint8_t value);
    void writeMBC3(uint16_t address, uint8_t value);
};

#endif
