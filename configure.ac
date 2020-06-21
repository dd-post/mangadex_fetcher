AC_PREREQ([2.69])
AC_INIT(mangadex_fetcher, 1)
AM_INIT_AUTOMAKE(mangadex_fetcher, 1)
AC_LANG([C++])
AC_CONFIG_SRCDIR([src/])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

# See if we have C++17 support
#AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-larchive':
AC_CHECK_LIB([archive], [main])
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h argp.h])

AC_CHECK_HEADER(nlohmann/json.hpp, [], [AC_MSG_ERROR([Could not find nlohmann/json.hpp... nlohmann/json package required])])


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([mkdir setenv strerror strtol])

AC_OUTPUT(Makefile src/Makefile)
