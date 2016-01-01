#ifndef __CGI_H__
#define __CGI_H__

typedef u8_t (* cgifunction)(void);

extern cgifunction cgitab[];

#endif /* __CGI_H__ */
