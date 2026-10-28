# ufo50-psp-port

Work-in-progress native PSP port framework for UFO 50.

## Native rendering foundations

- `native/viewport.h`: computes an aspect-preserving viewport and maps screen coordinates back to logical coordinates. For 384x216 on the PSP's 480x272 display, it yields 480x270 at offset (0, 1).
- `native/scale_rgb565.h`: portable C99 nearest-neighbour scaler for RGB565 framebuffer data. Source and destination pitches are expressed in pixels.
- `native/psp_framebuffer.h`: framebuffer presentation boundary that clears the visible screen to black and scales into a fitted viewport. It accepts the actual framebuffer pitch (often 512 pixels for a 480-pixel visible PSP screen) and has no PSPSDK dependency; the PSP runtime must still supply the correct buffer and present/swap it.
- `native/test_viewport.c`, `native/test_scale_rgb565.c`, and `native/test_psp_framebuffer.c`: regression tests for viewport geometry, scaling, row pitches, black letterbox bars, and invalid arguments.
- `.github/workflows/viewport-test.yml`: compiles and executes the three native regression tests on pushes and pull requests affecting native code, then repeats them with UndefinedBehaviorSanitizer (UBSan).

Run locally from the repository root:

```sh
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_viewport.c -o /tmp/test_viewport
/tmp/test_viewport
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_scale_rgb565.c -o /tmp/test_scale_rgb565
/tmp/test_scale_rgb565
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_psp_framebuffer.c -o /tmp/test_psp_framebuffer
/tmp/test_psp_framebuffer
```

## Integration status

These are standalone rendering foundations and are **not yet wired into a game runtime or the PSP display-list/framebuffer path**. The new presentation helper is an explicit framebuffer boundary, but it does not acquire PSP VRAM, configure the GU, flush caches, or swap/display buffers; those require the actual runtime integration. No EBOOT.PBP build, PPSSPP test, or hardware test is claimed. This repository currently does not establish that any of the 50 games are ported. The next integration step is to connect the scaler to the actual PSP framebuffer/render loop and confirm its pixel format and buffer pitch.
