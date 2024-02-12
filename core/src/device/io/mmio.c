#include <device/io/map.h>
#include <device/io/mmio.h>
// #include <device/keyboard.h>
// #include <device/vga.h>
#include <memory/memory.h>
// #include <util/timer.h>

static DeviceMap maps[MMIO_NUM] = {};
static int map_index = 0;

static void printfDeviceMMIOOverlap(const char *name_a,
                                    paddr_t addr_a_l,
                                    paddr_t addr_a_r,
                                    const char *name_b,
                                    paddr_t addr_b_l,
                                    paddr_t addr_b_r) {
    PANIC("[device] MMIO region %s@[" FMT_PADDR ", " FMT_PADDR "] is "
          "overlapped with %s@[" FMT_PADDR ", " FMT_PADDR "]",
          name_a,
          addr_a_l,
          addr_a_r,
          name_b,
          addr_b_l,
          addr_b_r);
}

static DeviceMap *getDeviceMMIOMap(paddr_t addr) {
    int id = findDeviceMapIDByAddr(maps, map_index, addr);
    return (id == -1 ? NULL : &maps[id]);
}

void addDeviceMMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback) {
    assert(map_index < MMIO_NUM);
    paddr_t addr_l = addr;
    paddr_t addr_r = addr + len - 1;
    if (judgeMemoryPhyAddr(addr_l) || judgeMemoryPhyAddr(addr_r)) {
        printfDeviceMMIOOverlap(name,
                                addr_l,
                                addr_r,
                                "pmem",
                                PMEM_LEFT,
                                PMEM_RIGHT);
    }
    for (int i = 0; i < MMIO_NUM; i++) {
        if (addr_l <= maps[i].high &&
            addr_r >= maps[i].low) {
            printfDeviceMMIOOverlap(name,
                                    addr_l,
                                    addr_r,
                                    maps[i].name,
                                    maps[i].low,
                                    maps[i].high);
        }
    }

    maps[map_index] = (DeviceMap) {
        .name     = name,
        .low      = addr,
        .high     = addr + len - 1,
        .space    = space,
        .callback = callback
    };
    LOG_PURE_COLOR(
        "[device] mmio map '%s': [" FMT_PADDR ", " FMT_PADDR "]",
        maps[map_index].name,
        maps[map_index].low,
        maps[map_index].high);

    map_index++;
}

word_t readDeviceMMIOData(paddr_t addr, int len) {
    return readDeviceMapData(addr, len, getDeviceMMIOMap(addr));
}

void writeDeviceMMIOData(paddr_t addr, int len, word_t data) {
    writeDeviceMapData(addr, len, getDeviceMMIOMap(addr), data);
}
