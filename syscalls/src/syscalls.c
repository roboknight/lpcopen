/*** NXP temporary system calls ***/

#include <sys/stat.h>

int _kill(pid_t pid, int signal) { return -1; }

pid_t _getpid(void) { return (pid_t)-1; }

void _exit(int ev) {
	NO_EXIT: goto NO_EXIT;
}

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

int _open(const char *name, int flags, int mode) { return -1; }

int _read(int file, char *ptr, int len) {
	return 0;
}

char *heap_end = 0;
caddr_t _sbrk(int incr) {
	extern char _pvHeapEnd; /* Defined by the linker */
	extern char _pvHeapStart; /* Defined by the linker */

	char *prev_heap_end;

	if (heap_end == 0) heap_end = &_pvHeapStart;

	prev_heap_end = heap_end;

	if (heap_end + incr > &_pvHeapEnd) {
		/* Heap and stack collision */
		return (caddr_t) 0;
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _write(int file, char *ptr, int len) {
	return 0;
}

