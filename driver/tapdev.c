#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/socket.h>

#ifdef linux
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#define DEVTAP "/dev/net/tun"
#else  /* linux */
#define DEVTAP "/dev/tap0"
#endif /* linux */

#include "net.h"

//static int drop = 0;
static int fd;

static unsigned long lasttime;
static struct timezone tz;

/*-----------------------------------------------------------------------------------*/
void
tapdev_init(void)
{
  int ret = 0;
  char buf[1024];
  
  fd = open(DEVTAP, O_RDWR);
  if(fd == -1) {
    perror("tapdev: tapdev_init: open");
    exit(1);
  }

#ifdef linux
  {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP|IFF_NO_PI;
    if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
      perror(buf);
      exit(1);
    }
  }
#endif /* Linux */

  snprintf(buf, sizeof(buf), "ifconfig tap0 inet %d.%d.%d.%d",
	   UIP_DRIPADDR0, UIP_DRIPADDR1, UIP_DRIPADDR2, UIP_DRIPADDR3);
  if((ret = system(buf)) == -1) printf("system call error!\n");

  lasttime = 0;
}
/*-----------------------------------------------------------------------------------*/
unsigned int
tapdev_read(void)
{
  fd_set fdset;
  struct timeval tv, now;
  int ret;
  
  if(lasttime >= 500000) {
    lasttime = 0;
    return 0;
  }
  
  tv.tv_sec = 0;
  tv.tv_usec = 500000 - lasttime;


  FD_ZERO(&fdset);
  FD_SET(fd, &fdset);

  gettimeofday(&now, &tz);  
  ret = select(fd + 1, &fdset, NULL, NULL, &tv);
  if(ret == 0) {
    lasttime = 0;    
    return 0;
  } 
  ret = read(fd, uip_buf, UIP_BUFSIZE);  
  if(ret == -1) {
    perror("tap_dev: tapdev_read: read");
  }
  gettimeofday(&tv, &tz);
  lasttime += (tv.tv_sec - now.tv_sec) * 1000000 + (tv.tv_usec - now.tv_usec);

  /*  printf("--- tap_dev: tapdev_read: read %d bytes\n", ret);*/
  /*  {
    int i;
    for(i = 0; i < 20; i++) {
      printf("%x ", uip_buf[i]);
    }
    printf("\n");
    }*/
  /*  check_checksum(uip_buf, ret);*/
  return ret;
}
/*-----------------------------------------------------------------------------------*/
void
tapdev_send(void)
{
  int ret;
  struct iovec iov[2];
  /*  printf("tapdev_send: sending %d bytes\n", size);*/
  /*  check_checksum(uip_buf, size);*/

  /*  drop++;
  if(drop % 8 == 7) {
    printf("Dropped a packet!\n");
    return;
    }*/

  iov[0].iov_base = uip_buf;
  iov[0].iov_len = 40 + UIP_LLH_LEN;
  iov[1].iov_base = (char *)uip_appdata;
  iov[1].iov_len = uip_len - 40 + UIP_LLH_LEN;
  
  
  ret = writev(fd, iov, 2);
  if(ret == -1) {
    perror("tap_dev: tapdev_send: writev");
    exit(1);
  }
}  
/*-----------------------------------------------------------------------------------*/
