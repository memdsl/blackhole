#include <device/io/map.h>
#include <device/io/pmio.h>
#include <device/io/mmio.h>
#include <device/serial.h>

static uint8_t *serial = NULL;

static void handleDeviceSerial(uint32_t offset, int len, bool write_flag) {
    assert(len == 1);
    switch (offset) {
        case 0: {
            if (write_flag) {
                putc(serial[0], stderr);
            }
            else {
                PANIC("[device] do not support read");
            }
            break;
        }
        default: {
            PANIC("[device] do not support offset = %d", offset);
        }
    }
}

void initDeviceSerial() {
    serial = (uint8_t *)newDeviceMapSpace(8);
#ifdef CONFIG_HAS_PMIO
    addDevicePMIOMap("serial  ",
                     CONFIG_SERIAL_PMIO,
                     serial,
                     8,
                     handleDeviceSerial);
#else
    addDeviceMMIOMap("serial  ",
                     CONFIG_SERIAL_MMIO,
                     serial,
                     8,
                     handleDeviceSerial);
#endif
}
