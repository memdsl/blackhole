#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <common.h>

#define PMEM_LEFT  ((paddr_t)CONFIG_MBASE)
#define PMEM_RIGHT ((paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT + CONFIG_PC_RESET_OFFSET)

uint8_t *convertGuestToHost(paddr_t paddr);
paddr_t  convertHostToGuest(uint8_t *haddr);

bool judgeMemoryPhyAddr(paddr_t addr);

word_t readMemoryPhyData(paddr_t addr, int len);
void writeMemoryPhyData(paddr_t addr, int len, word_t data);

void genMemoryFile(const char *mem_file, int size);
void initMemory();
void printfMemoryData(int size);

#endif
