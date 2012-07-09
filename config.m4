PHP_ARG_WITH(ftdi, whether to enable ftdi support,
[  --with-ftdi[=DIR]       Enable ftdi support. DIR is the prefix to libftdi installation directory.], yes)

if test "$PHP_FTDI" != "no"; then


dnl Get PHP version depending on shared/static build

  AC_MSG_CHECKING([PHP version is at least 5.2.0])

  if test -z "${PHP_VERSION_ID}"; then
    if test -z "${PHP_CONFIG}"; then
      AC_MSG_ERROR([php-config not found])
    fi
    PHP_FTDI_FOUND_VERNUM=`${PHP_CONFIG} --vernum`;
    PHP_FTDI_FOUND_VERSION=`${PHP_CONFIG} --version`
  else
    PHP_FTDI_FOUND_VERNUM="${PHP_VERSION_ID}"
    PHP_FTDI_FOUND_VERSION="${PHP_VERSION}"
  fi

  if test "$PHP_FTDI_FOUND_VERNUM" -ge "50200"; then
    AC_MSG_RESULT(yes. found $PHP_FTDI_FOUND_VERSION)
  else 
    AC_MSG_ERROR(no. found $PHP_FTDI_FOUND_VERSION)
  fi

  AC_MSG_CHECKING([for ftdi.h header])
  for i in $PHP_FTDI /usr/local /usr;
  do
    test -r $i/include/ftdi.h && FTDI_PREFIX=$i && FTDI_INC_DIR=$i/include/ && FTDI_OK=1
  done
  	
  if test "$FTDI_OK" != "1"; then
    AC_MSG_ERROR([Unable to find ftdi.h])
  fi
  
  AC_MSG_RESULT([found in $FTDI_INC_DIR])
  
  AC_MSG_CHECKING([for libftdi shared libraries])
  PHP_CHECK_LIBRARY(ftdi, ftdi_init, [
    PHP_ADD_LIBRARY_WITH_PATH(ftdi, $FTDI_PREFIX/lib, FTDI_SHARED_LIBADD)
    PHP_ADD_INCLUDE($FTDI_INC_DIR)
  ],[
    AC_MSG_ERROR([not found. Make sure that libftdi is installed])
  ],[
    FTDI_SHARED_LIBADD -lftdi
  ])
  
  PHP_NEW_EXTENSION(ftdi, ftdi.c, $ext_shared)
  AC_DEFINE(HAVE_FTDI,1,[ ])

  PHP_SUBST(FTDI_SHARED_LIBADD)
fi

