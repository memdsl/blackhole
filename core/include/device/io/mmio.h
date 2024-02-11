#ifndef __MMIO_H__
#define __MMIO_H__

#include <common.h>
#include <device/io/map.h>

#define DEVICE_MAP_NUM 16

void addDeviceMMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback);
word_t readDeviceMMIOData(paddr_t addr, int len);
void writeDeviceMMIOData(paddr_t addr, int len, word_t data);

#endif
