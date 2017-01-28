TESTS = check_snake

check_PROGRAMS = check_snake
check_snake_SOURCES = check_snake.c $(top_srcdir)/src/snake.h
check_snake_CFLAGS = @CHECK_CFLAGS@
check_snake_LDADD = $(top_builddir)/src/libsnake.a @CHECK_LIBS@

check_snake.c: check_snake.ts
	checkmk $< > $@

