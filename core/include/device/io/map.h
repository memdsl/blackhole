#ifndef __MAP_H__
#define __MAP_H__

#include <common.h>
#include <debug/difftest.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE  (1ul << PAGE_SHIFT)
#define PAGE_MASK  (PAGE_SIZE - 1)

#define MAP_SPACE_MAX (2 * 1024 * 1024)

typedef void(*callbackFunc)(uint32_t, int, bool);
typedef struct {
    const char  *name;
    paddr_t      low;
    paddr_t      high;
    void        *space;
    callbackFunc callback;
} DeviceMap;

static inline int findDeviceMapIDByAddr(DeviceMap *maps,
                                        int size,
                                        paddr_t addr) {
    int i;
    for (i = 0; i < size; i ++) {
        DeviceMap *map = (maps + i);
        if (addr >= map->low && addr <= map->high) {
            IFDEF(CONFIG_DIFFTEST, skipDebugDifftestRef());
            return i;
        }
    }
    return -1;
}

void initDeviceMap();
uint8_t *newDeviceMapSpace(int size);
word_t readDeviceMapData(paddr_t addr, int len, DeviceMap *map);
void writeDeviceMapData(paddr_t addr, int len, DeviceMap *map, word_t data);

#endif
