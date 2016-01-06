#ifndef __UIP_ARCH_H__
#define __UIP_ARCH_H__

#include "net.h"

#if UIP_BUFSIZE > 255
void uip_add_rcv_nxt(u16_t n);
void uip_add_ack_nxt(u16_t n);
#else
void uip_add_rcv_nxt(u8_t n);
void uip_add_ack_nxt(u8_t n);
#endif /* UIP_BUFSIZE > 255 */
u16_t uip_ipchksum(void);
u16_t uip_tcpchksum(void);

#endif /* __UIP_ARCH_H__ */
