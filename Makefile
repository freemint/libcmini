# disable verbose builds by default
# If you want to see all the gory details of the build process,
# run "VERBOSE=yes make <target>"

ifndef VERBOSE
  	VERBOSE=no
endif
ifeq (,$(filter $(VERBOSE),Y yes))
  	Q=@
else
  	Q=
endif

ifneq ($(DEVKITMINT),)
	export PATH		:=	$(DEVKITMINT)/../devkitMINT/bin:$(PATH)
endif

all:$(patsubst %,%/$(APP),$(TRGTDIRS))

#
# ONLY_68K: for faster developing; set to Y to build only the 68000 library
# BUILD_CF: Build ColfFire binaries.
# MINTLIB_COMPATIBLE: set to Y to build libcmini with mintlib-includes
#
ONLY_68K=N
BUILD_CF=Y
BUILD_FAST=Y
BUILD_SOFT_FLOAT=Y
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

ifeq (,$(filter $(MINTLIB_COMPATIBLE),Y yes))
	INCLUDE=-Iinclude
else
	INCLUDE=
	CFLAGS+=-D__MINTLIB_COMPATIBLE
endif

ifneq (,$(filter $(STDIO_WITH_LONG_LONG),Y yes))
	CFLAGS+=-DSTDIO_WITH_LONG_LONG
endif


STARTUP= \
	$(SRCDIR)/startup.S
	
CSRCS= $(wildcard $(SRCDIR)/*.c)
	
ASRCS= $(filter-out $(STARTUP),$(wildcard $(SRCDIR)/*.S))

SRCDIR=sources
ifneq (,$(filter $(ONLY_68K),Y yes))
	# asume a multi-lib without flags ar m68000
	# NOTE \s?$ is important - gcc on Windows outputs \r\n-lineendings but MSYS grep only accept \n -> \s matchs \r
	LIBDIRS := $(shell $(CC) -print-multi-lib | grep -E ';\s?$$' | sed -e "s/;.*//")
else
	LIBDIRS := $(shell $(CC) -print-multi-lib | sed -e "s/;.*//")
	ifeq (,$(filter $(BUILD_FAST),Y yes))
		MULTILIB := $(shell echo $(MULTILIB) | sed -e 's/\S*fastcall\S*/ /g')
	endif
	ifeq (,$(filter $(BUILD_CF),Y yes))
		MULTILIB := $(shell echo $(MULTILIB) | sed -e 's/\S*m5475\S*/ /g')
	endif
	ifeq (,$(filter $(BUILD_SOFT_FLOAT),Y yes))
		MULTILIB := $(shell echo $(MULTILIB) | sed -e 's/\S*soft-float\S*/ /g')
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

TESTS:= $(shell ls tests | grep -E -v '^(CVS)$$')

all: dirs libs startups tests
libs: $(LIBS) $(LIBSIIO)
	
dirs::
	$(Q)mkdir -p $(LIBDIRS) $(OBJDIRS)


startups: $(STARTUPS)

tests:
	$(Q)echo make tests
	$(Q)for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i || { exit 1;} fi; done;


clean:
	$(Q)rm -rf $(LIBS) $(LIBSIIO) $(patsubst %,%/objs/*.o,$(LIBDIRS)) $(patsubst %,%/objs/*.d,$(LIBDIRS)) $(patsubst %,%/objs/iio,$(LIBDIRS)) $(STARTUPS) $(STARTUPS:.o=.d) depend
	$(Q)for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i clean || { exit 1;} fi; done;
	$(Q) rm -rf libcmini-*

all:$(patsubst %,%/$(APP),$(TRGTDIRS))

#
# multilib flags
#
MULTIFLAGS = $(shell $(CC) -print-multi-lib | grep '^$(1);' | sed -e 's/^.*;//' -e 's/@/ -/g')
define MULTIFLAG_TEMPLATE
$(1)/%.a $(1)/startup.o: CFLAGS += $(call MULTIFLAGS,$(1))
endef
$(foreach DIR,$(LIBDIRS),$(eval $(call MULTIFLAG_TEMPLATE,$(DIR))))

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
		chown -R 0:0 $$RELEASEDIR/* ;\
		tar -C $$RELEASEDIR -cvzf $$RELEASEDIR.tar.gz . ;\
	    chmod 644 $$RELEASEDIR.tar.gz ;\
	fi ;\
	ls -l


DEPENDS := $(foreach dir,$(LIBDIRS), $(wildcard $(dir)/objs/*.d) $(wildcard $(dir)/objs/iio/*.d))

.PHONY: printvars tests
printvars:
	@$(foreach V,$(.VARIABLES),	$(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V))))
		
install::
	for i in $(LIBDIRS); do \
		mkdir -p $(DESTDIR)/usr/lib/$$i; \
		cp -a $$i/$(LIBC) $$i/$(LIBIIO) $(DESTDIR)/usr/lib/$$i; \
	done
	cp -a startup.o $(DESTDIR)/usr/lib

ifneq (clean,$(MAKECMDGOALS))
-include $(DEPENDS)
endif
