#include "net.h"
#include "arch.h"

#define BUF ((uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])
#define IP_PROTO_TCP    6

/*-----------------------------------------------------------------------------------*/
#if UIP_BUFSIZE > 255
/*-----------------------------------------------------------------------------------*/
void
uip_add_rcv_nxt(u16_t n)
{
  uip_conn->rcv_nxt[3] += (n & 0xff);
  uip_conn->rcv_nxt[2] += (n >> 8);

  if(uip_conn->rcv_nxt[2] < (n >> 8)) {
    ++uip_conn->rcv_nxt[1];    
    if(uip_conn->rcv_nxt[1] == 0) {
      ++uip_conn->rcv_nxt[0];
    }
  }
  
  
  if(uip_conn->rcv_nxt[3] < (n & 0xff)) {
    ++uip_conn->rcv_nxt[2];  
    if(uip_conn->rcv_nxt[2] == 0) {
      ++uip_conn->rcv_nxt[1];    
      if(uip_conn->rcv_nxt[1] == 0) {
	++uip_conn->rcv_nxt[0];
      }
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void
uip_add_ack_nxt(u16_t n)
{
  uip_conn->ack_nxt[3] += (n & 0xff);
  uip_conn->ack_nxt[2] += (n >> 8);

  if(uip_conn->ack_nxt[2] < (n >> 8)) {
    ++uip_conn->ack_nxt[1];    
    if(uip_conn->ack_nxt[1] == 0) {
      ++uip_conn->ack_nxt[0];
    }
  }
  
  
  if(uip_conn->ack_nxt[3] < (n & 0xff)) {
    ++uip_conn->ack_nxt[2];  
    if(uip_conn->ack_nxt[2] == 0) {
      ++uip_conn->ack_nxt[1];    
      if(uip_conn->ack_nxt[1] == 0) {
	++uip_conn->ack_nxt[0];
      }
    }
  }
}
/*-----------------------------------------------------------------------------------*/
#else /* UIP_BUFSIZE > 255 */
/*-----------------------------------------------------------------------------------*/
void
uip_add_rcv_nxt(u8_t n)
{
  uip_conn->rcv_nxt[3] += n;
  if(uip_conn->rcv_nxt[3] < n) {
    ++uip_conn->rcv_nxt[2];  
    if(uip_conn->rcv_nxt[2] == 0) {
      ++uip_conn->rcv_nxt[1];    
      if(uip_conn->rcv_nxt[1] == 0) {
	++uip_conn->rcv_nxt[0];
      }
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void
uip_add_ack_nxt(u8_t n)
{
  uip_conn->ack_nxt[3] += n;
  if(uip_conn->ack_nxt[3] < n) {
    ++uip_conn->ack_nxt[2];   
    if(uip_conn->ack_nxt[2] == 0) {
      ++uip_conn->ack_nxt[1];
      if(uip_conn->ack_nxt[1] == 0) {
	++uip_conn->ack_nxt[0];
      }
    }
  }
}
/*-----------------------------------------------------------------------------------*/
#endif /* UIP_BUFSIZE > 255 */
static u16_t
chksum(u16_t *sdata, u16_t len)
{
  u16_t acc;
  
  for(acc = 0; len > 1; len -= 2) {
    acc += *sdata;
    if(acc < *sdata) {
      /* Overflow, so we add the carry to acc (i.e., increase by
         one). */
      ++acc;
    }
    ++sdata;
  }

  /* add up any odd byte */
  if(len == 1) {
    acc += htons(((u16_t)(*(u8_t *)sdata)) << 8);
    if(acc < htons(((u16_t)(*(u8_t *)sdata)) << 8)) {
      ++acc;
    }
  }

  return acc;
}
/*-----------------------------------------------------------------------------------*/
u16_t
uip_ipchksum(void)
{
  return chksum((u16_t *)&uip_buf[UIP_LLH_LEN], 20);
}
/*-----------------------------------------------------------------------------------*/
u16_t
uip_tcpchksum(void)
{
  u16_t hsum, sum;

  
  /* Compute the checksum of the TCP header. */
  hsum = chksum((u16_t *)&uip_buf[20 + UIP_LLH_LEN], 20);

  /* Compute the checksum of the data in the TCP packet and add it to
     the TCP header checksum. */
  sum = chksum((u16_t *)uip_appdata,
	       (u16_t)(((((u16_t)(BUF->len[0]) << 8) + BUF->len[1]) - 40)));

  if((sum += hsum) < hsum) {
    ++sum;
  }
  
  if((sum += BUF->srcipaddr[0]) < BUF->srcipaddr[0]) {
    ++sum;
  }
  if((sum += BUF->srcipaddr[1]) < BUF->srcipaddr[1]) {
    ++sum;
  }
  if((sum += BUF->destipaddr[0]) < BUF->destipaddr[0]) {
    ++sum;
  }
  if((sum += BUF->destipaddr[1]) < BUF->destipaddr[1]) {
    ++sum;
  }
  if((sum += (u16_t)htons((u16_t)IP_PROTO_TCP)) < (u16_t)htons((u16_t)IP_PROTO_TCP)) {
    ++sum;
  }

  hsum = (u16_t)htons((((u16_t)(BUF->len[0]) << 8) + BUF->len[1]) - 20);
  
  if((sum += hsum) < hsum) {
    ++sum;
  }
  
  return sum;
}
/*-----------------------------------------------------------------------------------*/
