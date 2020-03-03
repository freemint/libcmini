ifneq (yes,$(VERBOSE))
	Q=@
else
	Q=
endif

CC=m68k-atari-mint-gcc
APP=sprinttst.tos

BUILD_FAST=$(shell if $(CC) -mfastcall -E - < /dev/null >/dev/null 2>&1; then echo Y; else echo N; fi)
ifeq ($(BUILD_FAST), Y)
LIBDIR = ../../build/$(shell $(CC) -mfastcall -print-multi-directory)
MFASTCALL = -mfastcall
else
LIBDIR:=../../build/$(shell $(CC) -print-multi-directory)
MFASTCALL =
endif

INCLUDE=-I../../include
LIBS=-nostdlib -L$(LIBDIR) -lcmini -lgcc -lcmini
OBJS=sprintf_test.o
CFLAGS=\
	-mcpu=68000 $(MFASTCALL) \
	-fomit-frame-pointer\
	-Os\
	$(INCLUDE)

all: $(APP)

$(APP): $(LIBDIR)/libcmini.a $(OBJS)
	$(Q)echo "CC $(@)"
	$(Q)$(CC) ../../build/crt0.o -o $(APP) $(OBJS) $(CFLAGS) $(LIBS)
	$(Q)m68k-atari-mint-strip $(APP)

	
clean:
	- $(Q)rm -f $(APP) $(OBJS) 
