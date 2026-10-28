# ufo50-psp-port

Work-in-progress native PSP port framework for UFO 50.

## Viewport helper

`native/viewport.h` provides a standalone, PSPSDK-independent helper for
fitting a 384x216 logical image into the PSP's 480x272 display while
preserving aspect ratio. For this pair of dimensions, the viewport is
480x270 at offset (0, 1), leaving one letterbox row above and below.
`ufo50_viewport_to_logical()` rejects points in those bars and maps points
inside the viewport back to logical coordinates.

A small regression test is in `native/test_viewport.c`, with a GitHub Actions workflow at `.github/workflows/viewport-test.yml` that compiles and runs it on pushes and pull requests affecting these files. To run it locally with a C compiler,
run it from the repository root:

```sh
cc -std=c99 -Wall -Wextra -Werror -Inative native/test_viewport.c -o /tmp/test_viewport
/tmp/test_viewport
```

**Integration status:** this helper is not yet wired into the renderer or
input path. This repository does not yet document a verified EBOOT.PBP build
or PPSSPP/hardware test. The helper is a small foundation step, not evidence
that any of the 50 games have been ported.
