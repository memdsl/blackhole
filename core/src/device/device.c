#include <SDL2/SDL.h>
#include <device/io/map.h>
#include <device/device.h>
#include <device/serial.h>
#include <device/timer.h>
#include <device/keyboard.h>
#include <device/vga.h>
#include <state.h>
#include <util/util.h>

#define TIMER_HZ 60

void clearDeviceEventQueue() {
    SDL_Event event;
    while (SDL_PollEvent(&event));
}

void initDevice() {
    initDeviceMap();

    IFDEF(CONFIG_HAS_SERIAL, initDeviceSerial());
    IFDEF(CONFIG_HAS_TIMER, initDeviceTimer());
    IFDEF(CONFIG_HAS_KEYBOARD, initDeviceKeyboard());
    IFDEF(CONFIG_HAS_VGA, initDeviceVGA());
}

void updateDeviceState() {
    static uint64_t last = 0;
    uint64_t curr = getUtilTimerData();
    if (curr - last < 1000000 / TIMER_HZ) {
        return;
    }
    last = curr;

    IFDEF(CONFIG_HAS_VGA, updateDeviceVGAScreen());

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                cpu_state.state = CPU_QUIT;
                break;
            }
#ifdef CONFIG_HAS_KEYBOARD
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                uint8_t k = event.key.keysym.scancode;
                bool keydown_flag = (event.key.type == SDL_KEYDOWN);
                sendDeviceKeyboard(k, keydown_flag);
                break;
            }
#endif
            default: break;
        }
    }
}
