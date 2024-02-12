#include <SDL2/SDL.h>
#include <device/io/map.h>
#include <device/io/pmio.h>
#include <device/io/mmio.h>
#include <device/keyboard.h>
#include <state.h>

static uint32_t keymap[256] = {};

static void initDeviceKeyboardMap() {
    MAP(KEYS, SDL_KEYMAP)
}

#define KEY_QUEUE_LEN 1024
static int key_queue[KEY_QUEUE_LEN] = {};
static int key_curr = 0, key_next = 0;

static void enqueueDeviceKeyboard(uint32_t am_scancode) {
    key_queue[key_next] = am_scancode;
    key_next = (key_next + 1) % KEY_QUEUE_LEN;
    ASSERT(key_next != key_curr, "[device] key queue overflow!");
}

static uint32_t dequeueDiviceKeyboard() {
    uint32_t key = KEY_NONE;
    if (key_curr != key_next) {
        key = key_queue[key_curr];
        key_curr = (key_curr + 1) % KEY_QUEUE_LEN;
    }
    return key;
}

static uint32_t *keyboard = NULL;

static void handleDeviceKeyboard(uint32_t offset, int len, bool write_flag) {
    assert(!write_flag);
    assert(offset == 0);
    keyboard[0] = dequeueDiviceKeyboard();
}

void initDeviceKeyboard() {
    keyboard = (uint32_t *)newDeviceMapSpace(4);
    keyboard[0] = KEY_NONE;
#ifdef CONFIG_HAS_PMIO
    addDevicePMIOMap("keyboard",
                     CONFIG_KEYBOARD_PMIO,
                     keyboard,
                     4,
                     handleDeviceKeyboard);
#else
    addDeviceMMIOMap("keyboard",
                     CONFIG_KEYBOARD_MMIO,
                     keyboard,
                     4,
                     handleDeviceKeyboard);
#endif

    initDeviceKeyboardMap();
}

void sendDeviceKeyboard(uint8_t scancode, bool keydown_flag) {
    if (cpu_state.state == CPU_RUNNING && keymap[scancode] != KEY_NONE) {
        uint32_t am_scancode = keymap[scancode] |
                              (keydown_flag ? KEYDOWN_MASK : 0);
        enqueueDeviceKeyboard(am_scancode);
    }
}
