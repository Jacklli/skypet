#ifndef __UIP_ARP_H__
#define __UIP_ARP_H__

#include "uip.h"

struct uip_eth_addr {
  u8_t addr[6];
};
  
struct uip_eth_hdr {
  struct uip_eth_addr dest;
  struct uip_eth_addr src;
  u16_t type;
};

#define UIP_ETHTYPE_ARP 0x0806
#define UIP_ETHTYPE_IP  0x0800

/* The uip_arp_init() function must be called before any of the other
   ARP functions. */
void uip_arp_init(void);

/* The uip_arp_ipin() function should be called whenever an IP packet
   arrives from the Ethernet. This function refreshes the ARP table or
   inserts a new mapping if none exists. The function assumes that an
   IP packet with an Ethernet header is present in the uip_buf buffer
   and that the length of the packet is in the uip_len variable. */
void uip_arp_ipin(void);

/* The uip_arp_arpin() should be called when an ARP packet is received
   by the Ethernet driver. This function also assumes that the
   Ethernet frame is present in the uip_buf buffer. When the
   uip_arp_arpin() function returns, the contents of the uip_buf
   buffer should be sent out on the Ethernet if the uip_len variable
   is > 0. */
void uip_arp_arpin(void);

/* The uip_arp_out() function should be called when an IP packet
   should be sent out on the Ethernet. This function creates an
   Ethernet header before the IP header in the uip_buf buffer. The
   Ethernet header will have the correct Ethernet MAC destination
   address filled in if an ARP table entry for the destination IP
   address (or the IP address of the default router) is present. If no
   such table entry is found, the IP packet is overwritten with an ARP
   request and we rely on TCP to retransmit the packet that was
   overwritten. In any case, the uip_len variable holds the length of
   the Ethernet frame that should be transmitted. */
void uip_arp_out(void);

/* The uip_arp_timer() function should be called every ten seconds. It
   is responsible for flushing old entries in the ARP table. */
void uip_arp_timer(void);

#endif /* __UIP_ARP_H__ */
