#include <device/io/map.h>
#include <device/io/mmio.h>
// #include <device/keyboard.h>
// #include <device/vga.h>
#include <memory/memory.h>
// #include <util/timer.h>

static DeviceMap device_maps[DEVICE_MAP_NUM] = {};
static int device_map_index = 0;

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
    int id = findDeviceMapIDByAddr(device_maps, device_map_index, addr);
    return (id == -1 ? NULL : &device_maps[id]);
}

void addDeviceMMIOMap(const char *name,
                      paddr_t addr,
                      void *space,
                      uint32_t len,
                      callbackFunc callback) {
    assert(device_map_index < DEVICE_MAP_NUM);
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
    for (int i = 0; i < DEVICE_MAP_NUM; i++) {
        if (addr_l <= device_maps[i].high &&
            addr_r >= device_maps[i].low) {
            printfDeviceMMIOOverlap(name,
                                    addr_l,
                                    addr_r,
                                    device_maps[i].name,
                                    device_maps[i].low,
                                    device_maps[i].high);
        }
    }

    device_maps[device_map_index] = (DeviceMap) {
        .name     = name,
        .low      = addr,
        .high     = addr + len - 1,
        .space    = space,
        .callback = callback
    };
    LOG_PURE_COLOR(
        "[device] Add mmio map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
        device_maps[device_map_index].name,
        device_maps[device_map_index].low,
        device_maps[device_map_index].high);

    device_map_index++;
}

word_t readDeviceMMIOData(paddr_t addr, int len) {
    return readDeviceMapData(addr, len, getDeviceMMIOMap(addr));
}

void writeDeviceMMIOData(paddr_t addr, int len, word_t data) {
    writeDeviceMapData(addr, len, getDeviceMMIOMap(addr), data);
}

// word_t readDeviceMMIOData(paddr_t addr, int len) {
//     word_t data = 0;

//     if (addr == CONFIG_TIMER_MMIO) {
//         data = (word_t)getTimerValue();
//     }
//     else if (addr == CONFIG_KEYBOARD_MMIO) {
//         data = (word_t)dequeueDiviceKey();
//     }
//     else if (addr == CONFIG_VGA_CTL_MMIO) {
//         data = (word_t)(getDeviceVGAScreenWidth() << 16 |
//                         getDeviceVGAScreenHeight());
//     }

//     return data;
// }

// void writeDeviceMMIOData(paddr_t addr, int len, word_t data) {
//     if (addr == CONFIG_SERIAL_MMIO) {
//         putc((uint8_t)data, stderr);
//     }
// }
