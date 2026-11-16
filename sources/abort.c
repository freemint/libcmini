#include <stdlib.h>
#include <unistd.h>

void abort(void)
{
	/* abnormal termination: unlike exit(), atexit() handlers must not be called */
	_exit(-1);
}
