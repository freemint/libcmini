ifneq (yes,$(VERBOSE))
	Q=@
else
	Q=
endif

CC=m68k-atari-mint-gcc
APP=sprinttst.tos

INCLUDE=-I../../include
LIBS=-nostdlib -L../../ -lcmini -lgcc -lcmini
OBJS=sprintf_test.o
CFLAGS=\
	-mcpu=68000\
	-fomit-frame-pointer\
	-Os\
	$(INCLUDE)

all: $(APP)

ifneq ($(DEVKITMINT),) 
# ardi's toolchain stuff
$(APP) :

%.tos: %.elf
	tostool $< $@

%.elf: ../../libcmini.a $(OBJS)
	$(CC) ../../startup.o -o $@ $(OBJS) -Wl,-Map,mapfile $(CFLAGS) $(LIBS)

else

$(APP): ../../libcmini.a $(OBJS)
	$(Q)echo "CC $(@)"
	$(Q)$(CC) ../../startup.o -o $(APP) $(OBJS) $(CFLAGS) $(LIBS)
	$(Q)m68k-atari-mint-strip $(APP)

endif
	
clean:
	- $(Q)rm -f $(APP) $(OBJS) 
