TARGET =	hw1
LIBS =		-lsimple -lcheck -lsuif -luseful
OBJS =		doproc.o main.o print.o
HEADERS =	print.h
CSRCS =		doproc.cc print.c
SRCS =		main.cc
INCLDIRFLAG =	-I$(SUIFHOME)/include
EXTRA_YFLAGS =
EXTRA_CFLAGS =	$(INCLDIRFLAG)
EXTRA_CXXFLAGS =
all:            $(TARGET)
install-bin:    install-prog

include $(SUIFHOME)/Makefile.std