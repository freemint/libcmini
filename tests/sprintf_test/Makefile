ifneq (yes,$(VERBOSE))
	Q=@
else
	Q=
endif

CC=m68k-atari-mint-gcc
APP=sprinttst.tos

BUILD_FAST=$(shell if $(CC) -mfastcall -E - < /dev/null >/dev/null 2>&1; then echo Y; else echo N; fi)
ifeq ($(BUILD_FAST), Y)
LIBDIR=../../mfastcall
MFASTCALL = -mfastcall
else
LIBDIR=../..
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

ifneq ($(DEVKITMINT),) 
# ardi's toolchain stuff
$(APP) :

%.tos: %.elf
	tostool $< $@

%.elf: $(LIBDIR)/libcmini.a $(OBJS)
	$(CC) $(LIBDIR)/startup.o -o $@ $(OBJS) -Wl,-Map,mapfile $(CFLAGS) $(LIBS)

else

$(APP): $(LIBDIR)/libcmini.a $(OBJS)
	$(Q)echo "CC $(@)"
	$(Q)$(CC) $(LIBDIR)/startup.o -o $(APP) $(OBJS) $(CFLAGS) $(LIBS)
	$(Q)m68k-atari-mint-strip $(APP)

endif
	
clean:
	- $(Q)rm -f $(APP) $(OBJS) 
