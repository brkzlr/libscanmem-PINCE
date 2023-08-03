ACLOCAL_AMFLAGS = -I m4

# '-O2 -g' are added by `configure`, unless the user overrides CFLAGS
AM_CFLAGS = -std=gnu99 -Wall

# Utilities library, statically linked in sm and libsm
noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = common.h \
    common.c \
    show_message.c

# libscanmem
lib_LTLIBRARIES = libscanmem.la

libscanmem_la_includedir = $(includedir)/scanmem

libscanmem_la_include_HEADERS = commands.h \
    handlers.h \
    interrupt.h \
    list.h \
    maps.h \
    scanmem.h \
    scanroutines.h \
    sets.h \
    show_message.h \
    targetmem.h \
    value.h

libscanmem_la_SOURCES = commands.c \
    procmem.c \
    handlers.c \
    interrupt.c \
    maps.c \
    scanmem.c \
    scanroutines.c \
    sets.c \
    targetmem.c \
    value.c 

libscanmem_la_LIBADD = libutil.la

libscanmem_la_LDFLAGS = -version-info 1:0:0 \
                        -export-symbols-regex '^sm_'

# Misc
dist_doc_DATA = README

EXTRA_DIST = lgpl-3.0.txt
