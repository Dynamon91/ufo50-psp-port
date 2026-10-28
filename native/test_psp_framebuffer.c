#include <assert.h>
#include <stdint.h>
#include "psp_framebuffer.h"

int main(void)
{
    /* 4x2 source, scaled to 8x4 and vertically letterboxed on an 8x8 screen. */
    const uint16_t src[8] = {
        0xF800, 0x07E0, 0x001F, 0xFFFF,
        0x0000, 0xFFFF, 0x07E0, 0xF800
    };
    uint16_t fb[80]; /* 8 rows, pitch 10: two padding pixels per row. */
    Ufo50Viewport v = ufo50_viewport_fit(8, 8, 4, 2);
    int x, y;

    assert(v.x == 0 && v.y == 2 && v.width == 8 && v.height == 4);
    for (y = 0; y < 80; ++y) fb[y] = 0x1234;

    assert(ufo50_present_rgb565(src, 4, 2, 4, fb, 8, 8, 10, &v) == 1);

    /* The top and bottom letterbox bars must be black. */
    for (y = 0; y < 2; ++y)
        for (x = 0; x < 8; ++x)
            assert(fb[y * 10 + x] == 0x0000);
    for (y = 6; y < 8; ++y)
        for (x = 0; x < 8; ++x)
            assert(fb[y * 10 + x] == 0x0000);

    /* Nearest-neighbour output: each source pixel spans 2x2 destination pixels. */
    for (y = 0; y < 4; ++y) {
        int sy = y / 2;
        for (x = 0; x < 8; ++x) {
            int sx = x / 2;
            assert(fb[(y + 2) * 10 + x] == src[sy * 4 + sx]);
        }
    }

    /* The two pitch-padding pixels on every row are outside the visible image. */
    for (y = 0; y < 8; ++y)
        assert(fb[y * 10 + 8] == 0x1234 && fb[y * 10 + 9] == 0x1234);

    assert(ufo50_present_rgb565(src, 4, 2, 3, fb, 8, 8, 10, &v) == 0);
    return 0;
}
