AC_DEFUN(AC_PILOT_LINK_HOOK,
[
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h malloc.h sys/ioctl.h sys/time.h sys/ioctl_compat.h memory.h string.h strings.h unistd.h stdlib.h netinet/in.h dirent.h sys/ndir.h sys/dir.h ndir.h sys/select.h sockio.h netdb.h sys/utsname.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

AC_CACHE_CHECK([for sa_len],
  ac_cv_sa_len,
[AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/socket.h>], [int main(void) {
 struct sockaddr t;t.sa_len = 0;}],
  ac_cv_sa_len=yes,ac_cv_sa_len=no)])
if test $ac_cv_sa_len = yes; then
  AC_DEFINE(HAVE_SA_LEN)
  SA_LEN_FIELD="unsigned char pi_len;"
fi
AC_SUBST(SA_LEN_FIELD)
])

AC_DEFUN([AC_PILOT_LINK],[
	if test x$1 = x; then
		plv=0.10.1
	else
		plv=$1
	fi
		
	GNOME_PILOT_HOOK($plv)
])
