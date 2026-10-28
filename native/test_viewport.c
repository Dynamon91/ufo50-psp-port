#include <assert.h>
#include "viewport.h"

int main(void)
{
    Ufo50Viewport v = ufo50_viewport_fit(480, 272, 384, 216);
    assert(v.x == 0 && v.y == 1);
    assert(v.width == 480 && v.height == 270);

    int x = -1, y = -1;
    assert(ufo50_viewport_to_logical(&v, 0, 1, &x, &y) == 1);
    assert(x == 0 && y == 0);

    /* The two letterbox rows must not map to game coordinates. */
    assert(ufo50_viewport_to_logical(&v, 200, 0, &x, &y) == 0);
    assert(ufo50_viewport_to_logical(&v, 200, 271, &x, &y) == 0);

    /* Right/bottom edges are exclusive, including the final visible pixel. */
    assert(ufo50_viewport_to_logical(&v, 479, 270, &x, &y) == 1);
    assert(x == 383 && y == 215);
    assert(ufo50_viewport_to_logical(&v, 480, 100, &x, &y) == 0);

    /* A portrait-shaped logical image is fitted without cropping. */
    v = ufo50_viewport_fit(480, 272, 240, 320);
    assert(v.width == 204 && v.height == 272);
    assert(v.x == 138 && v.y == 0);
    assert(ufo50_viewport_to_logical(&v, 137, 100, &x, &y) == 0);
    assert(ufo50_viewport_to_logical(&v, 138, 0, &x, &y) == 1);
    assert(x == 0 && y == 0);

    /* Invalid dimensions return an empty viewport and safe inverse mapping. */
    v = ufo50_viewport_fit(0, 272, 384, 216);
    assert(v.width == 0 && v.height == 0);
    assert(ufo50_viewport_to_logical(&v, 0, 0, &x, &y) == 0);

    return 0;
}
