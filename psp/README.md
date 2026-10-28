# PSP renderer smoke test

This directory contains a minimal PSPSDK executable to validate the real PSP display path independently of the UFO 50 runtime.

## Build

Install the PSPDEV toolchain / PSPSDK as documented at https://pspdev.github.io/ and run:

```sh
cd psp
make
```

The expected output is `psp/EBOOT.PBP`. Copy it to a homebrew folder on a PSP or load it in PPSSPP.

## What it tests

- Initializes the PSP display mode at 480x272.
- Allocates an RGB565 video-memory framebuffer through PSPSDK.
- Generates a 384x216 gradient test image.
- Uses the shared viewport and framebuffer presentation helper.
- Flushes the data cache and submits the framebuffer for display.

This is only a renderer smoke test. It does not run UFO 50, implement game logic, audio, input, or any of the 50 games. The PSPDEV toolchain is required to verify compilation; a successful GitHub file upload is not evidence that an EBOOT was built or tested.
