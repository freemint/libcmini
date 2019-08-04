#ifndef _EXT_H_
#define _EXT_H_

#include <time.h>
#include <stddef.h>

extern int __text, __data, __bss;
#define _text &__text
#define _data &__data
#define _bss  &__bss

#define FA_UPDATE       0x00
#define FA_RDONLY       0x01
#define FA_HIDDEN       0x02
#define FA_SYSTEM       0x04
#define FA_LABEL        0x08
#define FA_DIREC        0x10
#define FA_ARCH         0x20

#define MAXPATH   119
#define MAXDRIVE  3
#define MAXDIR    102
#define MAXFILE   9
#define MAXEXT    5

#define S_IFCHR   0020000
#define S_IFDIR   0040000
#define S_IFREG   0100000
#define S_IEXEC   0000100
#define S_IREAD   0000400
#define S_IWRITE  0000200


struct ffblk
{
    char  ff_reserved[21];              /* Reserved by TOS */
    char  ff_attrib;                    /* Attribute found */
    short ff_ftime;                     /* File time */
    short ff_fdate;                     /* File date */
    long  ff_fsize;                     /* File size */
    char  ff_name[13];                  /* File name found */
};

struct date
{
    int    da_year;                     /* Current year */
    char   da_day;                      /* Day of the month */
    char   da_mon;                      /* Month ( 1 = Jan ) */
};

struct time
{
    unsigned char   ti_min;             /* Minutes */
    unsigned char   ti_hour;            /* Hours */
    unsigned char   ti_hund;            /* Hundredths of seconds */
    unsigned char   ti_sec;             /* Seconds */
};

struct ftime
{
    unsigned int ft_hour:   5;
    unsigned int ft_min:    6;
    unsigned int ft_tsec:   5;
    unsigned int ft_year:   7;
    unsigned int ft_month:  4;
    unsigned int ft_day:    5;
};

struct stat
{
    int    st_dev;
    int    st_ino;
    int    st_mode;
    int    st_nlink;
    int    st_uid;
    int    st_gid;
    int    st_rdev;
    size_t st_size;
    long   st_atime;
    long   st_mtime;
    long   st_ctime;
};

struct dfree
{
        unsigned long df_avail;
        unsigned long df_total;
        unsigned long df_bsec;
        unsigned long df_sclus;
};

extern int getcurdir(int drive, char *path);
extern char *getcwd(char *buffer, int bufflen);
extern int getdisk(void);
extern void getdfree(unsigned char drive, struct dfree *dtable);
extern long setdisk(int drive);

extern int findfirst(const char *filename, struct ffblk *ffblk, int attrib);
extern int findnext(struct ffblk *ffblk);

extern void getdate(struct date *dateRec);
extern void gettime(struct time *timeRec);
extern void setdate(struct date *dateRec);
extern void settime(struct time *timeRec);
extern int getftime(int handle, struct ftime *ftimep);
extern int setftime(int handle, struct ftime *ftimep);

extern struct tm* ftimtotm(struct ftime *f);

extern void delay(unsigned long milliseconds);
extern void sleep(unsigned int seconds);

extern int kbhit(void);
extern int getch(void);
extern int getche(void);
extern int putch(int c);
extern int chdir(char *filename);

extern int fstat(int handle, struct stat *buff);
extern int stat(const char *path, struct stat *buff);

extern int isatty(int handle);
extern long filelength(int handle);

extern size_t coreleft(void);

#endif /* _EXT_H_ */


