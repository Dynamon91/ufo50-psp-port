TARGET = ufo50_render_test
OBJS = main.o
INCDIR = ../native
CFLAGS = -O2 -G0 -Wall -Wextra -std=c99
CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)
LIBS = -lpspgu -lpspdisplay
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = UFO50 PSP Renderer Test
PSP_EBOOT_ICON =

include $(PSPSDK)/lib/build.mak
