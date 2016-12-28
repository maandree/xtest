/* See LICENSE file for copyright and license details. */
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"

char *argv0;
static char delim = '\n';
static long int tests = 0;

static int test_b(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISBLK  (buf.st_mode); }
static int test_c(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISCHR  (buf.st_mode); }
static int test_d(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISDIR  (buf.st_mode); }
static int test_f(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISREG  (buf.st_mode); }
static int test_g(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISGID & buf.st_mode ; }
static int test_h(char *s) { struct stat buf; if (lstat(s, &buf)) return 0; return S_ISLNK  (buf.st_mode); }
static int test_k(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISVTX & buf.st_mode ; }
static int test_p(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISFIFO (buf.st_mode); }
static int test_S(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISSOCK (buf.st_mode); }
static int test_s(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return           buf.st_size ; }
static int test_u(char *s) { struct stat buf; if ( stat(s, &buf)) return 0; return S_ISUID & buf.st_mode ; }

static int test_e(char *s) { return !faccessat(AT_FDCWD, s, F_OK, AT_EACCESS); }
static int test_r(char *s) { return !faccessat(AT_FDCWD, s, R_OK, AT_EACCESS); }
static int test_w(char *s) { return !faccessat(AT_FDCWD, s, W_OK, AT_EACCESS); }
static int test_x(char *s) { return !faccessat(AT_FDCWD, s, X_OK, AT_EACCESS); }

static int
test_t(char *s)
{
	long int fd;
	char *end;
	errno = 0;
	fd = strtol(s, &end, 10);
	if (errno || *end || fd < 0 || fd > INT_MAX)
		return 0;
	return isatty((int)fd);
}

static int (*testmap[])(char *) = {
	test_b,
	test_c,
	test_d,
	test_e,
	test_f,
	test_g,
	test_h,
	test_k,
	test_p,
	test_r,
	test_S,
	test_s,
	test_t,
	test_u,
	test_w,
	test_x
};

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-0bcdefghkLprSstuwx] file ...\n", argv0);
	exit(1);
}

static void
test(char *s)
{
	long int i, j;
	for (i = 0; j = 1L << i, j <= tests; i++) {
		if ((tests & j) && testmap[i](s)) {
			printf("%s%c", s, delim);
			fflush(stdout);
			break;
		}
	}
}

int
main(int argc, char *argv[])
{
	char *line = NULL;
	size_t size = 0;
	ssize_t len;

	ARGBEGIN {
	case '0':
		delim = '\0';
		break;
	case 'b': tests |= 1 << 0; break;
	case 'c': tests |= 1 << 1; break;
	case 'd': tests |= 1 << 2; break;
	case 'e': tests |= 1 << 3; break;
	case 'f': tests |= 1 << 4; break;
	case 'g': tests |= 1 << 5; break;
	case 'h': tests |= 1 << 6; break;
	case 'k': tests |= 1 << 7; break;
	case 'L': tests |= 1 << 6; break;
	case 'p': tests |= 1 << 8; break;
	case 'r': tests |= 1 << 9; break;
	case 'S': tests |= 1 << 10; break;
	case 's': tests |= 1 << 11; break;
	case 't': tests |= 1 << 12; break;
	case 'u': tests |= 1 << 13; break;
	case 'w': tests |= 1 << 14; break;
	case 'x': tests |= 1 << 15; break;
	default:
		usage();
	} ARGEND;

	if (!tests) {
		fprintf(stderr, "%s: at least one test is required\n", argv0);
		return 1;
	}

	if (argc) {
		for (; *argv; argv++)
			test(*argv);
	} else {
		while (len = getdelim(&line, &size, delim, stdin), len >= 0) {
			if (len && line[len - 1] == delim)
				line[len - 1] = '\0';
			test(line);
		}
		if (ferror(stdin)) {
			fprintf(stderr, "%s: <stdin>: %s\n", strerror(errno));
			return 1;
		}
	}

	if (ferror(stdout)) {
		fprintf(stderr, "%s: <stdout>: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}
