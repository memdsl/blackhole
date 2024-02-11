#ifndef __MMIO_H__
#define __MMIO_H__

#include <common.h>

word_t readDeviceMMIOData(paddr_t addr, int len);
void writeDeviceMMIOData(paddr_t addr, int len, word_t data);

#endif
