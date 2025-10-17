TARGET = testIntraFont
OBJS = main.o


DEFINES := \
	-DPSP \
	-DOSL_IMAGE_LOADER_PNG \
	-DOSL_IMAGE_LOADER_JPEG \
	-DOSL_IMAGE_WRITER_PNG

CFLAGS = -O2 -g -G0 -Wall ${DEFINES}
INCDIR =
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371

LIBDIR =
LDFLAGS =

MYLIBS=
STDLIBS= -lm -losl -lpng -ljpeg -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl
LIBS=$(STDLIBS) $(MYLIBS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = intraFont Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

