#include <memory>
#include <cassert>

#include "memorycontroller.h"
#include "membankcontroller.h"
#include "memmap.h"

using std::shared_ptr;

shared_ptr<MemBankController> MemBankController::instance(MemoryController & parent)
{
    
    
    return shared_ptr<MemBankController>(new BankController1(parent));
}

MemBankController::MemBankController(MemoryController & parent)
    : m_parent(parent),
      m_selectedROM(0),
      m_selectedRAM(0)
{ }

bool MemBankController::isAddressed(uint16_t address) const
{
    return (address < (ROM_0_SIZE + ROM_1_SIZE));
}
