#include "net.h"
#include "fs.h"
#include "fsdata.h"

#define NULL (void *)0
#include "fsdata.c"

#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
static u16_t count[FS_NUMFILES];
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */

/*-----------------------------------------------------------------------------------*/
static u8_t
fs_strcmp(const char *str1, const char *str2)
{
  u8_t i;
  i = 0;
 loop:

  if(str2[i] == 0 ||
     str1[i] == '\r' || 
     str1[i] == '\n') {
    return 0;
  }

  if(str1[i] != str2[i]) {
    return 1;
  }


  ++i;
  goto loop;
}
/*-----------------------------------------------------------------------------------*/
int
fs_open(const char *name, struct fs_file *file)
{
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t i = 0;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
  struct fsdata_file_noconst *f;

  for(f = (struct fsdata_file_noconst *)FS_ROOT;
      f != NULL;
      f = (struct fsdata_file_noconst *)f->next) {

    if(fs_strcmp(name, f->name) == 0) {
      file->data = f->data;
      file->len = f->len;
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
      ++count[i];
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
      return 1;
    }
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
    ++i;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */

  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void
fs_init(void)
{
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t i;
  for(i = 0; i < FS_NUMFILES; i++) {
    count[i] = 0;
  }
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
}
/*-----------------------------------------------------------------------------------*/
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1  
u16_t fs_count
(char *name)
{
  struct fsdata_file_noconst *f;
  u16_t i;

  i = 0;
  for(f = (struct fsdata_file_noconst *)FS_ROOT;
      f != NULL;
      f = (struct fsdata_file_noconst *)f->next) {

    if(fs_strcmp(name, f->name) == 0) {
      return count[i];
    }
    ++i;
  }
  return 0;
}
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
/*-----------------------------------------------------------------------------------*/
