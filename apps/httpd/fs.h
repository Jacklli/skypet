#ifndef __FS_H__
#define __FS_H__

#include "net.h"

struct fs_file {
  char *data;
  int len;
};

/* file must be allocated by caller and will be filled in
   by the function. */
int fs_open(const char *name, struct fs_file *file);

#ifdef FS_STATISTICS
#if FS_STATISTICS == 1  
u16_t fs_count(char *name);
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */

void fs_init(void);

#endif /* __FS_H__ */
