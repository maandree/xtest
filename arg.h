/*
 * Copy me if you can.
 * by 20h
 */

#ifndef ARG_H__
#define ARG_H__

extern char *argv0;

/* use main(int argc, char *argv[]) */
#define ARGBEGIN	for (argv0 = *argv, argv++, argc--;\
					argv[0] && argv[0][0] && argv[0][1];\
					argc--, argv++) {\
				char argc_;\
				char **argv_;\
				int brk_;\
				if (argv[0][0] == '-') {\
					if (argv[0][1] == '-' && argv[0][2] == '\0') {\
						argv++;\
						argc--;\
						break;\
					}\
					for (brk_ = 0, argv[0]++, argv_ = argv;\
							argv[0][0] && !brk_;\
							argv[0]++) {\
						if (argv_ != argv)\
							break;\
						argc_ = argv[0][0];\
						switch (argc_)

#define ARGALT(SYMBOL)			}\
				} else if (argv[0][0] == SYMBOL) {\
					for (brk_ = 0, argv[0]++, argv_ = argv;\
							argv[0][0] && !brk_;\
							argv[0]++) {\
						if (argv_ != argv)\
							break;\
						argc_ = argv[0][0];\
						switch (argc_)

#define ARGEND				}\
				} else {\
					break;\
				}\
			}

#endif
