
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
	
ifneq ($(DEVKITMINT),) 
# ardi's toolchain stuff
$(APP) :

%.tos: %.elf
	tostool $< $@

%.elf: ../../libcmini.a $(OBJS)
	$(CC) ../../startup.o -o $@ $(OBJS) -Wl,-Map,mapfile $(CFLAGS) $(LIBS)

else

$(APP): ../../libcmini.a $(OBJS)
	$(CC) ../../startup.o -o $(APP) $(OBJS) $(CFLAGS) $(LIBS)
	m68k-atari-mint-strip $(APP)

endif
	
clean:
	- rm -f $(APP) $(OBJS) 
