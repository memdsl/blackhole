#include <device/mmio.h>

#include <device/keyboard.h>
#include <device/vga.h>
#include <util/timer.h>

word_t readDeviceMMIOData(paddr_t addr, int len) {
    word_t data = 0;

    if (addr == CONFIG_TIMER_MMIO) {
        data = (word_t)getTimerValue();
    }
    else if (addr == CONFIG_KEYBOARD_MMIO) {
        data = (word_t)dequeueDiviceKey();
    }
    else if (addr == CONFIG_VGA_CTL_MMIO) {
        data = (word_t)(getDeviceVGAScreenWidth() << 16 |
                        getDeviceVGAScreenHeight());
    }

    return data;
}

void writeDeviceMMIOData(paddr_t addr, int len, word_t data) {
    if (addr == CONFIG_SERIAL_MMIO) {
        putc((uint8_t)data, stderr);
    }
}
