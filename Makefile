# disable verbose builds by default
# If you want to see all the gory details of the build process,
# run "VERBOSE=yes make <target>"

ifndef VERBOSE
  	VERBOSE=no
endif
ifneq (yes,$(VERBOSE))
  	Q=@
else
  	Q=
endif

ifneq ($(DEVKITMINT),)
	export PATH		:=	$(DEVKITMINT)/../devkitMINT/bin:$(PATH)
endif

#
# ONLY_68K: for faster developing; set to Y to build only the 68000 library
# BUILD_CF: Build ColfFire binaries.
# MINTLIB_COMPATIBLE: set to Y to build libcmini with mintlib-includes
#
ONLY_68K=N
BUILD_CF=Y
BUILD_FAST=N
MINTLIB_COMPATIBLE=N
COMPILE_ELF=N
STDIO_WITH_LONG_LONG=N

ifeq (Y,$(COMPILE_ELF))
	PREFIX=m68k-elf-
else 
  	PREFIX=m68k-atari-mint-
endif

-include Make.config

CC=$(PREFIX)gcc
LD=$(PREFIX)ld
CPP=$(PREFIX)cpp
OBJCOPY=$(PREFIX)objcopy
AR=$(PREFIX)ar
RANLIB=$(PREFIX)ranlib

CFLAGS=\
	   -Wall\
	   -Os \
	   -fomit-frame-pointer

ifneq ($(MINTLIB_COMPATIBLE),Y)
	INCLUDE=-Iinclude
else
	CFLAGS+=-D__MINTLIB_COMPATIBLE
endif

ifeq ($(STDIO_WITH_LONG_LONG),Y)
	CFLAGS+=-DSTDIO_WITH_LONG_LONG
endif


STARTUP= \
	$(SRCDIR)/startup.S
	
CSRCS= $(wildcard $(SRCDIR)/*.c)
	
ASRCS= $(wildcard $(SRCDIR)/*.S)

SRCDIR=sources
ifeq ($(ONLY_68K),Y)
	LIBDIRS=.
else
	ifeq ($(BUILD_FAST), Y)
		ifeq ($(BUILD_CF),Y)
			LIBDIRS=. ./m68020-60 ./m5475 ./mshort ./m68020-60/mshort ./m5475/mshort ./mfastcall ./m68020-60/mfastcall ./m5475/mfastcall ./mshort/mfastcall ./m68020-60/mshort/mfastcall ./m5475/mshort/mfastcall
		else
			LIBDIRS=. ./m68020-60 ./mshort ./m68020-60/mshort ./mfastcall ./m68020-60/mfastcall ./mshort/mfastcall ./m68020-60/mshort/mfastcall
		endif
	else
		ifeq ($(BUILD_CF),Y)
			LIBDIRS=. ./m68020-60 ./m5475 ./mshort ./m68020-60/mshort ./m5475/mshort
		else
			LIBDIRS=. ./m68020-60 ./mshort ./m68020-60/mshort
		endif
	endif
endif
OBJDIRS=$(patsubst %,%/objs,$(LIBDIRS))

COBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.S,%.o,$(ASRCS)))
OBJS=$(COBJS) $(AOBJS)

IIO_OBJS = doprnt.o $(filter %printf.o, $(patsubst %,../%,$(OBJS)))

START_OBJ=startup.o
LIBC=libcmini.a
LIBIIO=libiiomini.a

LIBS=$(patsubst %,%/$(LIBC),$(LIBDIRS))
LIBSIIO=$(patsubst %,%/$(LIBIIO),$(LIBDIRS))
STARTUPS=$(patsubst %,%/$(START_OBJ),$(LIBDIRS))

TESTS:= $(shell ls tests | egrep -v '^(CVS)$$')

all: libs startups tests
libs: $(LIBS) $(LIBSIIO)
	
startups: $(STARTUPS)

tests:
	$(Q)echo make tests
	$(Q)for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i || { exit 1;} fi; done;


clean:
	$(Q)rm -rf $(LIBS) $(LIBSIIO) $(patsubst %,%/objs/*.o,$(LIBDIRS)) $(patsubst %,%/objs/*.d,$(LIBDIRS)) $(patsubst %,%/objs/iio,$(LIBDIRS)) $(STARTUPS) $(STARTUPS:.o=.d) depend
	$(Q)for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i clean || { exit 1;} fi; done;
	$(Q) rm -rf libcmini-*

#
# multilib flags
#
m68020-60/%.a m68020-60/startup.o: CFLAGS += -m68020-60
mshort/%.a mshort/startup.o: CFLAGS += -mshort
m68020-60/mshort/%.a m68020-60/mshort/startup.o: CFLAGS += -m68020-60 -mshort
ifeq ($(BUILD_FAST), Y)
mfastcall/%.a mfastcall/startup.o: CFLAGS +=  -mfastcall
m68020-60/mfastcall/%.a m68020-60/mfastcall/startup.o: CFLAGS += -m68020-60 -mfastcall
mshort/mfastcall/%.a mshort/mfastcall/startup.o: CFLAGS += -mshort -mfastcall
m68020-60/mshort/mfastcall/%.a m68020-60/mshort/mfastcall/startup.o: CFLAGS += -m68020-60 -mshort -mfastcall
endif
ifeq ($(BUILD_CF),Y)
m5475/%.a m5475/startup.o: CFLAGS += -mcpu=5475
m5475/mshort/%.a m5475/mshort/startup.o: CFLAGS += -mcpu=5475 -mshort
ifeq ($(BUILD_FAST), Y)
m5475/mfastcall/%.a m5475/mfastcall/startup.o: CFLAGS += -mcpu=5475 -mfastcall
m5475/mshort/mfastcall/%.a m5475/mshort/mfastcall/startup.o: CFLAGS += -mcpu=5475 -mshort -mfastcall
endif
endif

#
# generate pattern rules for multilib object files
#
define CC_TEMPLATE
$(1)/objs/iio/%.o:$(SRCDIR)/iio/%.c
	$(Q)echo "CC $$(@)"
	$(Q)$(CC) -MMD -MP -MF $$(@:.o=.d) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@

$(1)/objs/%.o:$(SRCDIR)/%.c
	$(Q)echo "CC $$(@)"
	$(Q)$(CC) -MMD -MP -MF $$(@:.o=.d) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@

$(1)/objs/%.o:$(SRCDIR)/%.S
	$(Q)echo "CC $$(@)"
	$(Q)$(CC) -MMD -MP -MF $$(@:.o=.d) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@

$(1)/%.o:$(SRCDIR)/%.S
	$(Q)echo "CC $$(@)"
	$(Q)$(CC) -MMD -MP -MF $$(@:.o=.d) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@
endef
$(foreach DIR,$(LIBDIRS),$(eval $(call CC_TEMPLATE,$(DIR))))

#
# generate pattern rules for multilib archive
#
define ARC_TEMPLATE
$(1)_OBJS=$(patsubst %,$(1)/objs/%,$(OBJS))
$(1)/$(LIBC): $$($(1)_OBJS)
	$(Q)echo "AR $$@"
	$(Q)$(AR) cr $$@ $$?
	$(Q)$(RANLIB) $$@
LIBDEPEND+=$$($1_OBJS)
LIBSE+=$(1)/$(LIBC)

$(shell mkdir -p $(1)/objs/iio)
$(1)_IIO_OBJS=$(patsubst %,$(1)/objs/iio/%,$(IIO_OBJS))
$(1)/$(LIBIIO): $$($(1)_IIO_OBJS)
	$(Q)echo "AR $$@"
	$(Q)$(AR) cr $$@ $$?
	$(Q)$(RANLIB) $$@
endef
$(foreach DIR,$(LIBDIRS),$(eval $(call ARC_TEMPLATE,$(DIR))))

.PHONY: release
release: all
	RELEASETAG=$$(git tag --contains | sed -e 's/v//' | sed -e 's/ //g') ;\
    RELEASEDIR=libcmini-$$RELEASETAG ;\
	if [ "x$$RELEASETAG" != "x" ] ; then\
	    mkdir -p $$RELEASEDIR/lib ;\
	    cp -r include $$RELEASEDIR ;\
	    for i in m5475 m68020-60 mshort m5475/mshort m68020-60/mshort; do \
		    mkdir -p $$RELEASEDIR/lib/$$i ;\
	        cp $$i/libcmini.a $$RELEASEDIR/lib/$$i ;\
	        cp $$i/startup.o $$RELEASEDIR/lib/$$i ;\
	    done ;\
	    cp startup.o libcmini.a $$RELEASEDIR/lib ;\
	fi ;\
    chown -R 0:0 $$RELEASEDIR/* ;\
	tar -C $$RELEASEDIR -cvzf $$RELEASEDIR.tar.gz . ;\
    chmod 644 $$RELEASEDIR.tar.gz ;\
	ls -l


DEPENDS := $(foreach dir,$(LIBDIRS), $(wildcard $(dir)/objs/*.d) $(wildcard $(dir)/objs/iio/*.d))

.PHONY: printvars tests
printvars:
	@$(foreach V,$(.VARIABLES),	$(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V))))
		

ifneq (clean,$(MAKECMDGOALS))
-include $(DEPENDS)
endif
