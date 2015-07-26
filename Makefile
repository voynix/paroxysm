# Instructions:
#
# 1. Add source files to the line beginning with SRCS = main.cc.
# 2. To build the dependency list, type 'make depend'.  You'll need to
#    do this whenever you add a source file.
# 3. To build the project, type 'make'.
# 4. To start over, type 'make clean', which does not clean the
#    dependency list.
#

CC = clang
CFLAGS = -Wall -g
SRCS = lex.c
TEST_SRCS = test.c
OBJS = ${SRCS:.c=.o}
TEST_OBJS = $(TEST_SRCS:.c=.o)

a.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

test: $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS)

$(OBJS):
	$(CC) $(CFLAGS) -c $(SRCS)

$(TEST_OBJS):
	$(CC) $(CFLAGS) -c $(TEST_SRCS)

depend:
	$(CC) -MM $(SRCS) $(TEST_SRCS) > Makefile.dep

Makefile.dep:
	touch Makefile.dep

clean:
	rm -f $(OBJS) $(TEST_OBJS) a.out core

include Makefile.dep

