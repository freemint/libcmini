#ifndef _GETOPT_H_
#define _GETOPT_H_

extern int getopt(int argc, char * const argv[], const char *optstring);

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

#endif /* _GETOPT_H_ */
