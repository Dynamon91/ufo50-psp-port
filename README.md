# ufo50-psp-port

Work-in-progress native PSP port framework for UFO 50.

## Native rendering foundations

- `native/viewport.h`: computes an aspect-preserving viewport and maps screen coordinates back to logical coordinates. For 384x216 on the PSP's 480x272 display, it yields 480x270 at offset (0, 1).
- `native/scale_rgb565.h`: portable C99 nearest-neighbour scaler for RGB565 framebuffer data. Source and destination pitches are expressed in pixels; the caller clears the destination first if letterbox bars must be black.
- `native/test_viewport.c` and `native/test_scale_rgb565.c`: regression tests for viewport geometry, letterbox rejection, scaling, row pitches, and invalid arguments.
- `.github/workflows/viewport-test.yml`: compiles both tests with `-Wall -Wextra -Werror` and executes them on pushes and pull requests affecting native code.

Run locally from the repository root:

```sh
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_viewport.c -o /tmp/test_viewport
/tmp/test_viewport
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_scale_rgb565.c -o /tmp/test_scale_rgb565
/tmp/test_scale_rgb565
```

## Integration status

These are standalone rendering foundations and are **not yet wired into a game runtime or the PSP display-list/framebuffer path**. No EBOOT.PBP build, PPSSPP test, or hardware test is claimed. This repository currently does not establish that any of the 50 games are ported. The next integration step is to connect the scaler to the actual PSP framebuffer/render loop and confirm its pixel format and buffer pitch.
