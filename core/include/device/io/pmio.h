#ifndef __PMIO_H__
#define __PMIO_H__

#include <common.h>
#include <device/io/map.h>

#define PMIO_NUM 16
#define PMIO_SPACE_MAX 65535

void addDevicePMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback);
word_t readDevicePMIOData(paddr_t addr, int len);
void writeDevicePMIOData(paddr_t addr, int len, word_t data);

#endif
