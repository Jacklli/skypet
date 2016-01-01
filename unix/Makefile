CC=gcc
CFLAGS=-Wall -g -fpack-struct -I../uip -I. -I../apps/httpd -O1

%.o:
	$(CC) $(CFLAGS) -c $(<:.o=.c)


uip: uip.o uip_arch.o tapdev.o httpd.o main.o fs.o uip_arp.o cgi.o

httpd.o: ../apps/httpd/httpd.c ../apps/httpd/httpd.h ../uip/uip.h \
 uipopt.h ../apps/httpd/fs.h
cgi.o: ../apps/httpd/cgi.c ../uip/uip.h uipopt.h ../apps/httpd/httpd.h \
 ../apps/httpd/cgi.h ../apps/httpd/fs.h
fs.o: ../apps/httpd/fs.c ../uip/uip.h uipopt.h ../apps/httpd/httpd.h \
 ../apps/httpd/fs.h ../apps/httpd/fsdata.h \
 ../apps/httpd/fsdata.c
tapdev.o: tapdev.c uipopt.h
main.o: main.c ../uip/uip.h uipopt.h ../apps/httpd/httpd.h \
 tapdev.h
uip_arch.o: uip_arch.c uip_arch.h ../uip/uip.h uipopt.h \
 ../apps/httpd/httpd.h 
uip.o: ../uip/uip.c ../uip/uip.h uipopt.h ../apps/httpd/httpd.h 

uip_arp.o: ../uip/uip_arp.c ../uip/uip_arp.h ../uip/uip.h uipopt.h \
 ../apps/httpd/httpd.h 

clean:
	rm -f *.o *~ *core uip






