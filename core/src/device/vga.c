#include <device/io/map.h>
#include <device/io/pmio.h>
#include <device/io/mmio.h>
#include <device/vga.h>
#include <memory/memory.h>

static uint32_t getDeviceVGAScreenWidth() {
    return SCREEN_W;
}

static uint32_t getDeviceVGAScreenHeight() {
    return SCREEN_H;
}

static uint32_t getDeviceVGAScreenSize() {
    return getDeviceVGAScreenWidth() * getDeviceVGAScreenHeight() *
           sizeof(uint32_t);
}

// static uint64_t  vga = NULL;
// static uint32_t *vga_mem[SCREEN_W * SCREEN_H];
static uint32_t *vga_ctl = NULL;
static void *vga_mem = NULL;

#ifdef CONFIG_VGA_SHOW_SCREEN
#include <SDL2/SDL.h>

static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;

static void initDeviceVGAScreen() {
    SDL_Window *window = NULL;
    char title[128];
    sprintf(title, "%s-CPU", str(CFLAGS_CPU_TYPE));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(
        SCREEN_W * (MUXDEF(CONFIG_VGA_SIZE_400x300, 2, 1)),
        SCREEN_H * (MUXDEF(CONFIG_VGA_SIZE_400x300, 2, 1)),
        0,
        &window,
        &renderer);
    SDL_SetWindowTitle(window, title);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        SCREEN_W,
        SCREEN_H);
    SDL_RenderPresent(renderer);
}

static void updateDeviceVGAScreenStep() {
    SDL_UpdateTexture(texture, NULL, vga_mem, SCREEN_W * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
#endif

void initDeviceVGA() {
    vga_ctl = (uint32_t *)newDeviceMapSpace(8);
    vga_ctl[0] = (getDeviceVGAScreenWidth() << 16) | getDeviceVGAScreenHeight();
#ifdef CONFIG_HAS_PMIO
    addDevicePMIOMap("vga_ctl ",
                     CONFIG_VGA_CTL_PMIO,
                     vga_ctl,
                     8,
                     NULL);
#else
    addDeviceMMIOMap("vga_ctl ",
                     CONFIG_VGA_CTL_MMIO,
                     vga_ctl,
                     8,
                     NULL);
#endif

    vga_mem = newDeviceMapSpace(getDeviceVGAScreenSize());
    addDeviceMMIOMap("vga_mem ",
                     CONFIG_VGA_FB_ADDR,
                     vga_mem,
                     getDeviceVGAScreenSize(),
                     NULL);

    IFDEF(CONFIG_VGA_SHOW_SCREEN, initDeviceVGAScreen());
    IFDEF(CONFIG_VGA_SHOW_SCREEN, memset(vga_mem, 0, getDeviceVGAScreenSize()));
}

void updateDeviceVGAScreen() {
#ifdef CONFIG_VGA_SHOW_SCREEN
    printf("%x: %d\n", CONFIG_VGA_CTL_MMIO + 4, readMemoryPhyData(CONFIG_VGA_CTL_MMIO + 4, 4));
    // if (readMemoryPhyData(CONFIG_VGA_CTL_MMIO + 4, 4) != 0) {
        updateDeviceVGAScreenStep();
    //     writeMemoryPhyData(CONFIG_VGA_CTL_MMIO + 4, 4, 0);
    // }
#endif
}
