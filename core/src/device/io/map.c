#include <debug/trace.h>
#include <device/io/map.h>
#include <isa/isa.h>
#include <memory/host.h>

static uint8_t *map_space_all  = NULL;
static uint8_t *map_space_curr = NULL;

static void checkDeviceMapBound(DeviceMap *map, paddr_t addr) {
    if (map == NULL) {
        ASSERT(
            map != NULL,
            "[device] address (" FMT_PADDR ") is out of bound at pc = " FMT_WORD,
            addr,
            cpu.pc);
    }
    else {
        ASSERT(
            addr <= map->high && addr >= map->low,
            "[device] address (" FMT_PADDR ") is out of bound {%s} [" FMT_PADDR
            ", " FMT_PADDR "] at pc = " FMT_WORD,
            addr,
            map->name,
            map->low,
            map->high,
            cpu.pc);
    }
}

static void invokeDeviceMapCallbackFunc(callbackFunc callback,
                                        paddr_t offset,
                                        int len,
                                        bool write_flag) {
    if (callback != NULL) {
        callback(offset, len, write_flag);
    }
}

void initDeviceMap() {
    map_space_all = (uint8_t *)malloc(MAP_SPACE_MAX);
    assert(map_space_all);
    map_space_curr = map_space_all;
}

uint8_t *newDeviceMapSpace(int size) {
    uint8_t *device_space_temp = map_space_curr;
    size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
    map_space_curr += size;
    assert(map_space_curr - map_space_all < MAP_SPACE_MAX);
    return device_space_temp;
}

word_t readDeviceMapData(paddr_t addr, int len, DeviceMap *map) {
    assert(len >= 1 && len <= 8);
    checkDeviceMapBound(map, addr);
    paddr_t offset = addr - map->low;
    invokeDeviceMapCallbackFunc(map->callback, offset, len, false);
    word_t data = readMemoryHost((paddr_t *)map->space + offset, len);
#ifdef CONFIG_DTRACE_PROCESS
    printfDebugDTrace((char *)"process",
                      (char *)map->name,
                      (char *)"rd",
                      addr,
                      data);
#endif
    return data;
}

void writeDeviceMapData(paddr_t addr, int len, DeviceMap *map, word_t data) {
    assert(len >= 1 && len <= 8);
    checkDeviceMapBound(map, addr);
    paddr_t offset = addr - map->low;
    writeMemoryHost((paddr_t *)map->space + offset, len, data);
    invokeDeviceMapCallbackFunc(map->callback, offset, len, true);
#ifdef CONFIG_DTRACE_PROCESS
    printfDebugDTrace((char *)"process",
                      (char *)map->name,
                      (char *)"wr",
                      addr,
                      data);
#endif
}
