/*
 * UFO50 PSP port — RGB565 framebuffer presentation boundary.
 *
 * Clears the visible framebuffer to black (including letterbox bars), then
 * scales a logical RGB565 image into the supplied viewport. The caller must
 * provide a valid linear framebuffer and its pitch in pixels; for PSP GU
 * buffers this pitch is commonly 512 even though the visible width is 480.
 *
 * PSPSDK-independent so the exact memory operation can be regression-tested.
 */
#ifndef UFO50_PSP_FRAMEBUFFER_H
#define UFO50_PSP_FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include "scale_rgb565.h"

static inline int ufo50_present_rgb565(
    const uint16_t *src, int src_width, int src_height, int src_pitch,
    uint16_t *framebuffer, int screen_width, int screen_height, int framebuffer_pitch,
    const Ufo50Viewport *viewport)
{
    int y;

    if (!src || !framebuffer || !viewport ||
        src_width <= 0 || src_height <= 0 || src_pitch < src_width ||
        screen_width <= 0 || screen_height <= 0 ||
        framebuffer_pitch < screen_width ||
        viewport->width <= 0 || viewport->height <= 0 ||
        viewport->x < 0 || viewport->y < 0 ||
        viewport->x + viewport->width > screen_width ||
        viewport->y + viewport->height > screen_height ||
        viewport->logical_width != src_width ||
        viewport->logical_height != src_height)
        return 0;

    for (y = 0; y < screen_height; ++y) {
        uint16_t *row = framebuffer + (size_t)y * (size_t)framebuffer_pitch;
        int x;
        for (x = 0; x < screen_width; ++x)
            row[x] = 0x0000; /* RGB565 black, including letterbox bars. */
    }

    return ufo50_scale_rgb565(src, src_width, src_height, src_pitch,
                              framebuffer, screen_width, screen_height,
                              framebuffer_pitch, viewport);
}

#endif
