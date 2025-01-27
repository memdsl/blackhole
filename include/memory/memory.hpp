#ifndef MEMORY_H
#define MEMORY_H

#include <common.hpp>

#define MEM_PHY_LEFT  ((u_addr_t)CONFIG_MEMORY_BASE)
#define MEM_PHY_RIGHT ((u_addr_t)CONFIG_MEMORY_BASE + CONFIG_MEMORY_SIZE - 1)
#define MEM_PG_ALIGN  __attribute((aligned(4096)))

class Memory {
public:
    Memory();
    ~Memory();
public:
    void initMemory();
private:
    uint8_t g_mem_phy[CONFIG_MEMORY_SIZE] MEM_PG_ALIGN;
};

#endif
