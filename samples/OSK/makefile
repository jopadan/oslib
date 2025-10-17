TARGET = osk
OBJS = main.o

#To build for custom firmware:
BUILD_PRX = 1
PSP_FW_VERSION=371

CFLAGS = -O2 -g -G0 -Wall -DOSL_IMAGE_LOADER_PNG -DOSL_IMAGE_WRITER_PNG -DOSL_IMAGE_LOADER_JPEG -DOSL_IMAGE_LOADER_GIF
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

MYLIBS=
STDLIBS= -lm -losl -lpng -ljpeg -lgif -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl
LIBS=$(STDLIBS) $(MYLIBS)

LDFLAGS =
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = OSK test
#PSP_EBOOT_ICON = ICON0.PNG
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
