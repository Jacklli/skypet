#include "net.h"
#include "arp.h"
#include "tapdev.h"
#include "httpd.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
/*-----------------------------------------------------------------------------------*/
int
main(void)
{
  u8_t i, arptimer;
  
  tapdev_init();
  uip_init();
  httpd_init();

  arptimer = 0;
  
  while(1) {
    /* Let the tapdev network device driver read an entire IP packet
       into the uip_buf. If it must wait for more than 0.5 seconds, it
       will return with the return value 0. If so, we know that it is
       time to call upon the uip_periodic(). Otherwise, the tapdev has
       received an IP packet that is to be processed by uIP. */
    uip_len = tapdev_read();
    if(uip_len == 0) {
      for(i = 0; i < UIP_CONNS; i++) {
	uip_periodic(i);
      /* If the above function invocation resulted in data that
	 should be sent out on the network, the global variable
	 uip_len is set to a value > 0. */
	if(uip_len > 0) {
	  uip_arp_out();
	  tapdev_send();
	}
      }

      /* Call the ARP timer function every 10 seconds. */
      if(++arptimer == 20) {	
	uip_arp_timer();
	arptimer = 0;
      }
      
    } else {
      if(BUF->type == htons(UIP_ETHTYPE_IP)) {
	uip_arp_ipin();
	uip_len -= sizeof(struct uip_eth_hdr);
	uip_input();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */
	if(uip_len > 0) {
	  uip_arp_out();
	  tapdev_send();
	}
      } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {
	uip_arp_arpin();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */	
	if(uip_len > 0) {	
	  tapdev_send();
	}
      }
    }
    
  }
  return 0;
}
