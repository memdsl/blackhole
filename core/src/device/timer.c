#include <device/io/map.h>
#include <device/io/pmio.h>
#include <device/io/mmio.h>
#include <device/timer.h>
#include <util/timer.h>

static uint32_t *timer = NULL;

static void handleDeviceTimer(uint32_t offset, int len, bool write_flag) {
    assert(offset == 0 || offset == 4);
    if (!write_flag && offset == 4) {
        uint64_t us = getTimerData();
        timer[0] = (uint32_t)us;
        timer[1] = us >> 32;
    }
}

void initDeviceTimer() {
    timer = (uint32_t *)newDeviceMapSpace(8);
#ifdef CONFIG_HAS_PMIO
    addDevicePMIOMap("timer   ",
                     CONFIG_TIMER_PMIO,
                     timer,
                     8,
                     handleDeviceTimer);
#else
    addDeviceMMIOMap("timer   ",
                     CONFIG_TIMER_MMIO,
                     timer,
                     8,
                     handleDeviceTimer);
#endif
}
