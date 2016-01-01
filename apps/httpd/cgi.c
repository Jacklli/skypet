/*
 * This file includes functions that are called by the web server
 * scripts. The functions takes no argument, and the return value is
 * interpreted as follows. A zero means that the function did not
 * complete and should be invoked for the next packet as well. A
 * non-zero value indicates that the function has completed and that
 * the web server should move along to the next script line.
 *
 */

#include "uip.h"
#include "cgi.h"
#include "httpd.h"
#include "fs.h"

#include <stdio.h>
#include <string.h>

static u8_t print_stats(void);
static u8_t file_stats(void);
static u8_t tcp_stats(void);

cgifunction cgitab[] = {
  print_stats,   /* CGI function "a" */
  file_stats,    /* CGI function "b" */
  tcp_stats      /* CGI function "c" */
};

static const char closed[] =   /*  "CLOSED",*/
{0x43, 0x4c, 0x4f, 0x53, 0x45, 0x44, 0};
static const char syn_rcvd[] = /*  "SYN-RCVD",*/
{0x53, 0x59, 0x4e, 0x2d, 0x52, 0x43, 0x56, 
 0x44,  0};
static const char syn_sent[] = /*  "SYN-SENT",*/
{0x53, 0x59, 0x4e, 0x2d, 0x53, 0x45, 0x4e, 
 0x54,  0};
static const char established[] = /*  "ESTABLISHED",*/
{0x45, 0x53, 0x54, 0x41, 0x42, 0x4c, 0x49, 0x53, 0x48, 
 0x45, 0x44, 0};
static const char fin_wait_1[] = /*  "FIN-WAIT-1",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49, 
 0x54, 0x2d, 0x31, 0};
static const char fin_wait_2[] = /*  "FIN-WAIT-2",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49, 
 0x54, 0x2d, 0x32, 0};
static const char closing[] = /*  "CLOSING",*/
{0x43, 0x4c, 0x4f, 0x53, 0x49, 
 0x4e, 0x47, 0};
static const char time_wait[] = /*  "TIME-WAIT,"*/
{0x54, 0x49, 0x4d, 0x45, 0x2d, 0x57, 0x41, 
 0x49, 0x54, 0};
static const char last_ack[] = /*  "LAST-ACK"*/
{0x4c, 0x41, 0x53, 0x54, 0x2d, 0x41, 0x43, 
 0x4b, 0};

static const char *states[] = {
  closed,
  syn_rcvd,
  syn_sent,
  established,
  fin_wait_1,
  fin_wait_2,
  closing,
  time_wait,
  last_ack};
  

/*-----------------------------------------------------------------------------------*/
/* print_stats:
 *
 * Prints out a part of the uIP statistics. The statistics data is
 * written into the uip_appdata buffer. It overwrites any incoming
 * packet.
 */
static u8_t
print_stats(void)
{
#if UIP_STATISTICS
  u16_t i, j;
  u8_t *buf;
  u16_t *databytes;
  
  if(uip_acked()) {
    /* If our last data has been acknowledged, we move on the next
       chunk of statistics. */
    hs->count = hs->count + 4;
    if(hs->count >= sizeof(struct uip_stats)/sizeof(u16_t)) {
      /* We have printed out all statistics, so we return 1 to
	 indicate that we are done. */
      return 1;
    }
  }

  /* Write part of the statistics into the uip_appdata buffer. */
  databytes = (u16_t *)&uip_stat + hs->count;
  buf       = (u8_t *)uip_appdata;

  j = 4 + 1;
  i = hs->count;
  while (i < sizeof(struct uip_stats)/sizeof(u16_t) && --j > 0) {
    sprintf((char *)buf, "%5u\r\n", *databytes);
    ++databytes;
    buf += 6;
    ++i;
  }

  /* Send the data. */
  uip_send(uip_appdata, buf - uip_appdata);
  
  return 0;
#else
  return 1;
#endif /* UIP_STATISTICS */
}
/*-----------------------------------------------------------------------------------*/
static u8_t
file_stats(void)
{
  /* We use sprintf() to print the number of file accesses to a
     particular file (given as an argument to the function in the
     script). We then use uip_send() to actually send the data. */
  if(uip_acked()) {
    return 1;
  }
  uip_send(uip_appdata, sprintf((char *)uip_appdata, "%5u", fs_count(&hs->script[4])));  
  return 0;
}
/*-----------------------------------------------------------------------------------*/
static u8_t
tcp_stats(void)
{
  struct uip_conn *conn;  

  if(uip_acked()) {
    /* If the previously sent data has been acknowledged, we move
       forward one connection. */
    if(++hs->count == UIP_CONNS) {
      /* If all connections has been printed out, we are done and
	 return 1. */
      return 1;
    }
  }
  
  conn = &uip_conns[hs->count];
  if((conn->tcpstateflags & TS_MASK) == CLOSED) {
    uip_send(uip_appdata, sprintf((char *)uip_appdata,
				  "<tr align=\"center\"><td>-</td><td>-</td><td>%d</td><td>%d</td><td>%c %c</td></tr>\r\n",
				  conn->nrtx,
				  conn->timer,
				  (conn->tcpstateflags & UIP_OUTSTANDING)? '*':' ',
    				  (conn->tcpstateflags & UIP_STOPPED)? '!':' '));
  } else {
    uip_send(uip_appdata, sprintf((char *)uip_appdata,
				  "<tr align=\"center\"><td>%d.%d.%d.%d:%d</td><td>%s</td><td>%d</td><td>%d</td><td>%c %c</td></tr>\r\n",
				  ntohs(conn->ripaddr[0]) >> 8,
				  ntohs(conn->ripaddr[0]) & 0xff,
				  ntohs(conn->ripaddr[1]) >> 8,
				  ntohs(conn->ripaddr[1]) & 0xff,
				  ntohs(conn->rport),
				  states[conn->tcpstateflags & TS_MASK],
				  conn->nrtx,
				  conn->timer,
				  (conn->tcpstateflags & UIP_OUTSTANDING)? '*':' ',
    				  (conn->tcpstateflags & UIP_STOPPED)? '!':' '));
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
