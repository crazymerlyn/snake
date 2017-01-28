AC_PREREQ([2.69])
AC_INIT([snake], [0.1], [https://github.com/crazymerlyn/snake/issues])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

CFLAGS="-g -O2 -Wall -Werror"
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])
PKG_CHECK_MODULES([NCURSES], [ncurses >= 5.0.0])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

AC_OUTPUT
