lib_LIBRARIES = libsnake.a
libsnake_a_SOURCES = snake.c snake.h

bin_PROGRAMS = snake
snake_SOURCES = main.c snake.h
snake_CFLAGS = @NCURSES_CFLAGS@
snake_LDADD = libsnake.a @NCURSES_LIBS@


