# platform_dgux4_epc.GNU,v 4.5 1997/09/10 16:47:14 levine Exp

# SunOS 5.x (Solaris 2.x) with SunC++ 4.x
# *not* using Orbix

debug = 1

CC		= gcc
CXX		= ec++
## CCFLAGS	+= -relax -tproff
CCFLAGS		+= $(CFLAGS) -relax -v -eh
DCFLAGS		+= -g
DLD		= $(CXX) 
LIBS		+= -lgen -lsocket -ldl -lnsl -lc
PIC		= -KPIC
AR		= ar
ARFLAGS		= -ruv
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
#SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
#		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
