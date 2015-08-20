CC = clang
CFLAGS = -Wall -Wno-return-type -std=c11
EXECUTABLE = paroxysm
TEST_EXECUTABLE = test
MAIN = main.c
SRCS = lex.c parse.c eval.c drive.c error.c
TEST_SRCS = test.c lex-test.c parse-test.c eval-test.c
OBJS = ${SRCS:.c=.o}
TEST_OBJS = $(TEST_SRCS:.c=.o)

paroxysm: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MAIN) -o $(EXECUTABLE)

test: $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $(TEST_EXECUTABLE)

$(OBJS):
	$(CC) $(CFLAGS) -c $(SRCS)

$(TEST_OBJS):
	$(CC) $(CFLAGS) -c $(TEST_SRCS)

depend:
	$(CC) -MM $(SRCS) $(TEST_SRCS) > Makefile.dep

Makefile.dep:
	touch Makefile.dep

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(EXECUTABLE) $(TEST_EXECUTABLE) core

loc:
	wc -l *.c *.h

realloc:
	wc -l $(SRCS)

include Makefile.dep

