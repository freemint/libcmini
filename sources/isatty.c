#include <mint/mintbind.h>
#include <mint/errno.h>
#include <unistd.h>

#ifndef TIOCGPGRP
#define TIOCGPGRP (('T'<< 8) | 6)
#endif

int isatty (int fd) {
	long dummy, retval;
	int rc;

	retval = Fcntl (fd, &dummy, TIOCGPGRP);
	if (retval == -ENOSYS) {
		long oldloc;

		/* save current location */
		oldloc = Fseek (0L, fd, SEEK_CUR);

		/* try to seek ahead one byte */
		if (Fseek (1L, fd, SEEK_CUR) != 0) {
			/* got either a file position or an error (usually
			 * EBADARG indicating a range error from trying to
			 * seek past EOF), so it is not a tty
			 */
			rc = 0;
			
			/* seek back to original location */
			(void) Fseek (oldloc, fd, SEEK_SET);
		}
		else  {
			rc = 1; /* yes, tty */
		}
	}
	else {
		rc = (retval == 0);
	}
	return rc;
}
