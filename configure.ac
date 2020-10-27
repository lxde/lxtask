AC_PREREQ([2.57])
AC_INIT(lxtask,[0.1.10], [],lxtask)
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz])

AM_MAINTAINER_MODE([enable])
AC_CONFIG_MACRO_DIR([.])

AC_CONFIG_HEADERS([config.h])

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

# Init intltool
IT_PROG_INTLTOOL

GETTEXT_PACKAGE=lxtask
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

# Generate po/LINGUAS on the fly rather than relying on translators
# to maintain it manually. This also overcome the problem that Transifex
# cannot add a language to po/LINGUAS if a new po file is submitted.
rm -f $srcdir/po/LINGUAS
for po_file in `ls $srcdir/po/*.po | sort`;
do
    lang=`echo "$po_file" | sed "s|.*/po/\(.*\)\.po|\1|g"`
    echo $lang >> $srcdir/po/LINGUAS
done

AM_GLIB_GNU_GETTEXT

AC_ARG_ENABLE(debug,
    [AC_HELP_STRING([--enable-debug],
        [build lxtask with debug support @<:@default=no@:>@])],
    [enable_debug="${enableval}"],
    [enable_debug=no]
)

if test "$enable_debug" = "yes"; then
    # turn on debug and disable optimization
    CPPFLAGS="$CPPFLAGS -DG_ENABLE_DEBUG -O0 -g"
    case "$CC" in
    gcc*)
        CPPFLAGS="$CPPFLAGS -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers"
        ;;
    *)
        ;;
    esac
    dnl Be more strict on portability
    CPPFLAGS="$CPPFLAGS -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=700"
else
    # turn off glib debug checks
    CPPFLAGS="$CPPFLAGS -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"
fi

AC_OUTPUT([
  Makefile
  src/Makefile
  po/Makefile.in
  data/lxtask.1
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
