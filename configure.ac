AC_PREREQ([2.57])
AC_INIT(lxtask,[0.1.4], [],lxtask)
AM_INIT_AUTOMAKE()
AC_PROG_INTLTOOL(, [no-xml])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

dnl Check for UNIX variants
AC_AIX()
AC_ISC_POSIX()
#AC_MINIX()

AC_HEADER_STDC()
AC_CHECK_HEADERS([dirent.h pwd.h sys/types.h sys/stat.h sys/param.h \
                  stdio.h stdlib.h string.h unistd.h stdlib.h signal.h])

AC_ARG_ENABLE(gtk3,
[  --enable-gtk3        enable to use gtk-3.0 instead of gtk-2.0 ],
[case "${enableval}" in
  yes)  enable_gtk3=yes ;;
  *)   enable_gtk3=no ;;
esac],[])

echo "x$enable_gtk3"
if test "x$enable_gtk3" = "xyes" ; then
  pkg_modules="gtk+-3.0 >= 3.0.0"
else
  pkg_modules="gtk+-2.0 >= 2.6.0"
fi

PKG_CHECK_MODULES(GTK, [$pkg_modules])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GETTEXT_PACKAGE=lxtask
#AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.14.1)
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

#These codes are NOT maintained by Pootle ca el en_GB lv sq
ALL_LINGUAS="af ar ca cs da de el en_GB es et eu fa fi fr gl hr hu id it ja ko lt lv ml nb nl nn pl ps pt pt_BR ru sk sl sq sv tr uk ur ur_PK vi zh_CN zh_TW"
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
  Makefile
  src/Makefile
  po/Makefile.in
])

echo
echo LXTask....................................... : Version $VERSION
echo
echo Prefix....................................... : $prefix
echo
echo The binary will be installed in $prefix/bin
echo
echo http://lxde.org/
echo
echo Have fun!
echo
