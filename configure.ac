# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.66])
AC_INIT([mate-applet-uptime], [0.0.1], [assen.totin@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AM_CONFIG_HEADER([config.h])

# prefix
if test "x$prefix" = "xNONE" ; then
	prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
	exec_prefix=$prefix
fi
PREFIX=$prefix
AC_SUBST([PREFIX])
BINDIR=$exec_prefix/bin
AC_SUBST([BINDIR])
SBINDIR=$exec_prefix/sbin
AC_SUBST([SBINDIR])
DATAROOTDIR=$prefix/share
AC_SUBST([DATAROOTDIR])
LIBEXECDIR=$prefix/libexec
if test "$libexecdir" != '${exec_prefix}/libexec'; then
        LIBEXECDIR=$libexecdir
fi
AC_SUBST([LIBEXECDIR])
PIXMAPDIR=$prefix/share/pixmaps
AC_SUBST([PIXMAPDIR])
ICONSDIR=$prefix/share/icons
AC_SUBST([ICONSDIR])
LOCALEDIR=$prefix/share/locale
AC_SUBST([LOCALEDIR])
AC_DEFINE_UNQUOTED([LOCALEDIR],["$LOCALEDIR"],[Locale files])
TOPLEVELDIR=`pwd`
AC_SUBST([TOPLEVELDIR])

# gettext
AM_GNU_GETTEXT_VERSION(0.18.1)
AM_GNU_GETTEXT([external])
if test $USE_NLS != "yes" ; then
        echo "*** gettext not found. You're losing internationalization."
fi

### Checks for programs

# GCC
AC_PROG_CC
MAKEFILE_CC=`which gcc`
AC_SUBST([MAKEFILE_CC])

# msgfmt - for NSL
AC_CHECK_PROG([FOUND_MSGFMT],[msgfmt],["yes"],["no"])
if test $FOUND_MSGFMT = "no"; then
	echo "*** msgfmt not found. Cannot continue."
	exit
fi

### Checks for libraries.
LIBMATEPANELAPPLET="no"
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_4], [libmatepanelapplet-4.0], [LIBMATEPANELAPPLET="libmatepanelapplet-4.0"], [
        PKG_CHECK_MODULES([LIBMATEPANELAPPLET_3], [libmatepanelapplet-3.0], [LIBMATEPANELAPPLET="libmatepanelapplet-3.0"], [
                PKG_CHECK_MODULES([LIBMATEPANELAPPLET_2], [libmatepanelapplet-2.0], [LIBMATEPANELAPPLET="libmatepanelapplet-2.0"], [
                        AC_MSG_ERROR(["*** libmatepanelapplet not found."])
                ])
        ])
])

### Check for packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])

# Makefiles
APPLET_FLAGS=`$PKG_CONFIG --cflags-only-I $LIBMATEPANELAPPLET`
APPLET_LIBS=`$PKG_CONFIG --libs $LIBMATEPANELAPPLET`
AC_SUBST([APPLET_FLAGS])
AC_SUBST([APPLET_LIBS])

AC_CONFIG_FILES([Makefile src/Makefile images/Makefile po/Makefile.in po/Makefile])

# Service files
AC_CONFIG_FILES([src/org.mate.applets.UptimeApplet.mate-panel-applet src/org.mate.panel.applet.UptimeApplet.service src/org.mate.panel.applet.UptimeApplet.gschema.xml mate-applet-uptime.spec])

# Icons
AC_DEFINE_UNQUOTED([APPLET_ICON_PATH], ["$PIXMAPDIR"], [Define icons location])

# Grand finale
AC_OUTPUT

echo
echo "Configuration complete."
echo
echo "Now, run make."
echo 
