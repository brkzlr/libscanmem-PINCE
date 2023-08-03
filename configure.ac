AC_INIT([scanmem],[0.20~dev],[https://github.com/brkzlr/scanmem-PINCE])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-extra-portability foreign])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS
AC_HEADER_STDBOOL

LT_INIT

AM_PROG_CC_C_O

AC_CHECK_FUNCS(getline secure_getenv)
if test "x$ac_cv_func_getline" = "xno"; then
	AC_MSG_ERROR([Cannot build without working getline().])
fi

AC_CHECK_HEADERS(fcntl.h limits.h stddef.h sys/ioctl.h sys/time.h)

AC_FUNC_ALLOCA
AC_FUNC_STRTOD

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_C_BIGENDIAN

# Detect the host OS
AC_CANONICAL_HOST
case "$host_os" in
  linux*)
    AC_MSG_NOTICE([Linux detected])
    ;;
  *)
    AC_MSG_NOTICE([Your platform is not currently supported])
    ;;
esac

# also need to check if the file is zero'ed (some hardened systems)
AC_CHECK_FILE([/proc/self/maps], [], [
	  echo "This system does not seem to have /proc/pid/maps files."
	  exit 1
	  ])

# also need to check this file works
AC_CHECK_FILE([/proc/self/mem], [
	  # LARGEFILE support required for this to work
	  AC_SYS_LARGEFILE
	  ],[
		 # This will hurt performance.
		 echo "This system does not seem to have /proc/pid/mem files."
		 exit 1
		 ])
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([
  Makefile
])


# copied from ubuntu-tweak

dnl AS_AC_EXPAND(VAR, CONFIGURE_VAR)
dnl
dnl example
dnl AS_AC_EXPAND(SYSCONFDIR, $sysconfdir)
dnl will set SYSCONFDIR to /usr/local/etc if prefix=/usr/local

AC_DEFUN([AS_AC_EXPAND],
[
  EXP_VAR=[$1]
  FROM_VAR=[$2]

  dnl first expand prefix and exec_prefix if necessary
  prefix_save=$prefix
  exec_prefix_save=$exec_prefix

  dnl if no prefix given, then use /usr/local, the default prefix
  if test "x$prefix" = "xNONE"; then
    prefix=$ac_default_prefix
  fi
  dnl if no exec_prefix given, then use prefix
  if test "x$exec_prefix" = "xNONE"; then
    exec_prefix=$prefix
  fi

  full_var="$FROM_VAR"
  dnl loop until it doesn't change anymore
  while true; do
    new_full_var="`eval echo $full_var`"
    if test "x$new_full_var" = "x$full_var"; then break; fi
    full_var=$new_full_var
  done

  dnl clean up
  full_var=$new_full_var
  AC_SUBST([$1], "$full_var")

  dnl restore prefix and exec_prefix
  prefix=$prefix_save
  exec_prefix=$exec_prefix_save
])
# end copy

AC_OUTPUT
