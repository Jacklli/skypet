#include "net.h"
#include "httpd.h"
#include "fs.h"
#include "fsdata.h"
//#include "cgi.h"

#define NULL (void *)0

/* The HTTP server states: */
#define HTTP_NOGET        0
#define HTTP_FILE         1
#define HTTP_TEXT         2
#define HTTP_FUNC         3
#define HTTP_END          4

#ifdef DEBUG
#include <stdio.h>
#define PRINT(x) printf("%s", x)
#define PRINTLN(x) printf("%s\n", x)
#else /* DEBUG */
#define PRINT(x)
#define PRINTLN(x)
#endif /* DEBUG */

struct httpd_state *hs;

extern const struct fsdata_file file_index_html;


#define ISO_G        0x47
#define ISO_E        0x45
#define ISO_T        0x54
#define ISO_slash    0x2f    
#define ISO_c        0x63
#define ISO_g        0x67
#define ISO_i        0x69
#define ISO_space    0x20
#define ISO_nl       0x0a
#define ISO_cr       0x0d
#define ISO_a        0x61
#define ISO_t        0x74
#define ISO_hash     0x23
#define ISO_period   0x2e


void
httpd_init(void)
{
  fs_init();
  
  /* Listen to port 80. */
  uip_listen(80);
}

void
httpd(void)
{
  struct fs_file fsfile;  
  u8_t i;
  
  switch(uip_conn->lport) {
    /* This is the web server: */
  case htons(80):
    /* Pick out the application state from the uip_conn structure. */
    hs = (struct httpd_state *)(uip_conn->appstate);

    /* We use the uip_ test functions to deduce why we were
       called. If uip_connected() is non-zero, we were called
       because a remote host has connected to us. If
       uip_newdata() is non-zero, we were called because the
       remote host has sent us new data, and if uip_acked() is
       non-zero, the remote host has acknowledged the data we
       previously sent to it. */
    if(uip_connected()) {
      /* Since we have just been connected with the remote host, we
         reset the state for this connection. The ->count variable
         contains the amount of data that is yet to be sent to the
         remote host, and the ->state is set to HTTP_NOGET to signal
         that we haven't received any HTTP GET request for this
         connection yet. */
      hs->state = HTTP_NOGET;
      hs->count = 0;
      /* Don't send any data in return; we wait for the HTTP request
	 instead. */
      uip_send(uip_appdata, 0);
      return;

    } else if(uip_poll()) {
      /* If we are polled ten times, we abort the connection. This is
         because we don't want connections lingering indefinately in
         the system. */
      if(hs->count++ >= 10) {
	uip_abort();
      }
      return;
    } else if(uip_newdata() && hs->state == HTTP_NOGET) {
      /* This is the first data we receive, and it should contain a
	 GET. */
      
      /* Check for GET. */
      if(uip_appdata[0] != ISO_G ||
	 uip_appdata[1] != ISO_E ||
	 uip_appdata[2] != ISO_T ||
	 uip_appdata[3] != ISO_space) {
	/* If it isn't a GET, we abort the connection. */
	uip_abort();
	return;
      }
	       
      /* Find the file we are looking for. */
      for(i = 4; i < 40; ++i) {
	if(uip_appdata[i] == ISO_space ||
	   uip_appdata[i] == ISO_cr ||
	   uip_appdata[i] == ISO_nl) {
	  uip_appdata[i] = 0;
	  break;
	}
      }

      PRINT("request for file ");
      PRINTLN(&uip_appdata[4]);
      
      if(!fs_open((const char *)&uip_appdata[4], &fsfile)) {
	PRINTLN("couldn't open file");
	fs_open(file_index_html.name, &fsfile);
      } 

      hs->script = NULL;
      /* The web server is now no longer in the HTTP_NOGET state, but
	   in the HTTP_FILE state since is has now got the GET from
	   the client and will start transmitting the file. */
      hs->state = HTTP_FILE;

      /* Point the file pointers in the connection state to point to
	   the first byte of the file. */
      hs->dataptr = fsfile.data;
      hs->count = fsfile.len;	
    }

    
    if(hs->state != HTTP_FUNC) {
      /* Check if the client (remote end) has acknowledged any data that
	 we've previously sent. If so, we move the file pointer further
	 into the file and send back more data. If we are out of data to
	 send, we close the connection. */
      if(uip_acked()) {
	
	if(hs->count >= uip_mss()) {
	  hs->count -= uip_mss();
	  hs->dataptr += uip_mss();
	} else {
	  hs->count = 0;
	}
	
      }         
    }
    
    if(hs->state != HTTP_FUNC && !uip_poll()) {
      /* Send a piece of data, but not more than the MSS of the
	 connection. */
      uip_send(hs->dataptr,
	       hs->count > uip_mss()? uip_mss(): hs->count);
    }

    /* Finally, return to uIP. Our outgoing packet will soon be on its
       way... */
    return;

  default:
    /* Should never happen. */
    uip_abort();
    break;
  }  
}
