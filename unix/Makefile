CC=gcc
CFLAGS=-Wall -g -fpack-struct -I../net -I. -I../apps/httpd -O1

%.o:
	$(CC) $(CFLAGS) -c $(<:.o=.c)


net: net.o arch.o tapdev.o httpd.o main.o fs.o arp.o cgi.o

httpd.o: ../apps/httpd/httpd.c ../apps/httpd/httpd.h ../net/net.h \
 uipopt.h ../apps/httpd/fs.h
cgi.o: ../apps/httpd/cgi.c ../net/net.h uipopt.h ../apps/httpd/httpd.h \
 ../apps/httpd/cgi.h ../apps/httpd/fs.h
fs.o: ../apps/httpd/fs.c ../net/net.h uipopt.h ../apps/httpd/httpd.h \
 ../apps/httpd/fs.h ../apps/httpd/fsdata.h \
 ../apps/httpd/fsdata.c
tapdev.o: tapdev.c uipopt.h
main.o: main.c ../net/net.h uipopt.h ../apps/httpd/httpd.h \
 tapdev.h
arch.o: arch.c arch.h ../net/net.h uipopt.h \
 ../apps/httpd/httpd.h 
net.o: ../net/net.c ../net/net.h uipopt.h ../apps/httpd/httpd.h 

arp.o: ../net/arp.c ../net/arp.h ../net/net.h uipopt.h \
 ../apps/httpd/httpd.h 

clean:
	rm -f *.o *~ *core net






