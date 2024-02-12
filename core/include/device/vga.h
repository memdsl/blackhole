#ifndef __VGA_H__
#define __VGA_H__

#include <common.h>

#define SCREEN_W (MUXDEF(CONFIG_VGA_SIZE_800x600, 800, 400))
#define SCREEN_H (MUXDEF(CONFIG_VGA_SIZE_800x600, 600, 300))

void initDeviceVGA();
void updateDeviceVGAScreen();

#endif
