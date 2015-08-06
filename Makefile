CC = clang
CFLAGS = -Wall -std=c11 -g
SRCS = lex.c parse.c eval.c
TEST_SRCS = test.c lex-test.c parse-test.c
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

loc:
	wc -l *.c *.h

include Makefile.dep

