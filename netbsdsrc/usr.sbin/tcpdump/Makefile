#	$NetBSD: Makefile,v 1.9.2.1 1997/11/08 15:12:49 lukem Exp $	

PROG=	tcpdump
MAN=	tcpdump.8 

CPPFLAGS+=-DHAVE_FCNTL_H=1 -DHAVE_MALLOC_H=1 -DHAVE_MEMORY_H=1
CPPFLAGS+=-DTIME_WITH_SYS_TIME=1 -DHAVE_NET_SLIP_H=1 -DHAVE_VFPRINTF=1
CPPFLAGS+=-DHAVE_STRCASECMP=1 -DHAVE_ETHER_NTOA=1 -DHAVE_SETLINEBUF=1
CPPFLAGS+=-DRETSIGTYPE=void -DRETSIGVAL= -DHAVE_SIGACTION=1
CPPFLAGS+=-DHAVE_SOCKADDR_SA_LEN=1 -DHAVE_FDDI

LDADD+=	-lpcap -ll
DPADD+=	${LIBPCAP} ${LIBL}

SRCS=	util.c tcpdump.c setsignal.c savestr.c print-wb.c print-udp.c \
print-tftp.c print-tcp.c print-sunrpc.c print-snmp.c print-sl.c print-rip.c \
print-raw.c print-ppp.c print-pim.c print-ospf.c print-null.c print-ntp.c \
print-nfs.c print-netbios.c print-llc.c print-krb.c print-isoclns.c print-ipx.c \
print-ip.c print-igrp.c print-icmp.c print-gre.c print-fddi.c print-ether.c \
print-egp.c print-dvmrp.c print-domain.c print-decnet.c print-bootp.c print-atm.c \
print-atalk.c print-arp.c parsenfsfh.c machdep.c gmt2local.c bpf_dump.c \
addrtoname.c 
SRCS+= version.c

AWKS= atime.awk packetdat.awk stime.awk send-ack.awk   

.include <bsd.prog.mk>
