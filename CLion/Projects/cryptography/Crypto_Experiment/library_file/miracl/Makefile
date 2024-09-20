#SDK_TOPDIR = /home/hudaolei/test/sm
EXC_FILE_NAME = test

#CC      = /opt/compiler/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-gcc
CC      = gcc
CFLAGS  = -Wall -g 
CFLAGS += -I$(shell pwd)
#LDFLAGS = -L$(SDK_TOPDIR)/lib -lc -ldirectfb -lfusion -ldirect -lz -lleck -llite -ldbus-1 -lrt -lpos -lnfc -liccard

OBJS = main.o \
	   hexdump.o \
	   mraes.c \
 	   mralloc.c \
 	   mrarth0.c \
	   mrarth1.c \
 	   mrarth2.c \
 	   mrarth3.c \
 	   mrbits.c \
 	   mrbrick.c \
 	   mrcore.c \
 	   mrcrt.c \
 	   mrcurve.c \
	   mrebrick.c \
 	   mrec2m.c \
 	   mrecn2.c \
 	   mrfast.c \
 	   mrgcd.c \
	   mrgcm.c \
	   mrgf2m.c \
 	   mrio1.c \
 	   mrio2.c \
 	   mrjack.c \
 	   mrlucas.c \
 	   mrmonty.c \
 	   mrpower.c \
 	   mrprime.c \
 	   mrrand.c \
 	   mrscrt.c \
 	   mrshs256.c \
 	   mrshs512.c \
 	   mrshs.c \
 	   mrsmall.c \
 	   mrsroot.c \
 	   mrstrong.c \
 	   mrxgcd.c \
 	   mrzzn2b.c \
 	   mrzzn2.c \
 	   mrzzn3.c \
 	   sm2.c \
	   sm3.o \
	   
	   
	   


$(EXC_FILE_NAME) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(EXC_FILE_NAME)
	rm -f *.o *~
