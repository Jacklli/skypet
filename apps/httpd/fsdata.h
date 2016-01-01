#ifndef __FSDATA_H__
#define __FSDATA_H__

#include "uipopt.h"

struct fsdata_file {
  const struct fsdata_file *next;
  const char *name;
  const char *data;
  const int len;
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t count;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
};

struct fsdata_file_noconst {
  struct fsdata_file *next;
  char *name;
  char *data;
  int len;
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t count;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
};

#endif /* __FSDATA_H__ */
