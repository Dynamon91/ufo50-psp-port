#include <assert.h>
#include <stdint.h>
#include "scale_rgb565.h"

int main(void)
{
    /* 2x2 source scaled to a 4x4 viewport, using padded row pitches. */
    const uint16_t src[6] = {
        0xF800, 0x07E0, 0xAAAA,
        0x001F, 0xFFFF, 0xBBBB
    };
    uint16_t dst[48];
    Ufo50Viewport v = ufo50_viewport_fit(6, 6, 2, 2);
    int y, x;

    assert(v.x == 0 && v.y == 0 && v.width == 6 && v.height == 6);
    /* Clear destination to detect untouched padding/invalid areas. */
    for (y = 0; y < 48; ++y) dst[y] = 0x1234;

    /* Use a 2x2 logical source and a 6x6 destination, row pitch 4/8. */
    assert(ufo50_scale_rgb565(src, 2, 2, 3, dst, 6, 6, 8, &v) == 1);
    for (y = 0; y < 6; ++y) {
        int sy = y * 2 / 6;
        for (x = 0; x < 6; ++x) {
            int sx = x * 2 / 6;
            assert(dst[y * 8 + x] == src[sy * 3 + sx]);
        }
        assert(dst[y * 8 + 6] == 0x1234 && dst[y * 8 + 7] == 0x1234);
    }

    /* Reject an invalid source pitch and a viewport outside the destination. */
    assert(ufo50_scale_rgb565(src, 2, 2, 1, dst, 6, 6, 8, &v) == 0);
    v.x = 1;
    assert(ufo50_scale_rgb565(src, 2, 2, 3, dst, 6, 6, 8, &v) == 0);
    return 0;
}
