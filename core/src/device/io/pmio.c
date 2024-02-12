#include <device/io/pmio.h>

static DeviceMap maps[PMIO_NUM] = {};
static int map_index = 0;

void addDevicePMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback) {
    assert(map_index < PMIO_NUM);
    assert(addr + len <= PMIO_SPACE_MAX);
    maps[map_index] = (DeviceMap) {
        .name     = name,
        .low      = addr,
        .high     = addr + len - 1,
        .space    = space,
        .callback = callback
    };
    LOG_PURE_COLOR(
        "[device] Add port-io map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
        maps[map_index].name,
        maps[map_index].low,
        maps[map_index].high);

    map_index ++;
}

word_t readDevicePMIOData(paddr_t addr, int len) {
    assert(addr + len - 1 < PMIO_SPACE_MAX);
    int id = findDeviceMapIDByAddr(maps, map_index, addr);
    assert(id != -1);
    return readDeviceMapData(addr, len, &maps[id]);
}

void writeDevicePMIOData(paddr_t addr, int len, word_t data) {
    assert(addr + len - 1 < PMIO_SPACE_MAX);
    int id = findDeviceMapIDByAddr(maps, map_index, addr);
    assert(id != -1);
    writeDeviceMapData(addr, len, &maps[id], data);
}
