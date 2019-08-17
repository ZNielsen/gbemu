#ifndef _MEMBANK_CONTROLLER_H
#define _MEMBANK_CONTROLLER_H

#include <memory>

class MemoryController;

class MemBankController {
public:
     MemBankController(MemoryController & parent) : m_parent(parent) { }
    ~MemBankController() = default;

    virtual bool isAddressed(uint16_t address) const;

    virtual void write(uint16_t address, uint8_t value) = 0;
    
    static std::shared_ptr<MemBankController> instance(MemoryController & parent);
    
protected:
    MemoryController & m_parent;

    struct {
        bool enabled;
        uint8_t selected;
    } m_rom;

    struct {
        bool enabled;
        uint8_t selected;
    } m_ram;
};

class BankController1 : public MemBankController {
public:
     BankController1(MemoryController & parent)
         : MemBankController(parent) { }

    ~BankController1() = default;

    void write(uint16_t address, uint8_t value) override;

 
};

#endif
