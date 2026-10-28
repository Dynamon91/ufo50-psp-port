# ufo50-psp-port

Work-in-progress native PSP port framework for UFO 50.

## Native rendering foundations

- `native/viewport.h`: computes an aspect-preserving viewport and maps screen coordinates back to logical coordinates. For 384x216 on the PSP's 480x272 display, it yields 480x270 at offset (0, 1).
- `native/scale_rgb565.h`: portable C99 nearest-neighbour scaler for RGB565 framebuffer data. Source and destination pitches are expressed in pixels.
- `native/psp_framebuffer.h`: framebuffer presentation boundary that clears the visible screen to black and scales into a fitted viewport. It accepts the actual framebuffer pitch (often 512 pixels for a 480-pixel visible PSP screen) and has no PSPSDK dependency.
- `native/test_viewport.c`, `native/test_scale_rgb565.c`, and `native/test_psp_framebuffer.c`: regression tests for viewport geometry, scaling, row pitches, black letterbox bars, and invalid arguments.
- `.github/workflows/viewport-test.yml`: compiles and executes the three native regression tests on pushes and pull requests affecting native code, including UndefinedBehaviorSanitizer (UBSan) runs.

Run locally from the repository root:

```sh
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_viewport.c -o /tmp/test_viewport
/tmp/test_viewport
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_scale_rgb565.c -o /tmp/test_scale_rgb565
/tmp/test_scale_rgb565
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_psp_framebuffer.c -o /tmp/test_psp_framebuffer
/tmp/test_psp_framebuffer
```

## PSP framebuffer smoke-test target

The `psp/` directory contains a minimal PSPSDK application that initializes the PSP display, allocates an RGB565 VRAM framebuffer, generates a 384x216 gradient, flushes the data cache, and submits the framebuffer for display. The Makefile now resolves the SDK's `build.mak` path through `psp-config`. Build with an installed PSPDEV toolchain using `cd psp && make`; the expected output is `psp/EBOOT.PBP`.

The GitHub Actions workflow builds this target in the official PSPDEV container and uploads the EBOOT as an artifact only if the build succeeds. Check [Actions](https://github.com/Dynamon91/ufo50-psp-port/actions) for the current result. A failed build is not a generated EBOOT, and compilation alone is not proof that the program runs correctly in PPSSPP or on a PSP.

This target is a graphics-path smoke test only, not the UFO 50 game port.

## Milestones and current scope

- [x] Portable viewport geometry and input-coordinate mapping.
- [x] RGB565 scaling and framebuffer presentation helpers.
- [x] Native regression tests and CI, including UBSan.
- [x] PSP display-path smoke-test source and automated PSPSDK build workflow.
- [ ] Confirm a successful `EBOOT.PBP` build and retain the CI artifact.
- [ ] Run the smoke test in PPSSPP and/or on PSP hardware and verify the displayed gradient/letterboxing.
- [ ] Recover and audit the actual UFO 50 runtime/source assets and their licensing/redistribution constraints.
- [ ] Integrate a real game runtime, input, audio/music, asset loading, save data, and a launcher/catalog for all 50 games.
- [ ] Test and track each game individually; no game is currently proven ported by this repository.

The renderer helpers are not a game engine and are not yet wired into a UFO 50 runtime. No claim is made that all 50 games, or any complete game, are currently playable on PSP.