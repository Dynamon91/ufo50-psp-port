/*
 * UFO50 PSP port — software RGB565 scaler.
 *
 * Nearest-neighbour scale of a logical RGB565 framebuffer into a viewport
 * calculated by viewport.h. Caller owns both buffers and supplies pitches
 * in pixels (not bytes). This module is portable C99 and PSPSDK-independent.
 */
#ifndef UFO50_SCALE_RGB565_H
#define UFO50_SCALE_RGB565_H

#include <stddef.h>
#include <stdint.h>
#include "viewport.h"

/*
 * Draw source into destination at the viewport's location. The destination
 * is not cleared: caller should clear letterbox bars before calling.
 * Returns 1 on success, 0 for invalid pointers/dimensions/pitches.
 */
static inline int ufo50_scale_rgb565(
    const uint16_t *src, int src_width, int src_height, int src_pitch,
    uint16_t *dst, int dst_width, int dst_height, int dst_pitch,
    const Ufo50Viewport *v)
{
    int y;
    if (!src || !dst || !v || src_width <= 0 || src_height <= 0 ||
        dst_width <= 0 || dst_height <= 0 || src_pitch < src_width ||
        dst_pitch < dst_width || v->width <= 0 || v->height <= 0 ||
        v->x < 0 || v->y < 0 || v->x + v->width > dst_width ||
        v->y + v->height > dst_height ||
        v->logical_width != src_width || v->logical_height != src_height)
        return 0;

    for (y = 0; y < v->height; ++y) {
        int sy = (int)((long long)y * src_height / v->height);
        int x;
        const uint16_t *src_row = src + (size_t)sy * (size_t)src_pitch;
        uint16_t *dst_row = dst + (size_t)(v->y + y) * (size_t)dst_pitch + v->x;
        for (x = 0; x < v->width; ++x) {
            int sx = (int)((long long)x * src_width / v->width);
            dst_row[x] = src_row[sx];
        }
    }
    return 1;
}

#endif
