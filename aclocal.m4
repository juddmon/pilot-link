## libtool.m4 - Configure libtool for the target system. -*-Shell-script-*-
## Copyright (C) 1996, 1997 Free Software Foundation, Inc.
## Gordon Matzigkeit <gord@gnu.ai.mit.edu>, 1996
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
##
## As a special exception to the GNU General Public License, if you
## distribute this file as part of a program that contains a
## configuration script generated by Autoconf, you may include it under
## the same distribution terms that you use for the rest of that program.

# serial 7 AM_PROG_LIBTOOL
AC_DEFUN(AM_PROG_LIBTOOL,
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_PROG_RANLIB])
AC_REQUIRE([AM_PATH_PROG_LD])

# Always use our own libtool.
LIBTOOL='$(top_builddir)/libtool'
AC_SUBST(LIBTOOL)

dnl Allow the --disable-shared flag to stop us from building shared libs.
AC_ARG_ENABLE(shared,
[  --enable-shared         build shared libraries [default=yes]],
test "$enableval" = no && libtool_shared=" --disable-shared",
libtool_shared=)

dnl Allow the --disable-static flag to stop us from building static libs.
AC_ARG_ENABLE(static,
[  --enable-static         build static libraries [default=yes]],
test "$enableval" = no && libtool_static=" --disable-static",
libtool_static=)

libtool_flags="$libtool_shared$libtool_static"
test "$silent" = yes && libtool_flags="$libtool_flags --silent"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
[case "$host" in
*-*-irix6*)
  for f in '-32' '-64' '-cckr' '-n32' '-mips1' '-mips2' '-mips3' '-mips4'; do
    if echo " $CC $CFLAGS " | egrep -e "[ 	]$f[	 ]" > /dev/null; then
      LD="${LD-ld} $f"
    fi
  done
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  CFLAGS="$CFLAGS -belf"
  ;;
esac]

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" LD="$LD" RANLIB="$RANLIB" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
|| AC_MSG_ERROR([libtool configure failed])
])

# AM_PATH_PROG_LD - find out which linker is being used by the C compiler
AC_DEFUN(AM_PATH_PROG_LD,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([for ld used by the C compiler ($CC $CFLAGS $LDFLAGS)])
AC_CACHE_VAL(am_cv_path_LD,
[case "$LD" in
  /*)
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
  ;;
  *)
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/ld"; then
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      if "$ac_dir/ld" -v 2>&1 < /dev/null | egrep '(GNU ld|with BFD)' > /dev/null; then
        # If it was GNU ld, only accept it if we're using GCC.
        am_cv_path_LD="$ac_dir/ld"
	test "$ac_cv_prog_gcc" = yes && break
      else
        # If it was not GNU ld, and we are not using GCC, then accept it.
        am_cv_path_LD="$ac_dir/ld"
        break
      fi
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac])
LD="$am_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_SUBST(LD)
])