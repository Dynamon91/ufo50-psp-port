/*
 * UFO50 PSP port — PSPDEV framebuffer smoke test.
 *
 * This is an executable hardware-path probe, not the UFO 50 game runtime.
 * It initializes the PSP display and paints a simple RGB565 test pattern.
 */
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <stdint.h>
#include "psp_framebuffer.h"

PSP_MODULE_INFO("UFO50 PSP Render Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define SCREEN_W 480
#define SCREEN_H 272
#define FRAME_PITCH 512
#define LOGICAL_W 384
#define LOGICAL_H 216

static uint16_t logical[LOGICAL_W * LOGICAL_H];

static int exit_callback(int arg1, int arg2, void *common)
{
    (void)arg1; (void)arg2; (void)common;
    sceKernelExitGame();
    return 0;
}

static int callback_thread(SceSize args, void *argp)
{
    (void)args; (void)argp;
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

static void setup_callbacks(void)
{
    int thid = sceKernelCreateThread("callback_thread", callback_thread,
                                     0x11, 0xFA0, PSP_THREAD_ATTR_USER, NULL);
    if (thid >= 0) sceKernelStartThread(thid, 0, NULL);
}

int main(void)
{
    int x, y;
    uint16_t *framebuffer;

    setup_callbacks();
    sceDisplaySetMode(0, SCREEN_W, SCREEN_H);

    /* PSPSDK allocates a suitably aligned RGB565 buffer in video RAM. */
    framebuffer = (uint16_t *)sceGuGetStaticVramBuffer(
        SCREEN_W, SCREEN_H, GU_PSM_5650);
    if (!framebuffer) {
        sceKernelSleepThread();
        return 1;
    }

    for (y = 0; y < LOGICAL_H; ++y) {
        for (x = 0; x < LOGICAL_W; ++x) {
            unsigned int r = (unsigned int)(x * 31 / (LOGICAL_W - 1));
            unsigned int g = (unsigned int)(y * 63 / (LOGICAL_H - 1));
            unsigned int b = (unsigned int)((x + y) * 31 /
                                           (LOGICAL_W + LOGICAL_H - 2));
            logical[y * LOGICAL_W + x] =
                (uint16_t)((r << 11) | (g << 5) | b);
        }
    }

    {
        Ufo50Viewport viewport =
            ufo50_viewport_fit(SCREEN_W, SCREEN_H, LOGICAL_W, LOGICAL_H);
        if (!ufo50_present_rgb565(logical, LOGICAL_W, LOGICAL_H, LOGICAL_W,
                                  framebuffer, SCREEN_W, SCREEN_H, FRAME_PITCH,
                                  &viewport)) {
            sceKernelSleepThread();
            return 2;
        }
    }

    sceKernelDcacheWritebackAll();
    sceDisplaySetFrameBuf(framebuffer, FRAME_PITCH, PSP_DISPLAY_PIXEL_FORMAT_565,
                          PSP_DISPLAY_SETBUF_IMMEDIATE);
    sceDisplayWaitVblankStart();

    /* Keep the rendered pattern visible until the user exits the application. */
    sceKernelSleepThread();
    return 0;
}
