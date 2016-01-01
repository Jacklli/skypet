#ifndef __HTTPD_H__
#define __HTTPD_H__

void httpd_init(void);
void httpd(void);

/* UIP_APPCALL: the name of the application function. This function
   must return void and take no arguments (i.e., C type "void
   appfunc(void)"). */
#define UIP_APPCALL     httpd

struct httpd_state {
  u8_t state; 
  u16_t count;
  char *dataptr;
  char *script;
};


/* UIP_APPSTATE_SIZE: The size of the application-specific state
   stored in the uip_conn structure. */
#define UIP_APPSTATE_SIZE (sizeof(struct httpd_state))

#define FS_STATISTICS 1

extern struct httpd_state *hs;

#endif /* __HTTPD_H__ */
