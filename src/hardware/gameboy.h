/*
 * gameboy.h
 *
 *  Created on: May 28, 2019
 *      Author: Robert Phillips III
 */

#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <cstdint>
#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "gameboyinterface.h"
#include "gpu.h"
#include "memorycontroller.h"
#include "processor.h"

class GameBoy : public GameBoyInterface {
public:
    GameBoy();
    ~GameBoy() = default;

    bool load(const std::string & filename) override;

    void start() override;
    void stop() override;

    void advance() override;
    
    ColorArray getRGB() override { return m_gpu.getColorMap(); }
    
private:
    static const uint16_t ROM_HEADER_LENGTH;
    static const uint16_t ROM_NAME_OFFSET;
    static const uint16_t ROM_TYPE_OFFSET;
    static const uint16_t ROM_SIZE_OFFSET;
    static const uint16_t ROM_RAM_SIZE_OFFSET;
    static const uint8_t ROM_NAME_MAX_LENGTH;
    
    struct RomHeader {
        std::string name;
        
        uint16_t length;
    };
    
    MemoryController m_memory;
    Processor m_cpu;
    GPU m_gpu;

    std::condition_variable m_wait;
    std::mutex m_sync;
    
    std::atomic<bool> m_run;
    std::atomic<bool> m_advance;
    
    std::thread m_thread;

    void run();
    RomHeader parseHeader(const std::vector<uint8_t> & header);
};



#endif /* GAMEBOY_H_ */
