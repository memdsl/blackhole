#include <device/io/pmio.h>

static DeviceMap device_maps[DEVICE_MAP_NUM] = {};
static int device_map_index = 0;

void addDevicePMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback) {
    assert(device_map_index < DEVICE_MAP_NUM);
    assert(addr + len <= DEVICE_PMIO_SPACE_MAX);
    device_maps[device_map_index] = (DeviceMap) {
        .name     = name,
        .low      = addr,
        .high     = addr + len - 1,
        .space    = space,
        .callback = callback
    };
    LOG_PURE_COLOR(
        "[device] Add port-io map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
        device_maps[device_map_index].name,
        device_maps[device_map_index].low,
        device_maps[device_map_index].high);

    device_map_index ++;
}

word_t readDevicePMIOData(paddr_t addr, int len) {
    assert(addr + len - 1 < DEVICE_PMIO_SPACE_MAX);
    int id = findDeviceMapIDByAddr(device_maps, device_map_index, addr);
    assert(id != -1);
    return readDeviceMapData(addr, len, &device_maps[id]);
}

void writeDevicePMIOData(paddr_t addr, int len, word_t data) {
    assert(addr + len - 1 < DEVICE_PMIO_SPACE_MAX);
    int id = findDeviceMapIDByAddr(device_maps, device_map_index, addr);
    assert(id != -1);
    writeDeviceMapData(addr, len, &device_maps[id], data);
}
