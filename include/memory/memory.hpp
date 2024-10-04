#ifndef MEMORY_H
#define MEMORY_H

#include <common.hpp>
#include <log.hpp>

class Memory {
public:
    Memory(shared_ptr<Log> log_ptr);
    ~Memory();
public:
    void initMemory();
private:
    uint8_t         g_mem_phy[CONFIG_MEMORY_SIZE] MEM_PG_ALIGN;
    shared_ptr<Log> g_log_ptr;
};

#endif
