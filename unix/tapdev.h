#ifndef __TAPDEV_H__
#define __TAPDEV_H__

void tapdev_init(void);
unsigned int tapdev_read(void);
void tapdev_send(void);

#endif /* __TAPDEV_H__ */
