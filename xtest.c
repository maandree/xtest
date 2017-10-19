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
static long int pos_tests = 0;
static long int neg_tests = 0;

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

#define LIST_TESTS\
	X('b', test_b, 0)\
	X('c', test_c, 1)\
	X('d', test_d, 2)\
	X('e', test_e, 3)\
	X('f', test_f, 4)\
	X('g', test_g, 5)\
	X('h', test_h, 6)\
	X('k', test_k, 7)\
	X('p', test_p, 8)\
	X('r', test_r, 9)\
	X('S', test_S, 10)\
	X('s', test_s, 11)\
	X('t', test_t, 12)\
	X('u', test_u, 13)\
	X('w', test_w, 14)\
	X('x', test_x, 15)

#define LIST_SYNONYMS\
	X('L', test_h, 6)

static int (*testmap[])(char *) = {
#define X(FLAG, FUNC, NUM) FUNC,
	LIST_TESTS
#undef X
};

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-0bcdefghkLprSstuwx] [+bcdefghkLprSstuwx] [file ...]\n", argv0);
	exit(1);
}

static void
test(char *s)
{
	long int i, j;
	for (i = 0; j = 1L << i, j <= pos_tests; i++) {
		if ((pos_tests & j) && testmap[i](s)) {
			printf("%s%c", s, delim);
			fflush(stdout);
			break;
		}
	}
	for (i = 0; j = 1L << i, j <= neg_tests; i++) {
		if ((neg_tests & j) && !testmap[i](s)) {
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
#define X(FLAG, FUNC, NUM)\
	case FLAG: pos_tests |= 1 << NUM; break;
	LIST_TESTS
	LIST_SYNONYMS
#undef X
	default:
		usage();
	} ARGALT('+') {
#define X(FLAG, FUNC, NUM)\
	case FLAG: neg_tests |= 1 << NUM; break;
	LIST_TESTS
	LIST_SYNONYMS
#undef X
	default:
		usage();
	} ARGEND;

	if (!pos_tests && !neg_tests) {
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
			fprintf(stderr, "%s: <stdin>: %s\n", argv0, strerror(errno));
			return 1;
		}
	}

	if (ferror(stdout)) {
		fprintf(stderr, "%s: <stdout>: %s\n", argv0, strerror(errno));
		return 1;
	}
	return 0;
}
