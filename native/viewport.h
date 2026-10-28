/*
 * UFO50 PSP port — logical viewport helper.
 *
 * Fits a 384x216 logical framebuffer into the PSP's 480x272 display
 * without distorting the 16:9 image. Integer coordinates are used for
 * stable edges; mouse/pointer coordinates can be mapped back to logical
 * space with the inverse helper below.
 *
 * This header is standalone and does not depend on PSPSDK.
 */
#ifndef UFO50_VIEWPORT_H
#define UFO50_VIEWPORT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x, y;
    int width, height;
    int logical_width, logical_height;
} Ufo50Viewport;

static inline int ufo50_min_int(int a, int b) { return a < b ? a : b; }

static inline Ufo50Viewport ufo50_viewport_fit(
    int screen_width, int screen_height,
    int logical_width, int logical_height)
{
    Ufo50Viewport v = {0, 0, 0, 0, logical_width, logical_height};
    if (screen_width <= 0 || screen_height <= 0 ||
        logical_width <= 0 || logical_height <= 0) return v;

    /* Compare aspect ratios using integer math to avoid float drift. */
    if ((long long)screen_width * logical_height <=
        (long long)screen_height * logical_width) {
        v.width = screen_width;
        v.height = (int)((long long)screen_width * logical_height / logical_width);
    } else {
        v.height = screen_height;
        v.width = (int)((long long)screen_height * logical_width / logical_height);
    }
    if (v.width < 1) v.width = 1;
    if (v.height < 1) v.height = 1;
    v.x = (screen_width - v.width) / 2;
    v.y = (screen_height - v.height) / 2;
    return v;
}

/* Return 1 if the screen point lies inside the viewport; otherwise 0. */
static inline int ufo50_viewport_to_logical(
    const Ufo50Viewport *v, int screen_x, int screen_y,
    int *logical_x, int *logical_y)
{
    if (!v || v->width <= 0 || v->height <= 0 ||
        screen_x < v->x || screen_y < v->y ||
        screen_x >= v->x + v->width ||
        screen_y >= v->y + v->height) return 0;

    if (logical_x)
        *logical_x = (int)((long long)(screen_x - v->x) * v->logical_width / v->width);
    if (logical_y)
        *logical_y = (int)((long long)(screen_y - v->y) * v->logical_height / v->height);
    return 1;
}

#ifdef __cplusplus
}
#endif
#endif
