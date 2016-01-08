CC=gcc
CFLAGS=-Wall -g -fpack-struct -I../net -I. -I../driver -I./httpd -O1

%.o:
	$(CC) $(CFLAGS) -c $(<:.o=.c)


net: net.o arch.o tapdev.o httpd.o main.o fs.o arp.o

httpd.o: httpd/httpd.c httpd/httpd.h ../net/net.h \
 ../driver/uipopt.h httpd/fs.h
#cgi.o: httpd/cgi.c ../net/net.h ../driver/uipopt.h httpd/httpd.h \
 httpd/cgi.h ../apps/httpd/fs.h
fs.o: httpd/fs.c ../net/net.h ../driver/uipopt.h ../apps/httpd/httpd.h \
 httpd/fs.h ../apps/httpd/fsdata.h \
 httpd/fsdata.c
tapdev.o: ../driver/tapdev.c ../driver/uipopt.h
main.o: main.c ../net/net.h ../driver/uipopt.h httpd/httpd.h \
 ../driver/tapdev.h
arch.o: ../driver/arch.c ../driver/arch.h ../net/net.h ../driver/uipopt.h \
 httpd/httpd.h 
net.o: ../net/net.c ../net/net.h ../driver/uipopt.h httpd/httpd.h 

arp.o: ../net/arp.c ../net/arp.h ../net/net.h ../driver/uipopt.h \
 httpd/httpd.h 

clean:
	rm -f *.o *~ *core net






