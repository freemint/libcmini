# disable verbose builds by default
# If you want to see all the gory details of the build process,
# run "VERBOSE=yes make <target>"

ifndef VERBOSE
  	VERBOSE=no
endif
ifeq (,$(filter $(VERBOSE),Y yes))
  	Q=@
	v=
else
  	Q=
	v=v
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
BUILD_SHORT=Y
MINTLIB_COMPATIBLE=N
COMPILE_ELF=N
STDIO_WITH_LONG_LONG=N

ifneq (,$(filter $(COMPILE_ELF),Y yes))
	CROSSPREFIX=m68k-elf-
else 
  	CROSSPREFIX=m68k-atari-mint-
endif

CFLAGS=\
	   -Wall\
	   -Os \
	   -fomit-frame-pointer

-include Make.config
-include Make.config.local

CC=$(CROSSPREFIX)gcc
LD=$(CROSSPREFIX)ld
CPP=$(CROSSPREFIX)cpp
OBJCOPY=$(CROSSPREFIX)objcopy
AR=$(CROSSPREFIX)ar
RANLIB=$(CROSSPREFIX)ranlib

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

BUILDDIR=build

ifneq (,$(filter $(ONLY_68K),Y yes))
	# asume a multi-lib without flags ar m68000
	# NOTE \s?$ is important - gcc on Windows outputs \r\n-lineendings but MSYS's grep only accept \n -> \s eats \r
	MULTILIBDIRS := $(shell $(CC) -print-multi-lib | grep -E ';\s?$$' | sed -e "s/;.*//")
else
	MULTILIBDIRS := $(shell $(CC) -print-multi-lib | sed -e "s/;.*//")
	ifeq (,$(filter $(BUILD_FAST),Y yes))
		MULTILIBDIRS := $(shell echo $(MULTILIBDIRS) | sed -e 's/\S*fastcall\S*/ /g')
	endif
	ifeq (,$(filter $(BUILD_CF),Y yes))
		MULTILIBDIRS := $(shell echo $(MULTILIBDIRS) | sed -e 's/\S*m5475\S*/ /g')
	endif
	ifeq (,$(filter $(BUILD_SOFT_FLOAT),Y yes))
		MULTILIBDIRS := $(shell echo $(MULTILIBDIRS) | sed -e 's/\S*soft-float\S*/ /g')
	endif
	ifeq (,$(filter $(BUILD_SHORT),Y yes))
		MULTILIBDIRS := $(shell echo $(MULTILIBDIRS) | sed -e 's/\S*short\S*/ /g')
	endif
endif
MULTILIBFLAGS = $(shell $(CC) -print-multi-lib | grep '^$(1);' | sed -e 's/^.*;//' -e 's/@/ -/g')

LIBDIRS=$(patsubst %,$(BUILDDIR)/%,$(MULTILIBDIRS))
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

ifeq (,$(filter $(ONLY_68K),Y yes))
tests:
	$(Q)echo make tests
	$(Q)for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i || { exit 1;} fi; done;
else
tests:
endif


clean:
	@rm -r$(v) $(patsubst %,$(BUILDDIR)/%,$(shell ls $(BUILDDIR)))
	@for i in $(TESTS); do if test -e tests/$$i/Makefile ; then $(MAKE) -C tests/$$i clean || { exit 1;} fi; done;

all:$(patsubst %,%/$(APP),$(TRGTDIRS))

#
# multilib flags
#
define MULTILIBFLAGS_TEMPLATE
$(BUILDDIR)/$(1)/%.a $(BUILDDIR)/$(1)/startup.o : CFLAGS += $(call MULTILIBFLAGS,$(1))
endef
$(foreach DIR,$(MULTILIBDIRS),$(eval $(call MULTILIBFLAGS_TEMPLATE,$(DIR))))

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
	    for i in $(MULTILIBDIRS); do \
		    mkdir -p $$RELEASEDIR/lib/$$i ;\
	        cp $(BUILDDIR)/$$i/libcmini.a $$RELEASEDIR/lib/$$i ;\
	        cp $(BUILDDIR)/$$i/startup.o $$RELEASEDIR/lib/$$i ;\
	    done ;\
		chown -R 0:0 $$RELEASEDIR/* ;\
		tar -C $$RELEASEDIR -cvzf $$RELEASEDIR.tar.gz . ;\
	    chmod 644 $$RELEASEDIR.tar.gz ;\
	fi ;\
	ls -l


DEPENDS := $(foreach dir,$(LIBDIRS), $(wildcard $(dir)/objs/*.d) $(wildcard $(dir)/objs/iio/*.d))

.PHONY: printvars tests
printvars:
	@$(foreach V,$(.VARIABLES),	$(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V))))

install: all

PREFIX =
PREFIX_FOR_INCLUDE =
PREFIX_FOR_LIB =
PREFIX_FOR_STARTUP =

ifneq (,$(PREFIX)$(PREFIX_FOR_INCLUDE)$(PREFIX_FOR_LIB)$(PREFIX_FOR_STARTUP))
	ifneq (,$(DESTDIR))
		ERR := $(error Options DESTDIR and PREFIX[_FOR_(INCLUDE|LIB|STARTUP)] are mutually incompatible. Use either DESTDIR or PREFIX...)
	endif
	ifneq (,$(PREFIX))
		ifeq (,$(PREFIX_FOR_INCLUDE))
			PREFIX_FOR_INCLUDE = $(PREFIX)/include
		endif
		ifeq (,$(PREFIX_FOR_LIB))
			PREFIX_FOR_LIB = $(PREFIX)/lib
		endif
		ifeq (,$(PREFIX_FOR_STARTUP))
			PREFIX_FOR_STARTUP = $(PREFIX)/lib
		endif
	endif
else
	PREFIX_FOR_LIB = $(DESTDIR)/usr/lib
	PREFIX_FOR_STARTUP = $(DESTDIR)/usr/lib
endif

ifneq (,$(PREFIX_FOR_INCLUDE))
install : install-include
install-include:
	@mkdir -pv $(PREFIX_FOR_INCLUDE)
	@cp -arv include/* $(PREFIX_FOR_INCLUDE)
endif

ifneq (,$(PREFIX_FOR_LIB))
install : install-libs
install-libs:
	@for i in $(MULTILIBDIRS); do \
		mkdir -pv $(PREFIX_FOR_LIB)/$$i; \
		cp -av $(BUILDDIR)/$$i/$(LIBC) $(BUILDDIR)/$$i/$(LIBIIO) $(PREFIX_FOR_LIB)/$$i; \
	done
endif

ifneq (,$(PREFIX_FOR_STARTUP))
install : install-startup
install-startup:
	@for i in $(MULTILIBDIRS); do \
		mkdir -pv $(PREFIX_FOR_STARTUP)/$$i; \
		cp -av $(BUILDDIR)/$$i/startup.o $(PREFIX_FOR_STARTUP)/$$i; \
	done
endif

ifneq (clean,$(MAKECMDGOALS))
-include $(DEPENDS)
endif
