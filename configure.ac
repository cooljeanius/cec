dnl# cec/configure.ac                                          -*- Autoconf -*-
dnl# Run aclocal to generate aclocal.m4
dnl# then run automake
dnl# then run autoconf

AC_PREREQ([2.70])
AC_INIT([cec],[0.4],[cec-users@lists.cs.columbia.edu])
AC_CONFIG_SRCDIR([src/cec-strlxml.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_REQUIRE_AUX_FILE([mkinstalldirs])
AM_INIT_AUTOMAKE([subdir-objects])
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AC_SEARCH_LIBS([strerror],[cposix])
AM_SET_LEADING_DOT
AC_EXEEXT
AC_CANONICAL_HOST

AC_PROG_INSTALL
AC_PROG_LN_S

AC_DEFUN([AC_REQUIRE_PROGS],[
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_MAKE_SET])dnl
  AC_REQUIRE([AC_PROG_CC])dnl
  AC_REQUIRE([AM_PROG_CC_C_O])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_PROGS
AC_LANG([C])dnl

AC_PROG_CPP
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_RANLIB
AC_PROG_GCC_TRADITIONAL
LT_INIT
LT_LANG([C])
LT_LANG([C++])dnl

AC_MSG_CHECKING([for C++ standard library headers])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([
#include <vector>
#include <set>
#include <map>
#include <sstream>

std::vector<int> myvect;
std::set<std::vector<int> > myset;
std::map<char *, std::set<int> > mymap;
std::ostringstream ost;

using std::vector;
using std::map;

vector<int> anothervect;
map<char*, int> anothermap;
])
],[AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no: Warning compilation likely to fail)])
# AX_* macros are from the autoconf-archive package
AX_CXX_GCC_ABI_DEMANGLE
AX_CXX_GNUCXX_HASHMAP
AX_CXX_HAVE_STD
AX_CXX_HAVE_STL
AX_CXX_NAMESPACE_STD
AX_CXX_NAMESPACES
AX_CXX_TEMPLATES

# FIXME: Add an option for the user to specify expat library/header paths

AC_CHECK_HEADER([expat.h],[],[
echo "The required expat header file expat.h was not found."
echo "Please re-run configure with CPPFLAGS=-Idirectory"
exit 1
])
AC_CHECK_LIB([expat],[XML_ParserCreate],[],[
echo "The required expat XML parser library was not found."
echo "Please re-run configure with LIBS=-Ldirectory"
exit 1
])dnl

# Checks for header files.
dnl# message from autoupdate:
dnl# The preprocessor macro `STDC_HEADERS' is obsolete.
dnl# Except in unusual embedded environments, you can safely include all
dnl# ISO C90 headers unconditionally.
AC_CHECK_INCLUDES_DEFAULT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS_ONCE([stdlib.h string.h strings.h xlocale.h])dnl

# These progs are useful for development, but aren't necessary for building:
AC_PATH_PROG([NOTANGLE],[notangle])
AC_PATH_PROG([NOWEAVE],[noweave])dnl

# More documentation tools:
AC_PATH_PROG([LATEX],[latex],[latex])
AC_PATH_PROG([DVIPS],[dvips],[dvips])
AC_PATH_PROG([PS2PDF],[ps2pdf],[ps2pdf])
AC_PATH_PROG([GROFF],[groff],[groff])dnl

# Check for the ability to run ANTLR -- necessary for development, but not
# for a build

AC_SUBST([ANTLR])dnl

AC_MSG_CHECKING([for ANTLR])
AC_CACHE_VAL([ac_cv_found_antlr],
[ac_cv_found_antlr=yes ;
cat << EOF > test.g
options { language="Cpp"; }
class TestParser extends Parser;
file : ;
EOF
java antlr.Tool test.g > antlr-version 2>&1 || ac_cv_found_antlr=no
antlr_version=`sed 's/^.*Version //
s/ .*$//' antlr-version`
rm -f TestParser.cpp TestParser.hpp TestParserTokenTypes.hpp TestParserTokenTypes.txt test.g antlr-version
])
if test "x${ac_cv_found_antlr}" = "xyes"; then
  AC_MSG_RESULT([${antlr_version}])
  ANTLR="java antlr.Tool"
else
  ANTLR=""
  AC_MSG_RESULT([no])
fi
AC_PATH_PROG([ANTLR_BIN],[antlr])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([alarm select strcasecmp strdup])dnl

AS_IF([test "x${ac_cv_func_strcasecmp}" = "xno"],[
  AS_IF([test "x${CXXFLAGS}" = "x"],[
    test -z "${CXXFLAGS}" && export CXXFLAGS=-DANTLR_REALLY_NO_STRCASECMP
  ],[
    test -n "${CXXFLAGS}" && export CXXFLAGS="${CXXFLAGS} -DANTLR_REALLY_NO_STRCASECMP"
  ])dnl
])dnl

# Output.
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile libantlr/Makefile])
AC_CONFIG_FILES([src/cec],[chmod +x src/cec])
AC_OUTPUT
